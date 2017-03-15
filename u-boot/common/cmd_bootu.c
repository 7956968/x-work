/*
 *Ingenic mensa boot android system command
 *
 * Copyright (c) 2013 Imagination Technologies
 * Author: Martin <czhu@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <stdarg.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <errno.h>
#include <div64.h>
#include <common.h>
#include <command.h>
#include <config.h>
#include <mmc.h>
#include <boot_img.h>
#include <fs.h>
#include <fat.h>
#ifdef CONFIG_MTD_SFCNAND
#include <nand.h>
#endif

#include <asm/nvrw_interface.h>

extern void *sfc_nor_get_nvinfo(void);
extern int run_command_list(const char *cmd, int len, int flag);

static int do_bootu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_JS1_BOARD
	nvinfo_t *nvinfo = NULL;
	nvinfo = sfc_nor_get_nvinfo();
	if (!nvinfo || nvinfo->update_flag == FLAG_NONUPDATE) {
		printf("normal boot...\n");
		run_command_list(CONFIG_NORMAL_BOOTCOMMAND, -1, 0);
	} else {
		printf("updating boot...\n");
		run_command_list(CONFIG_UPDATE_BOOTCOMMAND, -1, 0);
	}
#endif
	return 0;
}

#ifdef CONFIG_SYS_LONGHELP
static char bootu_help_text[] =
        "";
#endif

U_BOOT_CMD(
        bootu, 5, 1, do_bootu,
        "wrap bootx",bootu_help_text
);
