#ifndef __PMU_H__
#define __PMU_H__
#ifdef CONFIG_REGULATOR_RICOH618

#define PMU_I2C_BUSNUM  0

/* ****************************PMU DC/LDO NAME******************************* */
#define DC1_NAME "cpu_core"
#define DC1_SLP_NAME "cpu_core_slp"
#define DC2_NAME "cpu_vmema"
#define DC3_NAME "cpu_mem12"
#define LDO1_NAME "cpu_vdll1"
#define LDO2_NAME "cpu_2v5"
#define LDO3_NAME "v33"
#define LDO4_NAME "wifi_vddio_1v8"
#define LDO5_NAME "lcd_1v8"
#define LDORTC1_NAME "rtc_1v8"
#define LDORTC2_NAME "rtc_1v1"
/* ****************************PMU DC/LDO NAME END*************************** */

/* ****************************PMU DC/LDO DEFAULT V************************** */
#define DC1_INIT_UV     1250
#define DC2_INIT_UV     1800
#define DC3_INIT_UV     3300
#define LDO1_INIT_UV    2500
#define LDO2_INIT_UV    3300
#define LDO3_INIT_UV    3300
#define LDO4_INIT_UV    3300
#define LDO5_INIT_UV    3300
#define LDORTC1_INIT_UV 3300
#define LDORTC2_INIT_UV 1100
/* ****************************PMU DC/LDO DEFAULT V END********************** */

/* ****************************PMU DC/LDO ALWAYS ON************************** */
#define DC1_ALWAYS_ON     1
#define DC2_ALWAYS_ON     1
#define DC3_ALWAYS_ON     1
#define LDO1_ALWAYS_ON    1
#define LDO2_ALWAYS_ON    1
#define LDO3_ALWAYS_ON    1
#define LDO4_ALWAYS_ON    1
#define LDO5_ALWAYS_ON    0
#define LDORTC1_ALWAYS_ON 1
#define LDORTC2_ALWAYS_ON 1
/* ****************************PMU DC/LDO ALWAYS ON END********************** */

/* ****************************PMU DC/LDO BOOT ON**************************** */
#define DC1_BOOT_ON     1
#define DC2_BOOT_ON     1
#define DC3_BOOT_ON     1
#define LDO1_BOOT_ON    1
#define LDO2_BOOT_ON    1
#define LDO3_BOOT_ON    1
#define LDO4_BOOT_ON    1
#define LDO5_BOOT_ON    0
#define LDORTC1_BOOT_ON 1
#define LDORTC2_BOOT_ON 1
/* ****************************PMU DC/LDO BOOT ON END************************ */

/* ****************************PMU DC/LDO INIT ENABLE************************ */
#define DC1_INIT_ENABLE     DC1_BOOT_ON
#define DC2_INIT_ENABLE     DC2_BOOT_ON
#define DC3_INIT_ENABLE     DC3_BOOT_ON
#define LDO1_INIT_ENABLE    LDO1_BOOT_ON
#define LDO2_INIT_ENABLE    LDO2_BOOT_ON
#define LDO3_INIT_ENABLE    LDO3_BOOT_ON
#define LDO4_INIT_ENABLE    LDO4_BOOT_ON
#define LDO5_INIT_ENABLE    LDO5_BOOT_ON
#define LDORTC1_INIT_ENABLE LDORTC1_BOOT_ON
#define LDORTC2_INIT_ENABLE LDORTC2_BOOT_ON
/* ****************************PMU DC/LDO INIT ENABLE END******************** */
#endif	/* CONFIG_REGULATOR_RICOH618 */
#endif
