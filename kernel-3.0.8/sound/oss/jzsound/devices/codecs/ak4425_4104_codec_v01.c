/*
 * Linux/sound/oss/jzsound/devices/codecs/ak4425_4401_codec_v01.c
 *
 * CODEC driver for ak4425_4401 i2s external codec
 *
 * 2015-10-xx   tjin <tao.jin@ingenic.com>
 *
 * Copyright (c) Ingenic Semiconductor Co., Ltd.
 */
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/sound.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/initval.h>
#include <linux/proc_fs.h>
#include <linux/soundcard.h>
#include <linux/dma-mapping.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <mach/jzsnd.h>

#include <linux/clk.h>
#include "../xb47xx_i2s_v13.h"
#include "ak4425_4104_codec_v01.h"//SPI_BUS 

#define DEFAULT_REPLAY_SAMPLE_RATE   48000
#define AK4425_4401_EXTERNAL_CODEC_CLOCK 12000000
#define CODEC_MODE  CODEC_SLAVE
//#define CODEC_MODE  CODEC_MASTER

static int user_replay_volume = 100;
static unsigned long user_replay_rate = DEFAULT_REPLAY_SAMPLE_RATE;
static struct snd_codec_data *codec_platform_data = NULL;

extern int i2s_register_codec(char*, void *,unsigned long,enum codec_mode);



/*********************GPIO COMMUNICATE***************/
void AK4425_write_data(unsigned char addr, unsigned char data)
{
        unsigned int delay = 50;
        int i = 0;
		unsigned short writedata = 0x0;//printk("addr=%d,data=%d\n",addr,data);
       //                 0110
       writedata = (0x6000)|((addr&0x1f)<<8)|(data&0xff);

       //init
        gpio_set_value(AK4425_GPIO_CSN, 1); // CSN disable
        gpio_set_value(AK4425_GPIO_CLK, 1);  //CLK high
        gpio_set_value(AK4425_GPIO_CDTI, 1);
		udelay(delay);
		
        gpio_set_value(AK4425_GPIO_CSN, 0); // CSN enable
        for (i = 15; i >= 0; i--)
       {
		   gpio_set_value(AK4425_GPIO_CLK, 0);//CLK low           
           if ((writedata >> i) & 0x1)//write high bit first
               gpio_set_value(AK4425_GPIO_CDTI, 1);
           else
               gpio_set_value(AK4425_GPIO_CDTI, 0);

           udelay(delay);
           gpio_set_value(AK4425_GPIO_CLK, 1);  //CLK high
           udelay(delay);

        }
		//reset
        gpio_set_value(AK4425_GPIO_CLK, 1);  //CLK high
        udelay(delay);/**/
		gpio_set_value(AK4425_GPIO_CSN, 1); // CSN disable
		gpio_set_value(AK4425_GPIO_CDTI, 1);
}



void AK4104_write_data(unsigned char addr, unsigned char data)
{
        unsigned int delay = 50;
        int i = 0;
		unsigned short writedata = 0x0;
       //                 1110
       writedata = (0xE000)|((addr&0x1f)<<8)|(data&0xff);

       //init
       gpio_set_value(AK4104_GPIO_CSN, 1); // CSN disable
       gpio_set_value(AK4104_GPIO_CLK, 1);  //CLK high
       gpio_set_value(AK4104_GPIO_CDTI, 1);
       udelay(delay);
		
       gpio_set_value(AK4104_GPIO_CSN, 0); // CSN enable
       for (i = 15; i >= 0; i--)
       {
           gpio_set_value(AK4104_GPIO_CLK, 0);//CLK low

           if ((writedata >> i) & 0x1)//write high bit first
               gpio_set_value(AK4104_GPIO_CDTI, 1);
           else
               gpio_set_value(AK4104_GPIO_CDTI, 0);

           udelay(delay);
           gpio_set_value(AK4104_GPIO_CLK, 1);  //CLK high
           udelay(delay);
       }
		//reset/**/
        gpio_set_value(AK4104_GPIO_CSN, 1); // CSN disable
        gpio_set_value(AK4104_GPIO_CLK, 1);  //CLK high
        udelay(delay);
		gpio_set_value(AK4104_GPIO_CDTI, 1);
}


//must in 4-wired mode
unsigned char AK4104_read_data(unsigned char addr)
{
       unsigned int delay = 50; //CLK延时，依具体而定
       int i = 0,read_bit = 0x0;
       unsigned char writedata = 0x0;
	   unsigned char read_data = 0x0;
 
       //                 1100
       writedata = (0xC0)|((addr&0x1f));

       //init
       gpio_set_value(AK4104_GPIO_CSN, 1); // CSN disable
       gpio_set_value(AK4104_GPIO_CLK, 1);  //CLK high
       gpio_set_value(AK4104_GPIO_CDTI, 1);
       udelay(delay);

       gpio_set_value(AK4104_GPIO_CSN, 0); // CSN enable
       for (i = 7; i >= 0; i--)
       {
           gpio_set_value(AK4104_GPIO_CLK, 0);//CLK low

           if ((writedata >> i) & 0x1)//write high bit first
               gpio_set_value(AK4104_GPIO_CDTI, 1);
           else
               gpio_set_value(AK4104_GPIO_CDTI, 0);

           udelay(delay);
           gpio_set_value(AK4104_GPIO_CLK, 1);  //CLK high
           udelay(delay);
        }

       for (i = 7; i >= 0; i--)
       {
           gpio_set_value(AK4104_GPIO_CLK, 0);//CLK low
           udelay(delay);
           gpio_set_value(AK4104_GPIO_CLK, 1);  //CLK high
  
           read_bit = gpio_get_value(AK4104_GPIO_CDTO); //read data
           if(0!=read_bit)
           {
	           read_data |= (1<<i); //save value
           }
           udelay(delay);
        }

        return(read_data);    //return read byte
}

/*******************************************************/


/* Below is ak4425 registers value from 0x00 to 0x04 */
unsigned char ak4425_registers[][2] = {
	{ 0x00 ,0x8f },//sample rate auto setting mode
	//{ 0x00 ,0x8f },//sample rate auto setting mode
	//{ 0x00 ,0x8f },//sample rate auto setting mode
	{ 0x01 ,0x02 },
	{ 0x02 ,0x00 },
	{ 0x03 ,0x64 },//volume 
	{ 0x04 ,0x64 },/**/
	{ 0x00 ,0x8f },//sample rate auto setting mode
	//{ 0x00 ,0x8f },//sample rate auto setting mode
};



/* Below is ak4104 registers value from 0x00 to 0x04 */
unsigned char ak4104_registers[][2] = {
	{ 0x00 ,0x8f },//mode3
	//{ 0x01 ,0x04 },//reserved
	{ 0x02 ,0x04 },//use 3-wired, CDT1
	{ 0x03 ,0x81 },
	{ 0x04 ,0x04 },
	{ 0x05 ,0x00 },
	{ 0x06 ,0x00 },
	{ 0x07 ,0x00 },
	{ 0x08 ,0x00 },
	{ 0x09 ,0xfc }
};


static void gpio_enable_spk_en(void)
{
		printk("gpio_enable_spk_en\n");
	if(codec_platform_data && (codec_platform_data->gpio_spk_en.gpio != -1)) {
		if (codec_platform_data->gpio_spk_en.active_level) {
			gpio_direction_output(codec_platform_data->gpio_spk_en.gpio , 1);
			printk("set 1\n");
		} else {
			gpio_direction_output(codec_platform_data->gpio_spk_en.gpio , 0);
		}
	}
}

static void gpio_disable_spk_en(void)
{
		printk("gpio_disable_spk_en\n");
	if(codec_platform_data && (codec_platform_data->gpio_spk_en.gpio != -1)) {
		if (codec_platform_data->gpio_spk_en.active_level) {
			gpio_direction_output(codec_platform_data->gpio_spk_en.gpio , 0);
			printk("set 0\n");
		} else {
			gpio_direction_output(codec_platform_data->gpio_spk_en.gpio , 1);
		}
	}
}

static void codec_set_mute(void)
{
	AK4425_write_data(0x01, 0x03);
	//gpio_direction_output(AK4104_GPIO_PDN, 0);//4104 power down  !!!!!don't need reset,or can listen delay
	mdelay(5);
}

static void codec_set_unmute(void)
{
	AK4425_write_data(0x01, 0x02);
	gpio_direction_output(AK4104_GPIO_PDN, 1);//4104 power up
	mdelay(5);
}


static int codec_set_replay_rate(unsigned long *rate)
{
	printk("codec_set_replay_rate, %ld\n", *rate);
	codec_set_unmute();
	gpio_enable_spk_en();
  //a;dm6291_switch_mclk(MCLK_MODE_CODEC,*rate);
	user_replay_rate = *rate;
	return 0;
}

static int codec_set_replay_channel(int* channel)
{
	return 0;
}

static int codec_set_record_channel(int* channel)
{
	return 0;
}


static int dump_codec_regs(void)
{
	return 0;
}

static int codec_init(void)
{
	int ret = 0;
    int i = 0;
	int n = 0;
	
	/* Init ak4425 0x00 ~ 0x04 registers */
	n = sizeof(ak4425_registers) / sizeof(ak4425_registers[0]); //printk("nnnn=%d\n",n);
    for (i = 0; i < n; i++)
	{
        AK4425_write_data(ak4425_registers[i][0], ak4425_registers[i][1]);
		//printk("-------ak4425_registers[%d][0]=%d ak4425_registers[%d][1]=%d\n", i,ak4425_registers[i][0],i,ak4425_registers[i][1]);
		mdelay(5);
	}

	/* Init ak4425 0x00 ~ 0x09 registers */
	n = sizeof(ak4104_registers) / sizeof(ak4104_registers[0]); //printk("nntt=%d\n",n);
    for (i = 0; i < n; i++)
	{
        AK4104_write_data(ak4104_registers[i][0], ak4104_registers[i][1]);
		mdelay(5);
	}

	return ret;
}

static void ak4425_reInit(void)
{
	AK4425_write_data(0x00, 0x8f);
	mdelay(5);
    AK4425_write_data(0x01, 0x02);
	mdelay(5);
}
static int codec_set_replay_volume(int *val)
{
	unsigned char data = 0x0;
	int vol = 0;

	/************test***********/
		AK4425_write_data(0x00, 0x8f);
		mdelay(5);
/*		AK4425_write_data(0x03, 0xaa);
		mdelay(5);
		AK4425_write_data(0x04, 0xaa);
		mdelay(5);
		AK4104_write_data(0x00 ,0x8f);
		return;
	***********test end***********/
	/* get current volume */
	if (*val < 0) {
		*val = user_replay_volume;
		return *val;
	} else if (*val > 100)
		*val = 100;

	if (*val) {
		vol = ((*val)*180)/100;
		data = (unsigned char)vol;                       //printk("data1111:%d vol=%d: *val=%d\n", data,vol,*val);
		AK4425_write_data(0x01, 0x02);
		mdelay(5);
		AK4425_write_data(0x03, data);
		mdelay(5);
		AK4425_write_data(0x04, data);
	} else{
		/* Digital Volume mute */
		AK4425_write_data(0x01, 0x03);
		mdelay(5);//printk("data000:%d: \n", data);
		AK4425_write_data(0x03, 0x00);
		mdelay(5);
		AK4425_write_data(0x04, 0x00);
	}
	user_replay_volume = *val;
	return *val;
}

static int codec_suspend(void)
{
    AK4425_write_data(0x01, 0x03);
	mdelay(5);
	return 0;
}

static int codec_resume(void)
{
    ak4425_reInit();
	return 0;
}

static int codec_shutdown(void)
{
    AK4425_write_data(0x01, 0x03);
	mdelay(5);
	return 0;
}

static int jzcodec_ctl(unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	//printk(">>>>>>>>>>>>>>>>jzcodec_ctl cmd=%d\n",cmd);

	{
		switch (cmd) {
		case CODEC_INIT:
			codec_init();//printk(">>>>>>>>>>>>>>>>CODEC_INIT\n");
			break;

		case CODEC_TURN_OFF:
			//printk(">>>>>>>>>>>>>>>>CODEC_TURN_OFF set mute\n");
			codec_set_mute();
			break;

		case CODEC_SHUTDOWN:
			ret = codec_shutdown();
			break;

		case CODEC_RESET:
			break;

		case CODEC_SUSPEND:
			ret = codec_suspend();
			break;

		case CODEC_RESUME:
			ret = codec_resume();
			break;

		case CODEC_ANTI_POP:
			break;
		case CODEC_SET_DEFROUTE:

			break;

		case CODEC_SET_DEVICE:
			//ret = codec_set_device(*(enum snd_device_t *)arg);
			break;

			break;
			case CODEC_SET_STANDBY:

		case CODEC_SET_REPLAY_RATE:
			ret = codec_set_replay_rate((unsigned long*)arg);
			break;

		case CODEC_SET_RECORD_RATE:
			/*
			 * Record sample rate is follow the replay sample rate. Here set is invalid.
			 * Because record and replay i2s use the same BCLK and SYNC pin.
			*/
			break;

		case CODEC_SET_REPLAY_DATA_WIDTH:
			ak4425_reInit();
			break;

		case CODEC_SET_RECORD_DATA_WIDTH:
			break;

		case CODEC_SET_REPLAY_VOLUME:
			ret = codec_set_replay_volume((int*)arg);
			break;

		case CODEC_SET_RECORD_VOLUME:
			ret = *(int*)arg;
			break;

		case CODEC_SET_MIC_VOLUME:
			ret = *(int*)arg;
			break;

		case CODEC_SET_REPLAY_CHANNEL:
			ret = codec_set_replay_channel((int*)arg);
			break;

		case CODEC_SET_RECORD_CHANNEL:
			ret = codec_set_record_channel((int*)arg);
			break;

		case CODEC_GET_REPLAY_FMT_CAP:
			//codec_get_format_cap((unsigned long *)arg);
			break;

		case CODEC_GET_RECORD_FMT_CAP:
			*(unsigned long *)arg = 0;
			break;

		case CODEC_DAC_MUTE:
			//printk(">>>>>>>>>>>>>>>>CODEC_DAC_MUTE %d\n",(int)arg);
			if((int)arg == 0)
				ret = -1;
			else
			    ;//codec_set_mute();
			break;

		case CODEC_ADC_MUTE:
			break;

		case CODEC_DEBUG_ROUTINE:
			break;

		case CODEC_CLR_ROUTE:
			break;

		case CODEC_DEBUG:
			break;

		case CODEC_DUMP_REG:
			dump_codec_regs();
			break;
		default:
			printk("JZ CODEC:%s:%d: Unkown IOC commond\n", __FUNCTION__, __LINE__);
			ret = -1;
		}
	}

	return ret;
}

/********************JJJHHH**********************/
#if 1
static int g_mclk_mode = -1;
int dm6291_switch_mclk(CODEC_MCLK_MODE mode,unsigned long rate)
{
    struct i2s_device *i2s_dev = NULL;
	unsigned long sysclk = 0;
	int ret = 0;
	printk("dm6291_switch_mclk start!!!!! mode=%d  g_mclk_mode=%d\n",mode,g_mclk_mode);

	i2s_dev = get_g_i2s_device();
	/*sysclk output*/
	if(MCLK_MODE_CODEC == mode) 
	{//codec mode
	    if(MCLK_MODE_CODEC == g_mclk_mode)
		{
            printk("already in CODEC mode!\n");
		    return 0;
		}
		g_mclk_mode = MCLK_MODE_CODEC;
	    sysclk = rate * 256;
	    i2s_dev->cur_codec->codec_clk = sysclk;
		i2s_dev->cur_codec->replay_rate = rate;
		__i2s_stop_bitclk(i2s_dev);
		
	    if (i2s_dev->i2s_clk == NULL)
	    {
	        printk("JJJHHH ERROR!i2s_dev->i2s_clk == NULL\n");
	        return -1;
	    }
	    
	    ret = clk_set_rate(i2s_dev->i2s_clk, sysclk);
	    if(ret < 0) {
		    printk("JJJHHH ERROR:external codec set rate failed\n");
		    return -EINVAL;
	    }
	   	audio_write(0x3, I2SDIV_PRE);
		*(volatile unsigned int*)0xb0000070 = 0x0;
		__i2s_start_bitclk(i2s_dev);
	
		printk(">>>>>>>>dm6291_switch_mclk 12000000!!!!!\n");
	} 
	else if(MCLK_MODE_DMIC == mode) 
	{//dmic
    	if(MCLK_MODE_DMIC == g_mclk_mode)
		{
            printk("already in DMIC mode!\n");
		    return 0;
		}
		
		g_mclk_mode = MCLK_MODE_DMIC;
	    i2s_dev->cur_codec->codec_clk = 24000000;
		__i2s_stop_bitclk(i2s_dev);

		ret = clk_set_rate(i2s_dev->i2s_clk, 24000000);
		if(ret < 0) 
		{
			printk("JJJHHH dm6291_switch_mclk failed\n");
			return -EINVAL;
		}
		audio_write(0x3, I2SDIV_PRE);
		*(volatile unsigned int*)0xb0000070 = 0x0;
		__i2s_start_bitclk(i2s_dev);

		printk(">>>>>>>>dm6291_switch_mclk 24000000!!!!!\n");
	}

    printk("dm6291_switch_mclk successed!!!!!\n");
	return 0;
}
#endif
/********************JJJHHH**********************/

static int jz_codec_probe(struct platform_device *pdev)
{
	codec_platform_data = pdev->dev.platform_data;

	codec_platform_data->codec_sys_clk = AK4425_4401_EXTERNAL_CODEC_CLOCK;

	gpio_disable_spk_en();
	return 0;
}

static int __devexit jz_codec_remove(struct platform_device *pdev)
{
	codec_platform_data = NULL;

	return 0;
}

static struct platform_driver jz_codec_driver = {
        .probe          = jz_codec_probe,
        .remove         = __devexit_p(jz_codec_remove),
        .driver         = {
                .name   = "ak4425_4104_codec",
                .owner  = THIS_MODULE,
        },
};


static int init_gpio_bus(void)
{
/*AK4425*/
	if (gpio_request(AK4425_GPIO_CSN, "AK4425_GPIO_CSN") < 0) {
		gpio_free(AK4425_GPIO_CSN);
		gpio_request(AK4425_GPIO_CSN,"AK4425_GPIO_CSN");
	}
	gpio_direction_output(AK4425_GPIO_CSN, 1);//disable CSN
	
	if (gpio_request(AK4425_GPIO_CLK, "AK4425_GPIO_CLK") < 0) {
		gpio_free(AK4425_GPIO_CLK);
		gpio_request(AK4425_GPIO_CLK,"AK4425_GPIO_CLK");
	}
	gpio_direction_output(AK4425_GPIO_CLK, 1);
	
	if (gpio_request(AK4425_GPIO_CDTI, "AK4425_GPIO_CDTI") < 0) {
		gpio_free(AK4425_GPIO_CDTI);
		gpio_request(AK4425_GPIO_CDTI,"AK4425_GPIO_CDTI");
	}
	gpio_direction_output(AK4425_GPIO_CDTI, 1);

/*AK4104*/
	if (gpio_request(AK4104_GPIO_CSN, "AK4104_GPIO_CSN") < 0) {
		gpio_free(AK4104_GPIO_CSN);
		gpio_request(AK4104_GPIO_CSN,"AK4104_GPIO_CSN");
	}
	gpio_direction_output(AK4104_GPIO_CSN, 1);//disable CSN

	if (gpio_request(AK4104_GPIO_CLK, "AK4104_GPIO_CLK") < 0) {
		gpio_free(AK4104_GPIO_CLK);
		gpio_request(AK4104_GPIO_CLK,"AK4104_GPIO_CLK");
	}
	gpio_direction_output(AK4104_GPIO_CLK, 1);

	if (gpio_request(AK4104_GPIO_CDTI, "AK4104_GPIO_CDTI") < 0) {
		gpio_free(AK4104_GPIO_CDTI);
		gpio_request(AK4104_GPIO_CDTI,"AK4104_GPIO_CDTI");
	}
	gpio_direction_output(AK4104_GPIO_CDTI, 1);//use in RW mode

	if (gpio_request(AK4104_GPIO_CDTO, "AK4104_GPIO_CDTO") < 0) {
		gpio_free(AK4104_GPIO_CDTO);
		gpio_request(AK4104_GPIO_CDTO,"AK4104_GPIO_CDTO");
	}
	gpio_direction_input(AK4104_GPIO_CDTO);

	if (gpio_request(AK4104_GPIO_PDN, "AK4104_GPIO_PDN") < 0) {
		gpio_free(AK4104_GPIO_PDN);
		gpio_request(AK4104_GPIO_PDN,"AK4104_GPIO_PDN");
	}
	gpio_direction_output(AK4104_GPIO_PDN, 0);//reset
	mdelay(20);
	gpio_set_value(AK4104_GPIO_PDN, 1);//power up
    return 0;
}



/**
 * Module init
 */   
static int __init init_ak4425_4401_codec(void)
{
	int ret = 0;
	ret = i2s_register_codec("i2s_external_codec", (void *)jzcodec_ctl, AK4425_4401_EXTERNAL_CODEC_CLOCK, CODEC_MODE);
	if (ret < 0){
		printk("i2s audio is not support\n");
		return ret;
	}

        ret = platform_driver_register(&jz_codec_driver);
        if (ret) {
                printk("JZ CODEC: Could not register jz_codec_driver\n");
                return ret;
        }

	printk("audio codec ak4425_4401 register success\n");

	init_gpio_bus();

	return 0;
}

/**
 * Module exit
 */
static void __exit cleanup_ak4425_4401_codec(void)
{
	platform_driver_unregister(&jz_codec_driver);
}
module_init(init_ak4425_4401_codec);
module_exit(cleanup_ak4425_4401_codec);
MODULE_LICENSE("GPL");
