/*
 * cmd_nand_zm.c
 *
 * NAND cmd for which nand support the way of zone manager;
 *
 * Copyright (c) 2005-2008 Ingenic Semiconductor Inc.
 *
 */
#include <common.h>
#include <command.h>
#include <nand.h>
#define X_COMMAND_LENGTH 128

extern nand_info_t nand_info[CONFIG_SYS_MAX_NAND_DEVICE];
static unsigned int bad_len = 0;

static int sfc_nand_read_skip_bad(unsigned int addr, char *buffer, unsigned int len)
{
	unsigned int offset;
	nand_info_t *nand;
	nand = &nand_info[0];
	unsigned int block_size = nand->erasesize;

	offset = addr + bad_len;

	while (nand_block_isbad(nand, offset)) {
		printf("!!!!!!Skip bad block 0x%lx\n", offset);
		bad_len += block_size;
		offset += block_size;
	}

	nand_read(nand, offset, &len, buffer);

	return offset;
}



int do_sfcnand(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	char *cmd;
	unsigned int dst_addr,offset,len;
	char command[X_COMMAND_LENGTH];
	int ret;
	nand_info_t *nand;
	nand = &nand_info[0];
	unsigned int block_size = nand->erasesize;

	cmd = argv[1];

	if(argc != 5)
	{
		printf("ERROR: argv error,please check the param of cmd !!!\n");
		return CMD_RET_USAGE;
	}

	offset = (unsigned int)simple_strtoul(argv[2], NULL, 16);
	len = (unsigned int)simple_strtoul(argv[3], NULL, 16);
	dst_addr = (unsigned int)simple_strtoul(argv[4], NULL, 16);

	while(len) {

		sfc_nand_read_skip_bad(offset, dst_addr, block_size);
		offset += block_size;
		dst_addr += block_size;
		len -= block_size;

	}
	return CMD_RET_SUCCESS;
}
U_BOOT_CMD(sfcnand, 5, 1, do_sfcnand,
		"sfcnand    - SFC_NAND sub-system\n",
		"sfcnand read from(offs) size dst_addr\n"
		);
void sfc_nand_init(void)
{
	struct nand_chip *chip;
	struct mtd_info *mtd;
	mtd = &nand_info[0];

	jz_sfc_nand_init(0,NULL);

	chip =mtd->priv;
	chip->scan_bbt(mtd);
	chip->options |= NAND_BBT_SCANNED;
}
