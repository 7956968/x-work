/*
 * akm4753.c  --  AKM4753 ALSA Soc Audio driver
 *
 * Copyright 2016 Ingenic Semiconductor Co.,Ltd
 *
 * Author: tjin <tao.jin@ingenic.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/tlv.h>
#include <mach/jzsnd.h>
#include <linux/gpio.h>
#include "akm4753.h"

//#define  USE_GAIN_FULL_RANGE

/* codec private data */
struct akm4753_priv {
	unsigned int sysclk;
	struct i2c_client * i2c_client;
	struct mutex i2c_access_lock;
	enum snd_soc_control_type control_type;
	struct akm4753_platform_data *control_pin;
}* akm4753;

static unsigned long user_replay_rate = 0;

//static const u8 akm4753_reg[] = { };

static int akm4753_i2c_write_regs(unsigned char reg, unsigned char* data, unsigned int len)
{
	int ret = -1;
	int i = 0;
	unsigned char buf[128] = {0};
	struct i2c_client *client = akm4753->i2c_client;

	if (client == NULL) {
		printk("===>NOTE: %s error\n", __func__);
		return ret;
	}

	if(!data || len <= 0){
		printk("===>ERROR: %s\n", __func__);
		return ret;
	}

	mutex_lock(&akm4753->i2c_access_lock);
	buf[0] = reg;
	for(; i < len; i++) {
		buf[i+1] = *data;
		data++;
	}

	ret = i2c_master_send(client, buf, len+1);
	if (ret < len+1)
		printk("%s 0x%02x err %d!\n", __func__, reg, ret);
	mutex_unlock(&akm4753->i2c_access_lock);

	return ret < len+1 ? ret : 0;
}

static int akm4753_i2c_read_reg(unsigned char reg, unsigned char* data, unsigned int len)
{
        int ret = -1;
        struct i2c_client *client = akm4753->i2c_client;

        if (client == NULL) {
                printk("===>NOTE: %s error\n", __func__);
                return ret;
        }

        if(!data || len <= 0){
                printk("===>ERROR: %s\n", __func__);
                return ret;
        }

        mutex_lock(&akm4753->i2c_access_lock);
        ret = i2c_master_send(client, &reg, 1);
        if (ret < 1) {
                printk("%s 0x%02x err\n", __func__, reg);
                mutex_unlock(&akm4753->i2c_access_lock);
                return -1;
        }

        ret = i2c_master_recv(client, data, len);
        if (ret < 0)
                printk("%s 0x%02x err\n", __func__, reg);
        mutex_unlock(&akm4753->i2c_access_lock);

	return ret < len ? ret : 0;
}

/*
 * read akm4753 register cache
 */
static inline unsigned int akm4753_read_reg_cache(struct snd_soc_codec *codec,
							unsigned int reg)
{
	u8 *cache = codec->reg_cache;

        if (reg >= AKM4753_REGNUM)
                return 0;

        return cache[reg];
}

/*
 * write akm4753 register cache
 */
static inline void akm4753_write_reg_cache(struct snd_soc_codec *codec,
	unsigned int reg, unsigned char value)
{
	u8 *cache = codec->reg_cache;

        if (reg >= AKM4753_REGNUM)
                return;

        cache[reg] = value;
	return;
}

/*
 * read akm4753 real register
 */
static inline unsigned int akm4753_i2c_read(struct snd_soc_codec *codec,
							unsigned int reg)
{
	int ret = -1;
	unsigned char data;
	unsigned char reg_addr = (unsigned char)reg;

        if (reg >= AKM4753_REGNUM) {
		printk("===>NOTE: %s reg error\n", __func__);
                return 0;
	}

	ret = akm4753_i2c_read_reg(reg_addr, &data, 1);

	return ret != 0 ? 0 : data;
}

/*
 * write akm4753 register
 */
static int akm4753_i2c_write(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int value)
{
	int ret =0;
	unsigned char data;
	data = (unsigned char)value;

	ret = akm4753_i2c_write_regs((unsigned char)reg, &data, 1);
	if (ret != 0)
		printk("%s i2c error \n", __func__);
	else
		akm4753_write_reg_cache(codec, reg, data);

	return 0;
}

static int dump_codec_regs(void)
{
        int i;
        int ret = 0;
        unsigned char reg_val[126] = {0};

        printk("akm4753 registers:\n");

        ret = akm4753_i2c_read_reg(0x0, reg_val, 126);
        for (i = 0; i < 126; i++){
                printk("reg 0x%02x = 0x%02x\n", i, reg_val[i]);
        }
        return ret;
}

static int codec_reset(void)
{
        int ret = 0;

        if (akm4753->control_pin->pdn->gpio != -1) {
		gpio_set_value(akm4753->control_pin->pdn->gpio, akm4753->control_pin->pdn->active_level);
		msleep(20);
		gpio_set_value(akm4753->control_pin->pdn->gpio,!akm4753->control_pin->pdn->active_level);
		msleep(1);
	}

        return ret;
}

static int ingenic_snd_soc_info_volsw_2r(struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_info *uinfo)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	int platform_max;
	int min = mc->min;
	int max = mc->max;

	if (!mc->platform_max)
		mc->platform_max = max;
	platform_max = mc->platform_max;

	if (platform_max == 1 && !strstr(kcontrol->id.name, " Volume"))
		uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
	else
		uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;

	uinfo->count = 2;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = max - min;
	return 0;
}

static int ingenic_snd_soc_get_volsw_2r(struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int reg = mc->reg;
	unsigned int reg2 = mc->rreg;
	unsigned int shift = mc->shift;
	int max = mc->max;
	int min = mc->min;
	unsigned int mask = (1 << fls(max)) - 1;
	unsigned int invert = mc->invert;

	ucontrol->value.integer.value[0] =
		((snd_soc_read(codec, reg) >> shift) & mask) - min;
	ucontrol->value.integer.value[1] =
		((snd_soc_read(codec, reg2) >> shift) & mask) - min;
	if (invert) {
		ucontrol->value.integer.value[0] =
			max - ucontrol->value.integer.value[0] - min;
		ucontrol->value.integer.value[1] =
			max - ucontrol->value.integer.value[1] - min;
	}

	return 0;
}

static int ingenic_snd_soc_put_volsw_2r(struct snd_kcontrol *kcontrol,
        struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int reg = mc->reg;
	unsigned int reg2 = mc->rreg;
	unsigned int shift = mc->shift;
	int max = mc->max;
	int min = mc->min;
	/* 
	 * Platform_max is stand for the register bit mask.
	 * Here we just use some bits of the register.
	 * The other bits of the function, we should clear it.
	*/
	int platform_max = mc->platform_max;
	unsigned int mask = (1 << fls(max)) - 1;
	unsigned int mask_1 = (1 << fls(platform_max)) - 1;
	unsigned int invert = mc->invert;
	int err;
	unsigned int val, val2, val_mask;

	val_mask = mask_1 << shift;
	val = ((ucontrol->value.integer.value[0] + min) & mask);
	val2 = ((ucontrol->value.integer.value[1] + min) & mask);

	if (invert) {
		val = max - (val - min);
		val2 = max - (val2 - min);
	}

	val = val << shift;
	val2 = val2 << shift;

	err = snd_soc_update_bits_locked(codec, reg, val_mask, val);
	if (err < 0)
		return err;

	err = snd_soc_update_bits_locked(codec, reg2, val_mask, val2);
	return err;
}

#define INGENIC_SOC_DOUBLE_R_TLV(xname, reg_left, reg_right, xshift, xmin, xmax, xmask, xinvert, tlv_array) \
{       .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
        .access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
                 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
        .tlv.p = (tlv_array), \
        .info = ingenic_snd_soc_info_volsw_2r, \
        .get = ingenic_snd_soc_get_volsw_2r, .put = ingenic_snd_soc_put_volsw_2r, \
        .private_value = (unsigned long)&(struct soc_mixer_control) \
                {.reg = reg_left, .rreg = reg_right, .shift = xshift, \
                .min = xmin, .max = xmax, .platform_max = xmask, .invert = xinvert} }

static const char *akm4753_dsp_mix_switch[] = {"LINEIN/I2S NO MIX", "LINEIN/I2S MIX"};
static const char *akm4753_dsp_lineout_mode_select[] = {"STEREO MODE 1", "STEREO MODE 2", "2.1 CHANNELS MODE", "4 CHANNELS MODE"};
static const char *akm4753_dsp1_hpf_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp1_hpf_select[] = {"LPF", "HPF"};
static const char *akm4753_dsp2_hpf_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp2_hpf_select[] = {"LPF", "HPF"};
static const char *akm4753_dsp1_limiter_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp2_limiter_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp1_eq1_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp1_eq2_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp1_eq3_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp1_eq4_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp1_eq5_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp2_eq1_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp2_eq2_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp2_eq3_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp2_eq4_switch[] = {"DISABLE", "ENABLE"};
static const char *akm4753_dsp2_eq5_switch[] = {"DISABLE", "ENABLE"};

static const struct soc_enum akm4753_enum[] = {
	SOC_ENUM_SINGLE(SIGNAL_PATH, 2, 2, akm4753_dsp_mix_switch),
	SOC_ENUM_SINGLE(SIGNAL_PATH, 4, 4, akm4753_dsp_lineout_mode_select),
	SOC_ENUM_SINGLE(DSP1_HPF_LPF_SET, 0, 2, akm4753_dsp1_hpf_switch),
	SOC_ENUM_SINGLE(DSP1_HPF_LPF_SET, 1, 2, akm4753_dsp1_hpf_select),
	SOC_ENUM_SINGLE(DSP2_HPF_LPF_SET, 0, 2, akm4753_dsp2_hpf_switch),
	SOC_ENUM_SINGLE(DSP2_HPF_LPF_SET, 1, 2, akm4753_dsp2_hpf_select),
	SOC_ENUM_SINGLE(SIGNAL_PATH, 7, 2, akm4753_dsp1_limiter_switch),
	SOC_ENUM_SINGLE(SIGNAL_PATH, 6, 2, akm4753_dsp2_limiter_switch),
	SOC_ENUM_SINGLE(DSP1_EQ_SELECT, 0, 2, akm4753_dsp1_eq1_switch),
	SOC_ENUM_SINGLE(DSP1_EQ_SELECT, 1, 2, akm4753_dsp1_eq2_switch),
	SOC_ENUM_SINGLE(DSP1_EQ_SELECT, 2, 2, akm4753_dsp1_eq3_switch),
	SOC_ENUM_SINGLE(DSP1_EQ_SELECT, 3, 2, akm4753_dsp1_eq4_switch),
	SOC_ENUM_SINGLE(DSP1_EQ_SELECT, 4, 2, akm4753_dsp1_eq5_switch),
	SOC_ENUM_SINGLE(DSP2_EQ_SELECT, 0, 2, akm4753_dsp2_eq1_switch),
	SOC_ENUM_SINGLE(DSP2_EQ_SELECT, 1, 2, akm4753_dsp2_eq2_switch),
	SOC_ENUM_SINGLE(DSP2_EQ_SELECT, 2, 2, akm4753_dsp2_eq3_switch),
	SOC_ENUM_SINGLE(DSP2_EQ_SELECT, 3, 2, akm4753_dsp2_eq4_switch),
	SOC_ENUM_SINGLE(DSP2_EQ_SELECT, 4, 2, akm4753_dsp2_eq5_switch),
};

/* unit: 0.01dB */
#ifdef USE_GAIN_FULL_RANGE
/* If you want to use full gain range:-127dB ~ 0dB, you can enable the code */
static const DECLARE_TLV_DB_SCALE(dac_tlv, -12700, 50, 1);
#else

#ifdef CONFIG_BOARD_X1000_HL01_V10
/* Here we just use -16dB ~ -49dB */
static const DECLARE_TLV_DB_SCALE(dac_tlv, -4900, 50, 0);
#else
/* Here we just use -30dB ~ 0dB */
static const DECLARE_TLV_DB_SCALE(dac_tlv, -3000, 50, 0);
#endif

#endif

static const struct snd_kcontrol_new akm4753_snd_controls[] = {
#ifdef USE_GAIN_FULL_RANGE
	/* If you want to use full gain range:-127dB ~ 0dB, you can enable the code */
	SOC_DOUBLE_R_TLV("Master Playback Volume", LCH_DIGITAL_VOLUME, RCH_DIGITAL_VOLUME, 0, 0xff, 1, dac_tlv),
#else

#ifdef CONFIG_BOARD_X1000_HL01_V10
	/* Because of the hl01 board designed, here we just use -16dB ~ -49dB */
	INGENIC_SOC_DOUBLE_R_TLV("Master Playback Volume", LCH_DIGITAL_VOLUME, RCH_DIGITAL_VOLUME, 0, 0x20, 0x62, 0xff, 1, dac_tlv),
#else
	/* Here we use -30dB ~ 0dB */
	INGENIC_SOC_DOUBLE_R_TLV("Master Playback Volume", LCH_DIGITAL_VOLUME, RCH_DIGITAL_VOLUME, 0, 0x0, 0x3c, 0xff, 1, dac_tlv),
#endif

#endif
	SOC_SINGLE("Digital Playback mute", POWER_MANAGE, 3, 1, 1),
	SOC_SINGLE("Aux In Switch", SIGNAL_PATH, 0, 1, 1),
	//SOC_ENUM("Dsp Input Mix Switch", akm4753_enum[0]),	//This kcontrol is only for such as karaoke.
	SOC_ENUM("Output Mode Select", akm4753_enum[1]),
	SOC_ENUM("Dsp1 HPF/LPF Enable", akm4753_enum[2]),
	SOC_ENUM("Dsp1 HPF/LPF Select", akm4753_enum[3]),
	SOC_ENUM("Dsp2 HPF/LPF Enable", akm4753_enum[4]),
	SOC_ENUM("Dsp2 HPF/LPF Select", akm4753_enum[5]),
	SOC_ENUM("Dsp1 Limiter Switch", akm4753_enum[6]),
	SOC_ENUM("Dsp2 Limiter Switch", akm4753_enum[7]),
	SOC_ENUM("Dsp1 Eq1 Switch", akm4753_enum[8]),
	SOC_ENUM("Dsp1 Eq2 Switch", akm4753_enum[9]),
	SOC_ENUM("Dsp1 Eq3 Switch", akm4753_enum[10]),
	SOC_ENUM("Dsp1 Eq4 Switch", akm4753_enum[11]),
	SOC_ENUM("Dsp1 Eq5 Switch", akm4753_enum[12]),
	SOC_ENUM("Dsp2 Eq1 Switch", akm4753_enum[13]),
	SOC_ENUM("Dsp2 Eq2 Switch", akm4753_enum[14]),
	SOC_ENUM("Dsp2 Eq3 Switch", akm4753_enum[15]),
	SOC_ENUM("Dsp2 Eq4 Switch", akm4753_enum[16]),
	SOC_ENUM("Dsp2 Eq5 Switch", akm4753_enum[17]),
};

/* akm4753 dapm widgets */
static const struct snd_soc_dapm_widget akm4753_dapm_widgets[] = {
	/* akm4753 dapm route */
	SND_SOC_DAPM_DAC("DAC", "Playback", SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_ADC("ADC", "Capture", SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_OUTPUT("DAC OUT"),
	SND_SOC_DAPM_INPUT("ADC IN"),
};

static const struct snd_soc_dapm_route akm4753_audio_map[] = {
	/* ADC */
	{"ADC", NULL, "ADC IN"},
	{"DAC OUT", NULL, "DAC"},
};

static int akm4753_set_dai_sysclk(struct snd_soc_dai *codec_dai,
	int clk_id, unsigned int freq, int dir)
{
	/*
	 * AKM4753 only need a 24MHZ mclk as I2S master.
	 * It is set in asoc-i2s-v**.c.
	 */
	akm4753->sysclk = freq;
	return 0;
}

static int akm4753_hw_params(struct snd_pcm_substream *substream,
			    struct snd_pcm_hw_params *params,
			    struct snd_soc_dai *dai)
{
	/* 
	 * Params_format and params_channels are not set here.
	 * Because if you set register DIF to 0x3, akm4753 support all the I2S format.
	 * Such as 16bit,20bit,24bit.
	 * And akm4753 support stereo channels. 
	 * If the replay channel is mono, we should transfer it to stereo by AIC register.
	 */
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	int rate = params_rate(params);
	int i;
	unsigned char data = 0x0;
        unsigned long mrate[12] = {
                8000, 12000, 16000, 24000, 7350, 11025,
                14700,22050, 32000, 48000, 29400,44100
        };
        unsigned char reg[12] = {
                0, 1, 2, 3, 4, 5,
                6, 7, 10,11,14,15
        };

        for (i = 0; i < 12; i++) {
                if (rate == mrate[i])
                        break;
        }
        if (i == 12){
                printk("Replay rate %d is not support by akm4753, we fix it to 48000\n", rate);
                rate = 48000;
                i = 9;
        }

        if (user_replay_rate == rate)
                return 0;
        user_replay_rate = rate;

        akm4753_i2c_read_reg(0x02, &data, 1);
	data &= 0xf;
        data |= reg[i]<<4;
        akm4753_i2c_write_regs(0x02, &data, 1);
        msleep(50);            // Wait for akm4753 i2s clk stable.
        return 0;
}

static int akm4753_mute(struct snd_soc_dai *dai, int mute)
{
	if (!mute){

	} else {

	}
	return 0;
}

static int akm4753_set_bias_level(struct snd_soc_codec *codec,
	enum snd_soc_bias_level level)
{
	/* There is no bias level function in akm4753 */
	switch (level) {
	case SND_SOC_BIAS_ON:
		break;
	case SND_SOC_BIAS_PREPARE:
		break;
	case SND_SOC_BIAS_STANDBY:
		break;
	case SND_SOC_BIAS_OFF:
		break;
	}
	codec->dapm.bias_level = level;
	return 0;
}

#ifdef CONFIG_SND_ASOC_AKM4753_AEC_MODE
#define AKM4753_RATES   (SNDRV_PCM_RATE_48000)
#else
#define AKM4753_RATES   (SNDRV_PCM_RATE_8000_48000)
#endif
#define AKM4753_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE)

static int akm4753_startup(struct snd_pcm_substream *substream, struct snd_soc_dai *dai)
{
	/* snd_pcm_open will call it */
	return 0;
}
static void akm4753_shutdown(struct snd_pcm_substream *substream, struct snd_soc_dai *dai)
{
	/* snd_pcm_close will call it */
	return;
}

static struct snd_soc_dai_ops akm4753_dai_ops = {
	.hw_params	= akm4753_hw_params,
	.digital_mute	= akm4753_mute,
	.set_sysclk	= akm4753_set_dai_sysclk,
	.startup	= akm4753_startup,
	.shutdown	= akm4753_shutdown,
};

static struct snd_soc_dai_driver akm4753_dai = {
	.name = "akm4753-dai",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 1,
#ifdef CONFIG_BOARD_X1000_HL01_V10
		/* Because of the hl01 board designed, user space should mix stereo sound to mono here.*/
		.channels_max = 1,
#else
		.channels_max = 2,
#endif
		.rates = AKM4753_RATES,
		.formats = AKM4753_FORMATS,
	},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rates = AKM4753_RATES,
		.formats = AKM4753_FORMATS,
	},
	.ops = &akm4753_dai_ops,
};

static int akm4753_suspend(struct snd_soc_codec *codec, pm_message_t state)
{
	int ret = 0;
	unsigned char data;
	spk_mute();
	msleep(5);
	data = 0x84;
	ret |= akm4753_i2c_write_regs(0x04, &data, 1);
	return 0;
}

static int akm4753_resume(struct snd_soc_codec *codec)
{
	int ret = 0;
	unsigned char data;
	data = 0xbd;
	ret |= akm4753_i2c_write_regs(0x04, &data, 1);
	msleep(5);
	spk_unmute();
        return 0;
}

static int codec_dac_setup(void)
{
	int i;
	int ret = 0;
	unsigned char data = 0x0;
	/* Disable SAR and SAIN */
        data = 0x0;
        ret |= akm4753_i2c_write_regs(0x0, &data, 1);

#ifdef CONFIG_BOARD_X1000_HL01_V10
        /* Set up stereo mode(HPF, LPF individual mode) */
        data = 0x11;
        ret |= akm4753_i2c_write_regs(0x01, &data, 1);
#else
        /* Set up 2.1-channels mode */
        data = 0x21;
        ret |= akm4753_i2c_write_regs(0x01, &data, 1);
#endif

        /* Init 0x05 ~ 0x7d registers */
        for (i = 0; i < sizeof(akm4753_registers) / sizeof(akm4753_registers[0]); i++){
                ret |= akm4753_i2c_write_regs(akm4753_registers[i][0], &akm4753_registers[i][1], 1);
        }

        /* Power on DAC ADC DSP */
        data = 0xbd;
        ret |= akm4753_i2c_write_regs(0x04, &data, 1);

        if (ret)
                printk("===>ERROR: akm4753 init error!\n");

	return ret;
}

static int codec_clk_setup(void)
{
	int ret = 0;
	unsigned char data = 0x0;
	/* I2S clk setup */
        data = 0xb7;		//default 48KHZ sample rate
        ret |= akm4753_i2c_write_regs(0x02, &data, 1);
        data = 0xc3;
        ret |= akm4753_i2c_write_regs(0x03, &data, 1);
        data = 0x84;
        ret |= akm4753_i2c_write_regs(0x04, &data, 1);
        msleep(5);

	return ret;
}

static ssize_t akm4753_regs_show(struct device *dev,
                struct device_attribute *attr, char *buf)
{
	dump_codec_regs();
        return 0;
}

/* The codec register write interface is just for debug. */
#define AKM4753_REG_SUM         0x7d
static ssize_t akm4753_regs_write(struct device *dev, struct device_attribute *attr,
                const char *buf, size_t n)
{
        int ret;
        char * buf_2;
        unsigned char reg_addr, reg_val;

        reg_addr  = (unsigned char)simple_strtoul(buf, &buf_2, 0);
        buf_2 = skip_spaces(buf_2);
        reg_val = (unsigned char)simple_strtoul(buf_2, NULL, 0);

        if (reg_addr > AKM4753_REG_SUM)
                return -EINVAL;

        printk("\nwrite reg: 0x%x 0x%x\n", reg_addr, reg_val);

        ret = akm4753_i2c_write_regs(reg_addr, &reg_val, 1);
        if (ret)
                printk("write reg fail\n");
        return n;
}

static struct device_attribute akm4753_sysfs_attrs = 
	__ATTR(akm4753_regs, S_IRUGO | S_IWUSR, akm4753_regs_show, akm4753_regs_write);

static int akm4753_probe(struct snd_soc_codec *codec)
{
	int ret = 0;
	/* akm4753 pdn pin reset */
	ret = codec_reset();
        if (ret < 0) {
                dev_err(codec->dev, "Failed to issue reset\n");
                return ret;
        }

	/* clk setup */
	ret |= codec_clk_setup();

	/* DAC outputs setup */
	ret |= codec_dac_setup();

	/* If there is a amp power control pin, you should power on it */
	amp_poweron();

	/* Creat dump codec register file interface */
	ret = device_create_file(codec->dev, &akm4753_sysfs_attrs);
	if (ret)
		dev_warn(codec->dev,"attribute %s create failed %x",
				attr_name(akm4753_sysfs_attrs), ret);

	return ret;
}

/* power down chip */
static int akm4753_remove(struct snd_soc_codec *codec)
{
	int ret = 0;
	unsigned char data;
	/* Mute AMP pin */
	spk_mute();
        msleep(5);

	/* If there is a amp power control pin, you should power down it */
	amp_powerdown();

	/* Power down akm4753 */
        akm4753_i2c_read_reg(0x04, &data, 1);
        data = 0x0;
        ret |= akm4753_i2c_write_regs(0x04, &data, 1);

	if (akm4753->control_pin->pdn->gpio != -1){
		gpio_set_value(akm4753->control_pin->pdn->gpio, akm4753->control_pin->pdn->active_level);
	}

	if (ret < 0) {
		printk("%s i2c write err\n", __func__);
		return ret;
	}

	/* Remove the dump register file interface */
	device_remove_file(codec->dev, &akm4753_sysfs_attrs);

	return 0;
}

static struct snd_soc_codec_driver soc_codec_dev_akm4753 = {
	.probe =	akm4753_probe,
	.remove =	akm4753_remove,
	.suspend =	akm4753_suspend,
	.resume =	akm4753_resume,
	//.read = akm4753_read_reg_cache,
	.read =  akm4753_i2c_read,
	.write = akm4753_i2c_write,
	.set_bias_level = akm4753_set_bias_level,
	.reg_cache_size = AKM4753_REGNUM,
	.reg_word_size = sizeof(u8),
	//.reg_cache_default = akm4753_reg,
	.controls =     akm4753_snd_controls,
        .num_controls = ARRAY_SIZE(akm4753_snd_controls),
	.dapm_widgets = akm4753_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(akm4753_dapm_widgets),
	.dapm_routes = akm4753_audio_map,
	.num_dapm_routes = ARRAY_SIZE(akm4753_audio_map),
};

static __devinit int akm4753_i2c_probe(struct i2c_client *i2c,
				      const struct i2c_device_id *id)
{
	int ret = 0;
        struct device *dev = &i2c->dev;
        struct akm4753_platform_data *akm4753_data = dev->platform_data;

        if (!i2c_check_functionality(i2c->adapter, I2C_FUNC_I2C)) {
                return -ENODEV;
        }

        if (akm4753_data->pdn->gpio != -1) {
                ret = gpio_request_one(akm4753_data->pdn->gpio,
                                GPIOF_DIR_OUT, "akm4753_pdn");
                if (ret != 0) {
                        dev_err(dev, "Can't request pdn pin = %d\n",
                                        akm4753_data->pdn->gpio);
                        return ret;
                } else
			gpio_set_value(akm4753_data->pdn->gpio, akm4753_data->pdn->active_level);
        }

	akm4753 = kzalloc(sizeof(struct akm4753_priv), GFP_KERNEL);
	if (akm4753 == NULL)
		return -ENOMEM;

	akm4753->i2c_client   = i2c;
	akm4753->control_type = SND_SOC_I2C;
	akm4753->control_pin  = akm4753_data;

	mutex_init(&akm4753->i2c_access_lock);

	ret = snd_soc_register_codec(&i2c->dev, &soc_codec_dev_akm4753, &akm4753_dai, 1);
	if (ret < 0) {
		kfree(akm4753);
		dev_err(&i2c->dev, "Faild to register codec\n");
	}
	return ret;
}

static __devexit int akm4753_i2c_remove(struct i2c_client *client)
{
	struct device *dev = &client->dev;
        struct akm4753_platform_data *akm4753_data = dev->platform_data;

	if (akm4753_data->pdn->gpio != -1)
                gpio_free(akm4753_data->pdn->gpio);

	snd_soc_unregister_codec(&client->dev);

	kfree(akm4753);
	return 0;
}

static const struct i2c_device_id akm4753_i2c_id[] = {
	{ "akm4753", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, akm4753_i2c_id);

static struct i2c_driver akm4753_i2c_driver = {
	.driver = {
		.name = "akm4753",
		.owner = THIS_MODULE,
	},
	.probe =    akm4753_i2c_probe,
	.remove =   __devexit_p(akm4753_i2c_remove),
	.id_table = akm4753_i2c_id,
};

static int __init akm4753_init(void)
{
	int ret = 0;
	ret = i2c_add_driver(&akm4753_i2c_driver);
	if (ret != 0) {
		printk(KERN_ERR "Failed to register AKM4753 I2C driver: %d\n",
		       ret);
	}
	return ret;
}

static void __exit akm4753_exit(void)
{
	i2c_del_driver(&akm4753_i2c_driver);
}

module_init(akm4753_init);
module_exit(akm4753_exit);

MODULE_DESCRIPTION("Soc AKM4753 driver");
MODULE_AUTHOR("tjin<tao.jin@ingenic.com>");
MODULE_LICENSE("GPL");
