/*
 * include/linux/power/ricoh61x_battery_init.h
 *
 * Battery initial parameter for RICOH R5T618/619 power management chip.
 *
 * Copyright (C) 2012-2013 RICOH COMPANY,LTD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef __LINUX_POWER_RICOH618_BATTERY_INIT_H
#define __LINUX_POWER_RICOH618_BATTERY_INIT_H


#ifdef CONFIG_LARGE_CAPACITY_BATTERY
/*for 3900mA*/
uint8_t battery_init_para[][32] = {
	{
		0x0A, 0x06, 0x0B, 0xB0, 0x0B, 0xDE, 0x0B, 0xFA,
		0x0C, 0x10, 0x0C, 0x30, 0x0C, 0x5F, 0x0C, 0x9B,
		0x0C, 0xF4, 0x0D, 0x4A, 0x0D, 0xD2, 0x0F, 0x6E,
		0x00, 0x2E, 0x0F, 0xC8, 0x05, 0x2C, 0x22, 0x56
	}
};
#endif
uint8_t impe_init_para[][20] = {
	{
		0x00, 0x72, 0x00, 0x62, 0x00, 0x64, 0x00, 0x63, 0x00, 0x63,
		0x00, 0x61, 0x00, 0x64, 0x00, 0x6C, 0x00, 0x66, 0x00, 0x64
	}
};

#endif

/*
<Other Parameter>
nominal_capacity=3500
cut-off_v=3450
thermistor_b=3435
board_impe=0
bat_impe=0.2430
load_c=707
available_c=3292
battery_v=3642
MakingMode=Normal
ChargeV=4.20V
LoadMode=Resistor
 */
