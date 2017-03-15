#ifndef __BOARD_H__
#define __BOARD_H__
#include <gpio.h>
#include <soc/gpio.h>


#ifdef  CONFIG_BROADCOM_RFKILL
#define BLUETOOTH_UART_GPIO_PORT        GPIO_PORT_D
#define BLUETOOTH_UART_GPIO_FUNC        GPIO_FUNC_2
#define BLUETOOTH_UART_FUNC_SHIFT       0x4
#define HOST_WAKE_BT    GPIO_PG(6)
#define BT_WAKE_HOST    GPIO_PF(7)
#define BT_REG_EN       GPIO_PG(9)
#define BT_UART_RTS     GPIO_PD(29)
#define BT_PWR_EN   -1
#define HOST_BT_RST    -1
#define GPIO_PB_FLGREG      (0x10010158)
#define GPIO_BT_INT_BIT     (1 << (GPIO_BT_INT % 32))
#define BLUETOOTH_UPORT_NAME  "ttyS1"
#endif

/* MSC GPIO Definition */
#define GPIO_SD0_CD_N       GPIO_PC(21)

/*wifi*/
#define GPIO_WLAN_PW_EN     -1//GPIO_PD(24)
#define WL_WAKE_HOST        GPIO_PG(7)
#define WL_REG_EN       GPIO_PG(8)

#ifdef CONFIG_SPI_GPIO
#define GPIO_SPI_SCK  GPIO_PA(26)
#define GPIO_SPI_MOSI GPIO_PA(29)
#define GPIO_SPI_MISO GPIO_PA(28)
#endif

/* ****************************GPIO KEY START******************************** */
#define GPIO_POWERDOWN 		GPIO_PB(31)
#define GPIO_BOOT_SEL0		GPIO_PB(28)
#define GPIO_BOOT_SEL1		GPIO_PB(29)
#define ACTIVE_LOW_POWERDOWN 	1
#define ACTIVE_LOW_F4 		1
#define ACTIVE_LOW_F5 		1
/* ****************************GPIO KEY END******************************** */

/* ****************************GPIO USB START******************************** */
#define GPIO_USB_ID             GPIO_PD(2)/*GPIO_PD(2)*/
#define GPIO_USB_ID_LEVEL       LOW_ENABLE
#define GPIO_USB_DETE           GPIO_PB(8) /*GPIO_PB(8)*/
#define GPIO_USB_DETE_LEVEL     LOW_ENABLE
#define GPIO_USB_DRVVBUS        GPIO_PB(25)
#define GPIO_USB_DRVVBUS_LEVEL      HIGH_ENABLE
/* ****************************GPIO USB END********************************** */

/* ****************************GPIO AUDIO START****************************** */
#define GPIO_HP_MUTE        -1  /*hp mute gpio*/
#define GPIO_HP_MUTE_LEVEL  -1  /*vaild level*/

#define GPIO_SPEAKER_EN    	GPIO_PC(25)         /*speaker enable gpio*/
#define GPIO_SPEAKER_EN_LEVEL   1
#define GPIO_AMP_POWER_EN	-1	/* amp power enable pin */
#define GPIO_AMP_POWER_EN_LEVEL	-1

#define GPIO_HANDSET_EN     	-1  /*handset enable gpio*/
#define GPIO_HANDSET_EN_LEVEL   -1

#define GPIO_HP_DETECT  	-1      /*hp detect gpio*/
#define GPIO_HP_INSERT_LEVEL    -1
#define GPIO_LINEIN_DETECT          GPIO_PD(3)      /*linein detect gpio*/
#define GPIO_LINEIN_INSERT_LEVEL    1
#define GPIO_MIC_SELECT     	-1  /*mic select gpio*/
#define GPIO_BUILDIN_MIC_LEVEL  -1  /*builin mic select level*/
#define GPIO_MIC_DETECT     	-1
#define GPIO_MIC_INSERT_LEVEL   -1
#define GPIO_MIC_DETECT_EN  	-1  /*mic detect enable gpio*/
#define GPIO_MIC_DETECT_EN_LEVEL -1 /*mic detect enable gpio*/

#define HOOK_ACTIVE_LEVEL       -1

#ifdef CONFIG_TAS5010_EXTERNAL_CODEC
#define GPIO_TAS5010_SPEAKER_EN    	GPIO_PC(25)         /*speaker enable gpio*/
#define GPIO_TAS5010_SPEAKER_EN_LEVEL   1
#endif

/* There is no AKM4345 on canna board, you should modify the code on your board */
#ifdef CONFIG_AKM4345_EXTERNAL_CODEC
#define GPIO_AKM4345_PDN	-1
#define GPIO_AKM4345_CSN	-1
#define GPIO_AKM4345_CCLK	-1
#define GPIO_AKM4345_CDTI	-1
#define GPIO_AKM4345_CDTO	-1
#endif

/* There is no NPCA110P on canna board, you should modify the code on your board */
#ifdef CONFIG_SND_ASOC_JZ_EXTCODEC_NPCA110P
#define GPIO_NPCA110P_POWER	-1		 /* MPCA110P POWER ON pin */
#define GPIO_NPCA110P_RESET	GPIO_PC(22)	 /* NPCA110P RESET pin */
#endif
/* ****************************GPIO AUDIO END******************************** */


//#define GPIO_EFUSE_VDDQ			GPIO_PC(27)	/* EFUSE must be -ENODEV or a gpio */

//#ifdef CONFIG_I2C_GPIO

//#ifdef CONFIG_SOFT_I2C0_GPIO_V12_JZ
//#define GPIO_I2C0_SDA		-1
//#define GPIO_I2C0_SCK		-1
//#endif

//#ifdef CONFIG_SOFT_I2C1_GPIO_V12_JZ
//#define GPIO_I2C1_SDA		-1
//#define GPIO_I2C1_SCK		-1
//#endif

//#endif /* CONFIG_I2C_GPIO */

#endif /* __BOARD_H__ */