#ifndef DM6291_LED_H
#define DM6291_LED_H


#include <sharememory_interface.h>
#include "dm_mozart.h"
typedef enum _LED_NUM
{
    DM6291_LED_TOUCH_PWM=0,
	DM6291_LED_STATE_PWM,
	DM6291_LED_NET,
	DM6291_LED_SD,
	DM6291_LED_USB,
	DM6291_LED_WIFI,
	DM6291_LED_BT,
	DM6291_LED_COUNT,
}DM6291_LED_NUM;

enum LED_TYPE {
	LED_PWM = 0,
	LED_GPIO,
};

struct dm_led {
	char* name;
	enum LED_TYPE type;
};

#define MAX_BRIGHTNESS 255
#define DM6291_LED_ON MAX_BRIGHTNESS
#define DM6291_LED_OFF 0


int led_state_off();
int led_state_on();
int led_play_mode(enum PlayMode);


int update_led_status();
int get_trigger_path(DM6291_LED_NUM led, char* path, int size);
int get_brightness_path(DM6291_LED_NUM led, char* path, int size);
int get_delayon_path(DM6291_LED_NUM led, char* path, int size);
int get_delayoff_path(DM6291_LED_NUM led, char* path, int size);

#endif
