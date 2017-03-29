#ifndef DM6291_LED_GPIO_H
#define DM6291_LED_GPIO_H


void DM6291_SetLedBrightness_gpio(DM6291_LED_NUM lednum,int brightness);
void DM6291_SetLedBlink_gpio(DM6291_LED_NUM lednum,int delay_on,int delay_off);

#endif
