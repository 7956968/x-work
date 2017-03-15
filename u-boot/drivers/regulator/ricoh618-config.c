#include <config.h>
#include <common.h>
#include <power/ricoh618.h>

/*for 4000mA*/
uint8_t battery_init_para[][32] = {
	{
		0x0A, 0x06, 0x0B, 0xB0, 0x0B, 0xDE, 0x0B, 0xFA,
		0x0C, 0x10, 0x0C, 0x30, 0x0C, 0x5F, 0x0C, 0x9B,
		0x0C, 0xF4, 0x0D, 0x4A, 0x0D, 0xD2, 0x0F, 0x6E,
		0x00, 0x2E, 0x0F, 0xC8, 0x05, 0x2C, 0x22, 0x56
	}
};

struct ricoh618_battery_platform_data pdata = {
	.alarm_vol_mv 	= 3300,
	.multiple	= 100,
	.monitor_time 	= 60,

	// Battery Independent Parameter
	// Battery Type For 260mA 500mA
	.type[0] = {
		.ch_vfchg 	= 0x04,	// VFCHG	= 0 - 4 (4.05v, 4.10v, 4.15v, 4.20v, 4.35v)
		.ch_vrchg 	= 0x04,	// VRCHG	= 0 - 4 (3.85v, 3.90v, 3.95v, 4.00v, 4.10v)
		.ch_vbatovset 	= 0x0,	// VBATOVSET	= 0 or 1 (0 : 4.38v(up)/3.95v(down) 1: 4.53v(up)/4.10v(down))
		.ch_ichg 	= 0x10,	// ICHG		= 0 - 0x1D (100mA - 3000mA)
		.ch_ilim_adp 	= 0x18,	// ILIM_ADP	= 0 - 0x1D (100mA - 3000mA)
		.ch_ilim_usb 	= 0x04,	// ILIM_USB	= 0 - 0x1D (100mA - 3000mA)
		.ch_icchg 	= 0x03,	// ICCHG	= 0 - 3 (50mA 100mA 150mA 200mA)
		.fg_target_vsys = 3000,	// This value is the target one to DSOC=0%
		.fg_target_ibat = 200,  // This value is the target one to DSOC=0%
		.fg_poff_vbat 	= 0, 	// setting value of 0 per Vbat
		.fg_rsense_val	= 20,  // setting value of R Sense
		.jt_en 		= 0,	// JEITA Enable	  = 0 or 1 (1:enable, 0:disable)
		.jt_hw_sw 	= 1,	// JEITA HW or SW = 0 or 1 (1:HardWare, 0:SoftWare)
		.jt_temp_h 	= 50,	// degree C
		.jt_temp_l 	= 12,	// degree C
		.jt_vfchg_h 	= 0x04,	// VFCHG High  	= 0 - 4 (4.05v, 4.10v, 4.15v, 4.20v, 4.35v)
		.jt_vfchg_l 	= 0x0,	// VFCHG High  	= 0 - 4 (4.05v, 4.10v, 4.15v, 4.20v, 4.35v)
		.jt_ichg_h 	= 0x09,	// ICHG Hi   	= 0 - 0x1D (100mA - 3000mA)
		.jt_ichg_l 	= 0x01,	// ICHG Low   	= 0 - 0x1D (100mA - 3000mA)
	},

	.type[1] = {
		.ch_vfchg 	= 0x04,	// VFCHG	= 0 - 4 (4.05v, 4.10v, 4.15v, 4.20v, 4.35v)
		.ch_vrchg 	= 0x04,	// VRCHG	= 0 - 4 (3.85v, 3.90v, 3.95v, 4.00v, 4.10v)
		.ch_vbatovset 	= 0x01,	// VBATOVSET	= 0 or 1 (0 : 4.38v(up)/3.95v(down) 1: 4.53v(up)/4.10v(down))
		.ch_ichg 	= 0x09,	// ICHG		= 0 - 0x1D (100mA - 3000mA)
		.ch_ilim_adp 	= 0x0e,	// ILIM_ADP	= 0 - 0x1D (100mA - 3000mA)
		.ch_ilim_usb 	= 0x0e,	// ILIM_USB	= 0 - 0x1D (100mA - 3000mA)
		.ch_icchg 	= 0x01,	// ICCHG	= 0 - 3 (50mA 100mA 150mA 200mA)
		.fg_target_vsys = 3000,	// This value is the target one to DSOC=0%
		.fg_target_ibat = 100,  // This value is the target one to DSOC=0%
		.fg_poff_vbat 	= 0, 	// setting value of 0 per Vbat
		.fg_rsense_val	= 100,  // setting value of R Sense
		.jt_en 		= 0,	// JEITA Enable	  = 0 or 1 (1:enable, 0:disable)
		.jt_hw_sw 	= 1,	// JEITA HW or SW = 0 or 1 (1:HardWare, 0:SoftWare)
		.jt_temp_h 	= 50,	// degree C
		.jt_temp_l 	= 12,	// degree C
		.jt_vfchg_h 	= 0x04,	// VFCHG High  	= 0 - 4 (4.05v, 4.10v, 4.15v, 4.20v, 4.35v)
		.jt_vfchg_l 	= 0x02,	// VFCHG High  	= 0 - 4 (4.05v, 4.10v, 4.15v, 4.20v, 4.35v)
		.jt_ichg_h 	= 0x09,	// ICHG Hi   	= 0 - 0x1D (100mA - 3000mA)
		.jt_ichg_l 	= 0x01,	// ICHG Low   	= 0 - 0x1D (100mA - 3000mA)
	},

	.type[2] = {
		.ch_vfchg 	= 0x03,	// VFCHG	= 0 - 4 (4.05v, 4.10v, 4.15v, 4.20v, 4.35v)
		.ch_vrchg 	= 0x01,	// VRCHG	= 0 - 4 (3.85v, 3.90v, 3.95v, 4.00v, 4.10v)
		.ch_vbatovset 	= 0x0,	// VBATOVSET	= 0 or 1 (0 : 4.38v(up)/3.95v(down) 1: 4.53v(up)/4.10v(down))
		.ch_ichg 	= 0x0d,	// ICHG		= 0 - 0x1D (100mA - 3000mA)
		.ch_ilim_adp 	= 0x0e,	// ILIM_ADP	= 0 - 0x1D (100mA - 3000mA)
		.ch_ilim_usb 	= 0x0e,	// ILIM_USB	= 0 - 0x1D (100mA - 3000mA)
		.ch_icchg 	= 0x02,	// ICCHG	= 0 - 3 (50mA 100mA 150mA 200mA)
		.fg_target_vsys = 3400,	// This value is the target one to DSOC=0%
		.fg_target_ibat = 100,  // This value is the target one to DSOC=0%
		.fg_poff_vbat 	= 0, 	// setting value of 0 per Vbat
		.fg_rsense_val	= 20,   // setting value of R Sense
		.jt_en 		= 0,	// JEITA Enable	  = 0 or 1 (1:enable, 0:disable)
		.jt_hw_sw 	= 1,	// JEITA HW or SW = 0 or 1 (1:HardWare, 0:SoftWare)
		.jt_temp_h 	= 50,	// degree C
		.jt_temp_l 	= 12,	// degree C
		.jt_vfchg_h 	= 0x03,	// VFCHG High  	= 0 - 4 (4.05v, 4.10v, 4.15v, 4.20v, 4.35v)
		.jt_vfchg_l 	= 0x0,	// VFCHG High  	= 0 - 4 (4.05v, 4.10v, 4.15v, 4.20v, 4.35v)
		.jt_ichg_h 	= 0x0d,	// ICHG Hi   	= 0 - 0x1D (100mA - 3000mA)
		.jt_ichg_l 	= 0x01,	// ICHG Low   	= 0 - 0x1D (100mA - 3000mA)
	},

};

#ifndef CONFIG_RICOH618_BATTERY_TYPE
#define CONFIG_RICOH618_BATTERY_TYPE 0
#endif

void ricoh618_preinit()
{
	ricoh618_battery_init(&pdata.type[CONFIG_RICOH618_BATTERY_TYPE], battery_init_para[0]);
}
