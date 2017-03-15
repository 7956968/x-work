#include <common.h>
#include <config.h>
#include <spl.h>
#include <asm/io.h>
#include <nand.h>
#include <asm/arch/clk.h>
#include <asm/arch/base.h>
#include <asm/arch/sfc.h>
#include <asm/nvrw_interface.h>
//#include <asm/arch/spi.h>
int flag = 0;
int sfc_is_init = 0;
unsigned int sfc_rate = 0;
unsigned int sfc_quad_mode = 0;
unsigned int quad_mode_is_set = 0;
int column_cmdaddr_bytes = 0;
struct jz_sfc {
	unsigned int  addr;
	unsigned int  len;
	unsigned int  cmd;
	unsigned int  addr_plus;
	unsigned int  sfc_mode;
	unsigned char daten;
	unsigned char addr_len;
	unsigned char pollen;
	unsigned char phase;
	unsigned char dummy_byte;
};

#define SSI_BASE SSI0_BASE
#define SPL_TYPE_FLAG_LEN	6

#ifdef CONFIG_SPI_QUAD
#define SFC_QUAD_MODE 1
#else
#define SFC_QUAD_MODE 0
#endif

static inline uint32_t jz_sfc_readl(unsigned int offset)
{
	return readl(SFC_BASE + offset);
}

static inline void jz_sfc_writel(unsigned int value, unsigned int offset)
{
	writel(value, SFC_BASE + offset);
}


unsigned int sfc_fifo_num()
{
	unsigned int tmp;
	tmp = jz_sfc_readl(SFC_SR);
	tmp &= (0x7f << 16);
	tmp = tmp >> 16;
	return tmp;
}
void sfc_dev_addr_dummy_bytes(int channel, unsigned int value)
{
	unsigned int tmp;
	tmp = jz_sfc_readl(SFC_TRAN_CONF(channel));
	tmp &= ~TRAN_CONF_DMYBITS_MSK;
	tmp |= (value << TRAN_CONF_DMYBITS_OFFSET);
	jz_sfc_writel(tmp,SFC_TRAN_CONF(channel));
}
void sfc_transfer_direction(int value)
{
	if(value == 0) {
		unsigned int tmp;
		tmp = jz_sfc_readl(SFC_GLB);
		tmp &= ~TRAN_DIR;
		jz_sfc_writel(tmp,SFC_GLB);
	} else {
		unsigned int tmp;
		tmp = jz_sfc_readl(SFC_GLB);
		tmp |= TRAN_DIR;
		jz_sfc_writel(tmp,SFC_GLB);
	}
}
void sfc_set_length(int value)
{
	jz_sfc_writel(value,SFC_TRAN_LEN);
}
void sfc_set_addr_length(int channel, unsigned int value)
{
	unsigned int tmp;
	tmp = jz_sfc_readl(SFC_TRAN_CONF(channel));
	tmp &= ~(ADDR_WIDTH_MSK);
	tmp |= (value << ADDR_WIDTH_OFFSET);
	jz_sfc_writel(tmp,SFC_TRAN_CONF(channel));
}
void sfc_cmd_en(int channel, unsigned int value)
{
	if(value == 1) {
		unsigned int tmp;
		tmp = jz_sfc_readl(SFC_TRAN_CONF(channel));
		tmp |= CMDEN;
		jz_sfc_writel(tmp,SFC_TRAN_CONF(channel));
	} else {
		unsigned int tmp;
		tmp = jz_sfc_readl(SFC_TRAN_CONF(channel));
		tmp &= ~CMDEN;
		jz_sfc_writel(tmp,SFC_TRAN_CONF(channel));
	}
}

void sfc_data_en(int channel, unsigned int value)
{
	if(value == 1) {
	unsigned int tmp;
	tmp = jz_sfc_readl(SFC_TRAN_CONF(channel));
	tmp |= DATEEN;
	jz_sfc_writel(tmp,SFC_TRAN_CONF(channel));
	} else {
	unsigned int tmp;
	tmp = jz_sfc_readl(SFC_TRAN_CONF(channel));
	tmp &= ~DATEEN;
	jz_sfc_writel(tmp,SFC_TRAN_CONF(channel));
	}
}

void sfc_write_cmd(int channel, unsigned int value)
{
	unsigned int tmp;
	tmp = jz_sfc_readl(SFC_TRAN_CONF(channel));
	tmp &= ~CMD_MSK;
	tmp |= value;
	jz_sfc_writel(tmp,SFC_TRAN_CONF(channel));
}
void sfc_dev_addr(int channel, unsigned int value)
{
	jz_sfc_writel(value, SFC_DEV_ADDR(channel));
}
void sfc_dev_addr_plus(int channel, unsigned int value)
{
	jz_sfc_writel(value,SFC_DEV_ADDR_PLUS(channel));
}
void sfc_set_mode(int channel, int value)
{
	unsigned int tmp;
	tmp = jz_sfc_readl(SFC_TRAN_CONF(channel));
	tmp &= ~(TRAN_MODE_MSK);
	tmp |= (value << TRAN_MODE_OFFSET);
	jz_sfc_writel(tmp,SFC_TRAN_CONF(channel));
}
void sfc_set_transfer(struct jz_sfc *hw,int dir)
{
	if(dir == 1)
		sfc_transfer_direction(GLB_TRAN_DIR_WRITE);
	else
		sfc_transfer_direction(GLB_TRAN_DIR_READ);
	sfc_set_length(hw->len);
	sfc_set_addr_length(0, hw->addr_len);
	sfc_cmd_en(0, 0x1);
	sfc_data_en(0, hw->daten);
	sfc_write_cmd(0, hw->cmd);
	sfc_dev_addr(0, hw->addr);
	sfc_dev_addr_plus(0, hw->addr_plus);
	sfc_dev_addr_dummy_bytes(0,hw->dummy_byte);
	sfc_set_mode(0,hw->sfc_mode);
}
void sfc_send_cmd(unsigned char *cmd,unsigned int len,unsigned int addr ,unsigned addr_len,unsigned dummy_byte,unsigned int daten,unsigned char dir)
{
	struct jz_sfc sfc;
	unsigned int reg_tmp = 0;
	sfc.cmd = *cmd;
	sfc.addr_len = addr_len;
	sfc.addr = addr;
	sfc.addr_plus = 0;
	sfc.dummy_byte = dummy_byte;
	sfc.daten = daten;
	sfc.len = len;

	if((cmd[0] == 0x6b) || (cmd[0] == 0x32)){
		sfc.sfc_mode = TRAN_SPI_QUAD;
	}else{
		sfc.sfc_mode = TRAN_SPI_STANDARD;
	}
	sfc_set_transfer(&sfc,dir);
	jz_sfc_writel(1 << 2,SFC_TRIG);
	jz_sfc_writel(START,SFC_TRIG);
	/*this must judge the end status*/
	if((daten == 0))
	{
		reg_tmp = jz_sfc_readl(SFC_SR);
		while (!(reg_tmp & END)){
			reg_tmp = jz_sfc_readl(SFC_SR);
		}
		if ((jz_sfc_readl(SFC_SR)) & END)
			jz_sfc_writel(CLR_END,SFC_SCR);
	}
}
static int sfc_nand_read_data(unsigned int *data, unsigned int length)
{
	unsigned int tmp_len = 0;
	unsigned int fifo_num = 0;
	unsigned int i;
	unsigned int reg_tmp = 0;
	unsigned int  len = (length + 3) / 4 ;
	unsigned int time_out = 1000;

	while(1){
		reg_tmp = jz_sfc_readl(SFC_SR);
		if (reg_tmp & RECE_REQ) {
			jz_sfc_writel(CLR_RREQ,SFC_SCR);
			if ((len - tmp_len) > THRESHOLD)
				fifo_num = THRESHOLD;
			else {
				fifo_num = len - tmp_len;
			}
			for (i = 0; i < fifo_num; i++) {
				*data = jz_sfc_readl(SFC_DR);
				data++;
				tmp_len++;
			}
		}
		if (tmp_len == len)
		break;
	}
	reg_tmp = jz_sfc_readl(SFC_SR);
	while (!(reg_tmp & END)){
		reg_tmp = jz_sfc_readl(SFC_SR);
	}
	if ((jz_sfc_readl(SFC_SR)) & END)
		jz_sfc_writel(CLR_END,SFC_SCR);
	return 0;
}

static int sfc_nand_write_data(unsigned int *data, unsigned int length)
{
        unsigned int tmp_len = 0;
        unsigned int fifo_num = 0;
        unsigned int i;
        unsigned int reg_tmp = 0;
        unsigned int  len = (length + 3) / 4 ;

        while(1){
                reg_tmp = jz_sfc_readl(SFC_SR);
                if (reg_tmp & TRAN_REQ) {
                        jz_sfc_writel(CLR_TREQ,SFC_SCR);
                        if ((len - tmp_len) > THRESHOLD)
                                fifo_num = THRESHOLD;
                        else {
                                fifo_num = len - tmp_len;
                        }

                        for (i = 0; i < fifo_num; i++) {
                                jz_sfc_writel(*data,SFC_DR);
                                data++;
                                tmp_len++;
                        }
                }
                if (tmp_len == len)
                        break;
        }

        reg_tmp = jz_sfc_readl(SFC_SR);
        while (!(reg_tmp & END)){
                reg_tmp = jz_sfc_readl(SFC_SR);
        }

        if ((jz_sfc_readl(SFC_SR)) & END)
                jz_sfc_writel(CLR_END,SFC_SCR);


        return 0;
}

static void get_sfcnand_base_param(int *pagesize, int *ppb)
{
	unsigned char *spl_flag = (unsigned char *)0xF4001000;
	int type_len = SPL_TYPE_FLAG_LEN;

	*pagesize = spl_flag[type_len + 5] * 1024;//pagesize off 5,blocksize off 4
	*ppb = spl_flag[type_len + 4] * 32;//pagesize off 5,blocksize off 4
}

static int sfc_read_page(unsigned int page,unsigned char *dst_addr,int pagesize)
{
	unsigned char cmd[5];
	unsigned int src_addr;
	volatile unsigned int read_buf=0;
	u_char dummy;
	int column = 0;
	int t_read = 120;
	unsigned int i=0;

	/* the paraterms is
	* cmd , datelen,
	* addr,addr_len
	* dummy_byte, daten
	* dir 0,read 1.write
	*
	* */
	cmd[0]=0x13;
	sfc_send_cmd(&cmd[0],0,page,3,0,0,0);

	cmd[0]=0x0f;//get feature
	sfc_send_cmd(&cmd[0],1,0xc0,1,0,1,0);
	sfc_nand_read_data(&read_buf,1);

	while((read_buf & 0x1))
	{
		cmd[0]=0x0f;//get feature
		sfc_send_cmd(&cmd[0],1,0xc0,1,0,1,0);
		sfc_nand_read_data(&read_buf,1);
	}
	if((read_buf & 0x30) == 0x20)
	{
		printf("read error pageid\n");
		return -1;
	}
	if(SFC_QUAD_MODE == 1)
		cmd[0]=0x6b;
	else
		cmd[0]=0x03;
	column=(column<<8)&0xffffff00;
	sfc_send_cmd(&cmd[0],pagesize,column,column_cmdaddr_bytes,0,1,0);
	sfc_nand_read_data(dst_addr,pagesize);
//	printf("---------column=%d,dst_addr=%x,pagesize=%d\n",column,dst_addr,pagesize);
}

static int sfc_nand_read_oob(unsigned int page,unsigned char *dst_addr,int len, int pagesize)
{
	unsigned char cmd[5];
	unsigned int src_addr;
	volatile unsigned int read_buf=0;
	u_char dummy;
	int column = pagesize;
	int t_read = 120;
	unsigned int i=0;

	/* the paraterms is
	* cmd , datelen,
	* addr,addr_len
	* dummy_byte, daten
	* dir 0,read 1.write
	*
	* */
	cmd[0]=0x13;
	sfc_send_cmd(&cmd[0],0,page,3,0,0,0);

	cmd[0]=0x0f;//get feature
	sfc_send_cmd(&cmd[0],1,0xc0,1,0,1,0);
	sfc_nand_read_data(&read_buf,1);

	while((read_buf & 0x1))
	{
		cmd[0]=0x0f;//get feature
		sfc_send_cmd(&cmd[0],1,0xc0,1,0,1,0);
		sfc_nand_read_data(&read_buf,1);
	}
	if((read_buf & 0x30) == 0x20)
	{
		printf("read oob error \n");
		return 1;
	}
	if(SFC_QUAD_MODE == 1)
		cmd[0]=0x6b;
	else
		cmd[0]=0x03;
	column=(column<<8)&0xffffff00;
	sfc_send_cmd(&cmd[0],len,column,column_cmdaddr_bytes,0,1,0);
	sfc_nand_read_data(dst_addr,len);
	return 0;
}

static int badblk_check(int len,unsigned char *buf)
{
        int i,bit0_cnt = 0;
        unsigned short *check_buf = (unsigned short *)buf;

        if(check_buf[0] != 0xffff){
                for(i = 0; i < len * 8; i++){
                        if(!((check_buf[0] >> i) & 0x1))
                                bit0_cnt++;
                }
        }
        if(bit0_cnt > 6 * len)
                return 1;// is bad blk

        return 0;
}

static int sfc_nand_block_isbad(int block, int ppb, int pagesize)
{
	int check_len = 2;
	int ret = 0;
	unsigned char check_buf[] = {0xaa,0xaa};
	int page = block * ppb;

	ret = sfc_nand_read_oob(page, check_buf, check_len, pagesize);
	if (ret)
		return 1;
	if(badblk_check(check_len,check_buf))
		return 1;
	return 0;
}

void sfc_nand_load(long offs,long size,void *dst)
{
	int pagesize, blocksize, ppb, page, block, blk_start, blk_cnt;
	int pagecopy_cnt = 0;
	unsigned int i=0;
	get_sfcnand_base_param(&pagesize, &ppb);
	unsigned char *buf=dst;
	page = offs / pagesize;
	blocksize = ppb * pagesize;

	blk_start = page / ppb;
	if (size % blocksize == 0)
		blk_cnt = size / blocksize;
	else
		blk_cnt = size / blocksize + 1;
	block = blk_start - 1;

	while(pagecopy_cnt * pagesize < size) {
		if (blk_start > block) {
			if(sfc_nand_block_isbad(blk_start, ppb, pagesize)) {
				printf("bad block = %d\n",blk_start);
				page += ppb;
				block = blk_start;
				blk_start++;
				continue;
			}
			block = blk_start;
		}
		sfc_read_page(page, (unsigned char *)dst, pagesize);
		dst += pagesize;
		page++;
		pagecopy_cnt++;
		blk_start = page / ppb;
	}

	return ;
}

static int get_column_cmdaddr_bytes(void)
{
	unsigned char buffer[50];
	unsigned int len;
	int page_size;
	int trytime = 0;
	column_cmdaddr_bytes = 3;
try:
	sfc_read_page(0, buffer, 50);
	page_size = buffer[SPL_TYPE_FLAG_LEN + 5] * 1024;
	trytime++;
	if((page_size > 0) && (page_size < 4096))
		;
	else if(trytime < 2) {
		column_cmdaddr_bytes = 4;
		goto try;
	}
	else {
		printf("failed to get pagesize from nand\n");
		return -1;
	}

	return 0;
}

static void sfc_nand_set_feature(void)
{
        unsigned char cmd[8];
        unsigned int x=0;
        /* disable write protect */
        unsigned int add;
        cmd[0]=0x1f;//set feature
        add=0xa0;

        sfc_send_cmd(&cmd[0],1,add,1,0,1,1);
        sfc_nand_write_data(&x,1);

	/* enable ECC */
        cmd[0]=0x1f;//get feature
        add=0xb0;
	if(SFC_QUAD_MODE == 1)
		x=0x11;
	else
		x=0x10;
        sfc_send_cmd(&cmd[0],1,add,1,0,1,1);
        sfc_nand_write_data(&x,1);

	get_column_cmdaddr_bytes();
}

static void sfc_init(void)
{
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
}
static void gpio_as_sfc(void)
{
	writel(0x3c << 26,GPIO_PXINTC(0));
	writel(0x3c << 26,GPIO_PXMSKC(0));
	writel(0x3c << 26,GPIO_PXPAT1S(0));
	writel(0x3c << 26,GPIO_PXPAT0C(0));
}

#ifdef CONFIG_SPL_OS_BOOT
void spl_load_kernel(struct image_header *header, long offset)
{
	sfc_nand_load(offset, sizeof(struct image_header), CONFIG_SYS_TEXT_BASE);
	spl_parse_image_header(header);
	sfc_nand_load(offset, spl_image.size, spl_image.load_addr);
}
#endif

void spl_load_uboot(struct image_header *header)
{
	spl_parse_image_header(header);
	sfc_nand_load(CONFIG_UBOOT_OFFSET, CONFIG_SYS_MONITOR_LEN, (void *)CONFIG_SYS_TEXT_BASE);
}

static void *spl_get_nvinfo(void)
{
	nvinfo_t *nvinfo = (nvinfo_t *)CONFIG_SPL_NV_BASE;
	int i = 0;
	int erasesize = 0;

	int pagesize = 0;
	int ppb = 0;
	get_sfcnand_base_param(&pagesize, &ppb);

	erasesize = pagesize * ppb;

	for (i = 0; i < 2; i++) {
		sfc_nand_load(NV_AREA_BASE_ADDR + i * erasesize, sizeof(nvinfo_t), nvinfo);
		if (*(int *)nvinfo->start_magic == 0x41544f && *(int *)nvinfo->end_magic == 0x41544f) {
			debug("nvinfo->update_flag=%d, nvinfo->update_process: %d.\n",
				  nvinfo->update_flag, nvinfo->update_process);
			break;
		}
	}

	if (i == 0 || i == 1) {
		simple_puts("nv is in #");
		simple_put_dec(i);
		return nvinfo;
	} else {
		return NULL;
	}
}

char *spl_sfc_nand_load_image(void)
{
	char *cmdargs = NULL;
	struct image_header *header;
	char cmd[5] = {0};
	char idcode[5] = {0};
	unsigned int i=0;
	header = (struct image_header *)(CONFIG_SYS_TEXT_BASE);
#ifdef CONFIG_SPL_OS_BOOT
	nvinfo_t *nvinfo = NULL;
#endif

	sfc_init();
	sfc_nand_set_feature();
#ifdef CONFIG_SPL_OS_BOOT
	nvinfo = spl_get_nvinfo();
	if (!nvinfo) {
		/* if nv not valid, force to nonupdate status */
		nvinfo->update_flag = FLAG_NONUPDATE;
	}
	if (nvinfo->update_flag == FLAG_NONUPDATE) {
		spl_load_kernel(header, CONFIG_SPL_OS_OFFSET);
		cmdargs = CONFIG_SYS_SPL_ARGS_ADDR;
	} else if (nvinfo->update_flag == FLAG_UPDATE) {
		spl_load_kernel(header, CONFIG_SPL_OTA_OS_OFFSET);
		cmdargs = CONFIG_SYS_SPL_OTA_ARGS_ADDR;
	} else {
		debug("Invalid update flag: %d.\n", nvinfo->update_flag);
	}
#else
	spl_load_uboot(header);
#endif
	return cmdargs;
}

