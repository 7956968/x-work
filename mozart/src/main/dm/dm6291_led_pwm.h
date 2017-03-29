#ifndef DM6291_LED_PWM_H
#define DM6291_LED_PWM_H





void DM6291_SetLedBrightness_pwm(DM6291_LED_NUM lednum,int brightness);
void DM6291_SetLedBlink_pwm(DM6291_LED_NUM lednum,int delay_on,int delay_off);
int DM6291_SetLedBreath_pwm(DM6291_LED_NUM lednum,int period);
int DM6291_SetLedExitBreath_pwm(DM6291_LED_NUM lednum);
int DM6291_SetLedExitBreath_pwm(DM6291_LED_NUM lednum);

#endif
