#ifndef AK4425_4104_CODEC_V01_H
#define AK4425_4104_CODEC_V01_H

/******************************AK4425*********************************/
//IO BUS
#define AK4425_GPIO_CSN       (GPIO_PA(16))//PA(16)//PA(19)
#define AK4425_GPIO_CLK       (GPIO_PB(23))
#define AK4425_GPIO_CDTI      (GPIO_PB(24))
// Register
//0x00~0x04




/******************************AK4104*********************************/
//IO BUS
#define AK4104_GPIO_CSN       (GPIO_PA(11))
#define AK4104_GPIO_CLK       (GPIO_PA(8))
#define AK4104_GPIO_CDTI      (GPIO_PA(9))
#define AK4104_GPIO_CDTO      (GPIO_PA(10))//use in RW mode
#define AK4104_GPIO_PDN       (GPIO_PA(12))
// Register
//0x00~0x09


typedef enum _CODEC_MCLK_MODE
{
    MCLK_MODE_CODEC=0,
	MCLK_MODE_DMIC,
}CODEC_MCLK_MODE;
extern struct i2s_device *get_g_i2s_device(void);
extern int dm6291_switch_mclk(CODEC_MCLK_MODE mode,unsigned long rate);

#endif
