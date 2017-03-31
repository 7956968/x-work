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


int led_play_mode(enum PlayMode mode)
{
	DM6291_SetLedBrightness_gpio(DM6291_LED_SD,DM6291_LED_OFF);	
	DM6291_SetLedBrightness_gpio(DM6291_LED_USB,DM6291_LED_OFF);	
	DM6291_SetLedBrightness_gpio(DM6291_LED_NET,DM6291_LED_OFF);	
	DM6291_SetLedBrightness_gpio(DM6291_LED_WIFI,DM6291_LED_OFF);	
	DM6291_SetLedBrightness_gpio(DM6291_LED_BT,DM6291_LED_OFF);	
	switch(mode) {
		case PM_SDCARD:
			DM6291_SetLedBrightness_gpio(DM6291_LED_SD,DM6291_LED_ON);	
			break;
		case PM_UDISK:
			DM6291_SetLedBrightness_gpio(DM6291_LED_USB,DM6291_LED_ON);	
			break;
		case PM_RENDER:
		case PM_AIRPLAY:
			DM6291_SetLedBrightness_gpio(DM6291_LED_WIFI,DM6291_LED_ON);	
			break;
		case PM_BTAVK:
		case PM_BTHS:
			DM6291_SetLedBrightness_gpio(DM6291_LED_BT,DM6291_LED_ON);	
			break;
		case PM_CLOUD:	
		case PM_INGENIC:
		case PM_VR:
			DM6291_SetLedBrightness_gpio(DM6291_LED_NET,DM6291_LED_ON);
			break;
		default:
			break;
	}
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
