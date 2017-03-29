#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include "mozart_config.h"
#include "dm6291_led.h"
#include "dm6291_led_gpio.h"
#include "dm6291_led_pwm.h"

#include "dm_mozart.h"


struct dm_led g_dm_leds[DM6291_LED_COUNT] = {
	[DM6291_LED_TOUCH_PWM] = {"led-touch", LED_PWM},
	[DM6291_LED_STATE_PWM] = {"led-state", LED_PWM},
	[DM6291_LED_NET] = {"led-net", LED_GPIO},
	[DM6291_LED_SD] = {"led-sd", LED_GPIO},
	[DM6291_LED_USB] = {"led-usb", LED_GPIO},
	[DM6291_LED_WIFI] = {"led-wifi", LED_GPIO},
	[DM6291_LED_BT] = {"led-bt", LED_GPIO},
};

int get_trigger_path(DM6291_LED_NUM led, char* path, int size)
{
	snprintf(path, size, "/sys/class/leds/%s/trigger", g_dm_leds[led].name);
	return 0;
}

int get_brightness_path(DM6291_LED_NUM led, char* path, int size)
{
	snprintf(path, size, "/sys/class/leds/%s/brightness", g_dm_leds[led].name);
	return 0;
}

int get_delayon_path(DM6291_LED_NUM led, char* path, int size)
{
	snprintf(path, size, "/sys/class/leds/%s/delay_on", g_dm_leds[led].name);
	return 0;
}

int get_delayoff_path(DM6291_LED_NUM led, char* path, int size)
{
	snprintf(path, size, "/sys/class/leds/%s/delay_off", g_dm_leds[led].name);
	return 0;
}

int turn_on_musicplayer_led(int musicplayer_type)
{
	switch(musicplayer_type) {
		case MUSIC_USB:
			DM6291_SetLedBrightness_gpio(DM6291_LED_USB,DM6291_LED_ON);	
			break;
		case MUSIC_SD:
			DM6291_SetLedBrightness_gpio(DM6291_LED_SD,DM6291_LED_ON);	
			break;
		case MUSIC_CLOUD:
		default:
			DM6291_SetLedBrightness_gpio(DM6291_LED_NET,DM6291_LED_ON);	
			break;
	}
	return 0;
}

int led_mode_on(memory_domain domain)
{
#if 0
	static memory_domain cur_domain = UNKNOWN_DOMAIN;
	static int cur_musicplayer_type = 0;
	int musicplayer_type = get_musicplayer_type();
	if (domain == cur_domain && musicplayer_type == cur_musicplayer_type)
		return 0;
	cur_domain = domain;
	cur_musicplayer_type = musicplayer_type;
#endif
	DM6291_SetLedBrightness_gpio(DM6291_LED_SD,DM6291_LED_OFF);	
	DM6291_SetLedBrightness_gpio(DM6291_LED_USB,DM6291_LED_OFF);	
	DM6291_SetLedBrightness_gpio(DM6291_LED_NET,DM6291_LED_OFF);	
	DM6291_SetLedBrightness_gpio(DM6291_LED_WIFI,DM6291_LED_OFF);	
	DM6291_SetLedBrightness_gpio(DM6291_LED_BT,DM6291_LED_OFF);	
	switch(domain) {
		case SDCARD_DOMAIN:
			DM6291_SetLedBrightness_gpio(DM6291_LED_SD,DM6291_LED_ON);	
			break;
		case UDISK_DOMAIN:
			DM6291_SetLedBrightness_gpio(DM6291_LED_USB,DM6291_LED_ON);	
			break;
		case RENDER_DOMAIN:
		case AIRPLAY_DOMAIN:
			DM6291_SetLedBrightness_gpio(DM6291_LED_WIFI,DM6291_LED_ON);	
			break;
		case BT_AVK_DOMAIN:
			DM6291_SetLedBrightness_gpio(DM6291_LED_BT,DM6291_LED_ON);	
			break;
		case MUSICPLAYER_DOMAIN:	//MUSICPLAYERDOMAIN means cloud music
		case LAPSULE_DOMAIN://LAPSULE_MOMAIN means ingenic player
		case VR_DOMAIN:
			DM6291_SetLedBrightness_gpio(DM6291_LED_NET,DM6291_LED_ON);
			break;
		default:
			break;
	}
	printf("current domain: %s\n", memory_domain_str[domain]);
	return 0;
}




int update_led_status()
{	
	static int airkiss = 0;
	static int playing = 0;
	static int touch = 0;
	if (airkiss != get_airkissing() || touch != get_touch()) {
		airkiss = get_airkissing();
		touch = get_touch();
		if (touch) 
			DM6291_SetLedBrightness_pwm(DM6291_LED_STATE_PWM,DM6291_LED_ON);
		else if (airkiss) {
				printf("led airkiss status\n");
				//DM6291_SetLedBrightness_pwm(DM6291_LED_TOUCH_PWM,DM6291_LED_OFF);//Mutex
				DM6291_SetLedBlink_pwm(DM6291_LED_STATE_PWM,200,200);
				//DM6291_SetLedBrightness_pwm(DM6291_LED_TOUCH_PWM,DM6291_LED_OFF);
			}else
				DM6291_SetLedBrightness_pwm(DM6291_LED_STATE_PWM,DM6291_LED_OFF);
	}

	if (playing != get_playing()) {
		playing = get_playing();
		if (playing) {
			//printf("led playing status\n");
			DM6291_SetLedBrightness_pwm(DM6291_LED_TOUCH_PWM,DM6291_LED_ON);
		} else {
			//printf("led idle status\n");
			DM6291_SetLedBreath_pwm(DM6291_LED_TOUCH_PWM,2);
		}
	}
	return 0;
}
