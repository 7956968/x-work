#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "mozart_config.h"
#include "dm6291_led.h"


#include "dm_mozart.h"



void DM6291_SetLedBrightness_gpio(DM6291_LED_NUM lednum,int brightness)
{
    int led_fd = -1;
	int write_size = 0;
    char led_path[64] = {0};
	char brightness_str[4]={0};

	if (get_brightness_path(lednum, led_path, sizeof(led_path) ) < 0) 
		return;
	
	led_fd = open(led_path, O_WRONLY);// O_RDONLY  O_WRONLY O_RDWR
	if (led_fd < 0) 
	{
		printf("[DM6291 ERROR] open %s error: %s\n", led_path,strerror(errno));
		return;
	}
    snprintf(brightness_str, sizeof(brightness_str),"%d",brightness);

	if(DM6291_LED_ON == brightness)//if it is blinking ,must write 0 to finish blink.
	{
	    write(led_fd, "0", 1);
		usleep(1000);
	}
	write_size = write(led_fd, brightness_str, strlen(brightness_str));
	if (write_size < 0) {
		printf("[DM6291 ERROR] write %s to %s failed: %s\n", brightness_str,led_path, strerror(errno));
	}
    close(led_fd);
}

void DM6291_SetLedBlink_gpio(DM6291_LED_NUM lednum,int delay_on,int delay_off)
{
    int blinkfd = -1;
	//int write_size = 0;
    char trigger_path[64] = {0};
	char delay_on_path[64] = {0};
	char delay_off_path[64] = {0};
	char delay_str[16]={0};

	if (get_trigger_path(lednum, trigger_path, sizeof(trigger_path) ) < 0 ||
		get_delayon_path(lednum, delay_on_path, sizeof(delay_on_path) ) < 0 ||
		get_delayoff_path(lednum, delay_off_path, sizeof(delay_off_path) ) < 0 ) 
		return ;

    //write trigger
    blinkfd = open(trigger_path, O_WRONLY);// O_RDONLY  O_WRONLY O_RDWR
    if (blinkfd < 0) 
    {
	    printf("[DM6291 ERROR] open %s error: %s\n", trigger_path,strerror(errno));
	    return;
    }
	write(blinkfd, "timer", 5);
	close(blinkfd);

	//write delay_on
    blinkfd = open(delay_on_path, O_WRONLY);// O_RDONLY  O_WRONLY O_RDWR
    if (blinkfd < 0) 
    {
	    printf("[DM6291 ERROR] open %s error: %s\n", delay_on_path,strerror(errno));
	    return;
    }
	snprintf(delay_str, sizeof(delay_str),"%d",delay_on);
	write(blinkfd, delay_str, strlen(delay_str));
	close(blinkfd);

    //write delay_off
	blinkfd = open(delay_off_path, O_WRONLY);// O_RDONLY  O_WRONLY O_RDWR
    if (blinkfd < 0)
    {
	    printf("[DM6291 ERROR] open %s error: %s\n", delay_off_path,strerror(errno));
	    return;
    }
	memset(delay_str,0x0,sizeof(delay_str));
	snprintf(delay_str, sizeof(delay_str),"%d",delay_off);
	write(blinkfd, delay_str, strlen(delay_str));
	close(blinkfd);
}

