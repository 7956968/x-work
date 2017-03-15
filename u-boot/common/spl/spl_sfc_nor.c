#include <common.h>
#include <config.h>
#include <spl.h>
#include <spi.h>
#include <asm/io.h>
#include <nand.h>
#include <asm/arch/sfc.h>
#include <asm/arch/spi.h>
#include <asm/arch/clk.h>
#include <asm/nvrw_interface.h>
#include <linux/compiler.h>

#include "../../drivers/spi/jz_spi.h"

static struct spi_quad_mode *quad_mode = NULL;
/* Borrow the uboot space */
static struct nor_sharing_params *pdata = CONFIG_FLASH_PARAMS_BASE;

unsigned int quad_mode_is_set = 0;

#ifdef CONFIG_SPI_QUAD
#define SFC_QUAD_MODE 1
#else
#define SFC_QUAD_MODE 0
#endif

struct jz_sfc {
	unsigned int  addr;
	unsigned int  len;
	unsigned int  cmd;
	unsigned int  addr_plus;
	unsigned char channel;
	unsigned char dir;      /* read or write */
	unsigned char mode;
	unsigned char daten;
	unsigned char addr_size;
	unsigned char pollen;
	unsigned char phase;
	unsigned char dummy_byte;
};


/*
 * Weak default function for board specific boot type.
 * Some boards/platforms might not need it, so just provide
 * an empty stub here.
 *
 * @return return 0 on default, return 1 on boot uboot, return 2 on boot kernel
 */
__weak int spl_get_boot_type(void)
{
	return 0;
}

static inline uint32_t jz_sfc_readl(unsigned int offset)
{
	return readl(SFC_BASE + offset);
}

static inline void jz_sfc_writel(unsigned int value, unsigned int offset)
{
	writel(value, SFC_BASE + offset);
}

static inline void is_end_and_clear(void)
{
	unsigned int val;
	val = jz_sfc_readl(SFC_SR);
	while (!(val & END)){
		val = jz_sfc_readl(SFC_SR);
	}
	if ((jz_sfc_readl(SFC_SR)) & END)
		jz_sfc_writel(CLR_END,SFC_SCR);
}

static void read_fifo_data(unsigned int *data, unsigned int length)
{
	unsigned int n;
	while(!(jz_sfc_readl(SFC_SR) & RECE_REQ))
		;
	jz_sfc_writel(CLR_RREQ, SFC_SCR);
	for(n =0 ; n < length; n ++) {
		*data = jz_sfc_readl(SFC_DR);
		data++;
	}
}

static void write_fifo_data(unsigned int *data, unsigned int length)
{
	unsigned int n;
	while(!(jz_sfc_readl(SFC_SR) & TRAN_REQ))
		;
	jz_sfc_writel(CLR_TREQ, SFC_SCR);
	for(n =0 ; n < length; n ++) {
		jz_sfc_writel(*data, SFC_DR);
		data++;
	}
}

static int sfc_data_ops(unsigned int *data, unsigned int size, void (*ops)(unsigned int*, unsigned int))
{
	unsigned int *end = data + (size + 3) / 4;
	while(data < (end - THRESHOLD)) {
		ops(data, THRESHOLD);
		data += THRESHOLD;
	}
	if(data < end)
		ops(data, end - data);
	is_end_and_clear();

	return 0;
}

static inline void sfc_set_transfer(struct jz_sfc *hw)
{
	unsigned int val;

	val = jz_sfc_readl(SFC_GLB);
	val &= ~TRAN_DIR;
	val |= hw->dir << TRAN_DIR_OFFSET;
	jz_sfc_writel(val,SFC_GLB);

	jz_sfc_writel(hw->len, SFC_TRAN_LEN);

	val = jz_sfc_readl(SFC_TRAN_CONF(hw->channel));
	val &= ~(ADDR_WIDTH_MSK | CMDEN | DATEEN | CMD_MSK \
		| TRAN_CONF_DMYBITS_MSK | TRAN_MODE_MSK);
	val |= (hw->addr_size << ADDR_WIDTH_OFFSET | CMDEN \
		| hw->daten << TRAN_DATEEN_OFFSET | hw->cmd << CMD_OFFSET \
		| hw->dummy_byte << TRAN_CONF_DMYBITS_OFFSET \
		| hw->mode << TRAN_MODE_OFFSET);
	jz_sfc_writel(val,SFC_TRAN_CONF(hw->channel));

	jz_sfc_writel(hw->addr, SFC_DEV_ADDR(hw->channel));
	jz_sfc_writel(hw->addr_plus,SFC_DEV_ADDR_PLUS(hw->channel));
}

static inline void sfc_send_cmd(struct jz_sfc *sfc)
{
	sfc_set_transfer(sfc);
	jz_sfc_writel(1 << 2,SFC_TRIG);
	jz_sfc_writel(START,SFC_TRIG);

	/*this must judge the end status*/
	if(sfc->daten == 0)
		is_end_and_clear();
}

static void sfc_set_quad_mode(void)
{
	unsigned char cmd[4];
	unsigned int buf = 0;
	unsigned int tmp = 0;
	int i = 10;
	struct jz_sfc sfc;

	if(quad_mode != NULL){
		cmd[0] = CMD_WREN;
		cmd[1] = quad_mode->WRSR_CMD;
		cmd[2] = quad_mode->RDSR_CMD;
		cmd[3] = CMD_RDSR;

		sfc.cmd = cmd[0];
		sfc.addr = 0;
		sfc.addr_size = 0;
		sfc.addr_plus = 0;
		sfc.dummy_byte = 0;
		sfc.daten = 0;
		sfc.len = 0;
		sfc.dir = GLB_TRAN_DIR_WRITE;
		sfc.channel = 0;
		sfc.mode = TRAN_SPI_STANDARD;
		sfc_send_cmd(&sfc);

		sfc.cmd = cmd[1];
		sfc.len = quad_mode->WD_DATE_SIZE;
		sfc.daten = 1;
		sfc_send_cmd(&sfc);
		sfc_data_ops(&quad_mode->WRSR_DATE,1, write_fifo_data);

		sfc.cmd = cmd[3];
		sfc.len = 1;
		sfc.daten = 1;
		sfc.dir = GLB_TRAN_DIR_READ;
		sfc_send_cmd(&sfc);
		sfc_data_ops(&tmp, 1, read_fifo_data);

		while(tmp & CMD_SR_WIP) {
			sfc.cmd = cmd[3];
			sfc.len = 1;
			sfc.daten = 1;
			sfc.dir = GLB_TRAN_DIR_READ;
			sfc_send_cmd(&sfc);
			sfc_data_ops(&tmp, 1, read_fifo_data);
		}
		sfc.cmd = cmd[2];
		sfc.len = quad_mode->WD_DATE_SIZE;
		sfc.daten = 1;
		sfc_send_cmd(&sfc);
		sfc_data_ops(&buf, 1, read_fifo_data);
		while(!(buf & quad_mode->RDSR_DATE)&&((i--) > 0)) {
			sfc.cmd = cmd[2];
			sfc.len = quad_mode->WD_DATE_SIZE;
			sfc.daten = 1;
			sfc_send_cmd(&sfc);
			sfc_data_ops(&buf, 1, read_fifo_data);
		}
		quad_mode_is_set = 1;
		/* printf("set quad mode is enable.the buf = %x\n",buf); */
	} else {
		printf("WARN!!!: the nor flash not support quad_mode\n");
	}
}

static void sfc_nor_read_params(unsigned int offset)
{
	unsigned int len;
	struct jz_sfc sfc;

	len = sizeof(struct nor_sharing_params);
	sfc.cmd = CMD_READ;
	sfc.addr = offset;
	sfc.addr_size = 3;
	sfc.addr_plus = 0;
	sfc.dummy_byte = 0;
	sfc.daten = 1;
	sfc.len = len;
	sfc.dir = GLB_TRAN_DIR_READ;
	sfc.channel = 0;
	sfc.mode = TRAN_SPI_STANDARD;

	sfc_send_cmd(&sfc);
	sfc_data_ops((unsigned int *)pdata, len, read_fifo_data);
}

static inline int get_nor_info(unsigned int offset)
{
#ifndef CONFIG_BURNER
	sfc_nor_read_params(offset);
	if(pdata->magic == NOR_MAGIC) {
		if(pdata->version == CONFIG_NOR_VERSION){
			quad_mode = &pdata->norflash_params.quad_mode;
			if(pdata->norflash_params.addrsize == 4)
				printf("WARNNING: xImage not support addr > 16M\n");
			return 0;
		}
	}
#endif
	return -1;
}

static void reset_nor(void)
{
	struct jz_sfc sfc;

	sfc.cmd = 0x66;
	sfc.addr = 0;
	sfc.addr_size = 0;
	sfc.addr_plus = 0;
	sfc.dummy_byte = 0;
	sfc.daten = 0;
	sfc.len = 0;
	sfc.dir = GLB_TRAN_DIR_READ;
	sfc.channel = 0;
	sfc.mode = TRAN_SPI_STANDARD;
	sfc_send_cmd(&sfc);
	sfc.cmd = 0x99;
	sfc_send_cmd(&sfc);
	udelay(60);
}

void sfc_init(void)
{
	int err;
	unsigned int val;

	/*the sfc clk is 1/2 ssi clk */
	clk_set_rate(SSI, 150000000);

	jz_sfc_writel(3 << 1 ,SFC_TRIG);

	val = jz_sfc_readl(SFC_GLB);
	val &= ~(TRAN_DIR | OP_MODE | THRESHOLD_MSK);
	val |= (WP_EN | THRESHOLD << THRESHOLD_OFFSET);
	jz_sfc_writel(val,SFC_GLB);

	val = jz_sfc_readl(SFC_DEV_CONF);
	val &= ~(CMD_TYPE | CPHA | CPOL | SMP_DELAY_MSK |
			THOLD_MSK | TSETUP_MSK | TSH_MSK);
	val |= (CEDL | HOLDDL | WPDL | (1 << TSETUP_OFFSET) | (1 << SMP_DELAY_OFFSET));
	jz_sfc_writel(val,SFC_DEV_CONF);

	val = CLR_END | CLR_TREQ | CLR_RREQ | CLR_OVER | CLR_UNDER;
	jz_sfc_writel(val,SFC_INTC);
	jz_sfc_writel(0,SFC_CGE);

	reset_nor();

	err = get_nor_info(CONFIG_SPIFLASH_PART_OFFSET);
	if(err < 0){
		printf("the sfc quad mode err,check your soft code\n");
	} else {
		sfc_set_quad_mode();
	}
}

static int jz_sfc_set_address_mode(int on)
{
	unsigned char cmd[3];
	unsigned int  buf = 0;
	struct jz_sfc sfc;

	if (pdata->norflash_params.addrsize != 4)
		return 0;

	cmd[0] = CMD_WREN;
	if(on == 1){
		cmd[1] = CMD_EN4B;
	}else{
		cmd[1] = CMD_EX4B;
	}
	cmd[2] = CMD_RDSR;

	sfc.cmd = cmd[0];
	sfc.len = 0;
	sfc.addr = 0;
	sfc.addr_size = 0;
	sfc.dummy_byte = 0;
	sfc.daten = 0;
	sfc.dir = GLB_TRAN_DIR_WRITE;
	sfc.addr_plus = 0;
	sfc.channel = 0;
	sfc.mode = TRAN_SPI_STANDARD;
	sfc_send_cmd(&sfc);

	sfc.cmd = cmd[1];
	sfc_send_cmd(&sfc);

	sfc.cmd = cmd[2];
	sfc.len = 1;
	sfc.daten = 1;
	sfc.dir = GLB_TRAN_DIR_READ;
	sfc_send_cmd(&sfc);

	sfc_data_ops((unsigned int *)&buf, 1, read_fifo_data);
	while(buf & CMD_SR_WIP) {
		sfc.cmd = cmd[2];
		sfc.len = 1;
		sfc.daten = 1;
		sfc.dir = GLB_TRAN_DIR_READ;
		sfc_send_cmd(&sfc);
		sfc_data_ops((unsigned int *)&buf, 1, read_fifo_data);
	}

	return 0;
}

void sfc_nor_load(unsigned int offset, unsigned int size, unsigned int data)
{
	unsigned char cmd;
	struct jz_sfc sfc;

	/* enable 4bit mode */
	jz_sfc_set_address_mode(1);

	cmd = CMD_READ;

	sfc.addr = offset;
	sfc.addr_size = pdata->norflash_params.addrsize;
	sfc.addr_plus = 0;
	sfc.daten = 1;
	sfc.len = size;
	sfc.dir = GLB_TRAN_DIR_READ;
	sfc.channel = 0;
	sfc.mode = TRAN_SPI_STANDARD;
	sfc.dummy_byte = 0;

	if(SFC_QUAD_MODE == 1){
		cmd  = quad_mode->cmd_read;
		sfc.mode = quad_mode->sfc_mode;
		sfc.dummy_byte = quad_mode->dummy_byte;
	}

	sfc.cmd = cmd;
	sfc_send_cmd(&sfc);
	sfc_data_ops((unsigned int *)data, size, read_fifo_data);

	/* disable 4bit mode */
	jz_sfc_set_address_mode(0);
}

void spl_load_kernel(struct image_header *header, long offset)
{
	sfc_nor_load(offset, sizeof(struct image_header), CONFIG_SYS_TEXT_BASE);

	spl_parse_image_header(header);

	sfc_nor_load(offset, spl_image.size, spl_image.load_addr);
}

void spl_load_uboot(struct image_header *header)
{
	spl_parse_image_header(header);
	sfc_nor_load(CONFIG_UBOOT_OFFSET, CONFIG_SYS_MONITOR_LEN,CONFIG_SYS_TEXT_BASE);
}

void *spl_get_nvinfo(void)
{
	nvinfo_t *nvinfo = (nvinfo_t *)CONFIG_SPL_NV_BASE;
	int i = 0;
	int erasesize = 0;

	if (pdata->magic != 0x726f6e) {
		debug("bad norinfo.\n");
		return NULL;
	}

	erasesize = pdata->norflash_params.erasesize;

	for (i = 0; i < 2; i++) {
		sfc_nor_load(NV_AREA_BASE_ADDR + i * erasesize, sizeof(nvinfo_t), nvinfo);
		if (*(int *)nvinfo->start_magic == 0x41544f && *(int *)nvinfo->end_magic == 0x41544f) {
			debug("nvinfo->update_flag=%d, nvinfo->update_process: %d.\n",
				  nvinfo->update_flag, nvinfo->update_process);
			break;
		}
	}

	if (i == 0 || i == 1) {
		debug("nv is in #%d.\n", i);
		return nvinfo;
	} else {
		return NULL;
	}
}

char *spl_sfc_nor_load_image(void)
{
	/* 0: default; 1: boot uboot; 2: boot kernel */
	int boot_type;

	char *cmdargs = NULL;
	struct image_header *header = (struct image_header *)CONFIG_SYS_TEXT_BASE;

#if defined(CONFIG_SPL_OS_BOOT) && !defined(CONFIG_SPECIAL_OTA)
	nvinfo_t *nvinfo = NULL;
#endif
	sfc_init();

	boot_type = spl_get_boot_type();
	if (boot_type == 1) {
		spl_load_uboot(header);
	} else if (boot_type == 2) {
		spl_load_kernel(header, CONFIG_SPL_OS_OFFSET);
		cmdargs = CONFIG_SYS_SPL_ARGS_ADDR;
	} else if (boot_type == 0) {
#ifdef CONFIG_SPL_OS_BOOT
#ifndef CONFIG_SPECIAL_OTA
		nvinfo = spl_get_nvinfo();
		if (!nvinfo) {
			/* if magic not valid, force to nonupdate status */
			printf("Invalid nvinfo.\n");
			nvinfo->update_flag = FLAG_NONUPDATE;
		}
		if (nvinfo->update_flag == FLAG_NONUPDATE) {
			spl_load_kernel(header, CONFIG_SPL_OS_OFFSET);
			cmdargs = CONFIG_SYS_SPL_ARGS_ADDR;
		} else if (nvinfo->update_flag == FLAG_UPDATE) {
			switch (nvinfo->update_process) {
			case PROCESS_2:
				debug("update, process: %d, load kernel from 0x%x\n",
					  nvinfo->update_process, CONFIG_SPL_OTA_OS_OFFSET);
				spl_load_kernel(header, CONFIG_SPL_OTA_OS_OFFSET);
				cmdargs = CONFIG_SYS_SPL_OTA_ARGS_ADDR;
				break;
			case PROCESS_1:
			case PROCESS_3:
			case PROCESS_DONE:
				debug("update, process: %d, load kernel from 0x%x\n",
					  nvinfo->update_process, CONFIG_SPL_OS_OFFSET);
				spl_load_kernel(header, CONFIG_SPL_OS_OFFSET);
				cmdargs = CONFIG_SYS_SPL_ARGS_ADDR;
				break;
			}
		} else {
			debug("Invalid update flag: %d.\n", nvinfo->update_flag);
		}
#else /* CONFIG_SPECIAL_OTA */
		spl_load_kernel(header, CONFIG_SPL_OS_OFFSET);
		cmdargs = CONFIG_SYS_SPL_ARGS_ADDR;
#endif /* CONFIG_SPECIAL_OTA */
#else /* CONFIG_SPL_OS_BOOT */
		spl_load_uboot(header);
#endif /* CONFIG_SPL_OS_BOOT */
	}

	return cmdargs;
}
