#ifndef DM6291_LED_H
#define DM6291_LED_H

typedef enum _LED_NUM
{
    DM6291_LED_BT=0,
	DM6291_LED_WIFI,
	DM6291_LED_PLAY,
	DM6291_LED_RED,
	DM6291_LED_BLUE,
	DM6291_LED_COUNT,
}DM6291_LED_NUM;

#define MAX_BRIGHTNESS 255
#define DM6291_LED_ON MAX_BRIGHTNESS
#define DM6291_LED_OFF 0



#define WIFI_BRIGHTNESS "/sys/class/leds/led-wifi/brightness"
#define WIFI_TRIGGER "/sys/class/leds/led-wifi/trigger"
#define WIFI_DEYLAY_ON "/sys/class/leds/led-wifi/delay_on"
#define WIFI_DELAY_OFF "/sys/class/leds/led-wifi/delay_off"


#define BT_BRIGHTNESS "/sys/class/leds/led-bt/brightness"
#define BT_TRIGGER "/sys/class/leds/led-bt/trigger"
#define BT_DEYLAY_ON "/sys/class/leds/led-bt/delay_on"
#define BT_DELAY_OFF "/sys/class/leds/led-bt/delay_off"

#define PLAY_BRIGHTNESS "/sys/class/leds/led-play/brightness"
#define PLAY_TRIGGER "/sys/class/leds/led-play/trigger"
#define PLAY_DEYLAY_ON "/sys/class/leds/led-play/delay_on"
#define PLAY_DELAY_OFF "/sys/class/leds/led-play/delay_off"


/*#define RED_BRIGHTNESS "/sys/class/leds/led_red/brightness"
#define RED_TRIGGER "/sys/class/leds/led_red/trigger"
#define RED_DEYLAY_ON "/sys/class/leds/led_red/delay_on"
#define RED_DELAY_OFF "/sys/class/leds/led_red/delay_off"

#define BLUE_BRIGHTNESS "/sys/class/leds/led_blue/brightness"
#define BLUE_TRIGGER "/sys/class/leds/led_blue/trigger"
#define BLUE_DEYLAY_ON "/sys/class/leds/led_blue/delay_on"
#define BLUE_DELAY_OFF "/sys/class/leds/led_blue/delay_off"*/




void DM6291_SetLedBrightness(DM6291_LED_NUM lednum,int brightness);
void DM6291_SetLedBlink(DM6291_LED_NUM lednum,int delay_on,int delay_off);
int DM6291_SetLedBreath(DM6291_LED_NUM lednum,int period);
int DM6291_SetLedExitBreath(DM6291_LED_NUM lednum);
int update_led_status();

#endif
