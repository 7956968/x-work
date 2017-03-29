#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "sharememory_interface.h"
#include "tips_interface.h"
#include "dm6291_led.h"
#include "ini_interface.h"
#include "mozart_key_function.h"
#include "dm_mozart.h"

volatile static int g_busy_flag = 0;
volatile static int g_airkissing = 0;
volatile static int g_playing = 0;

	
//ini
/////////////////////////////////////////////////////////////////////{
char* dm_get_ini(char* file, char* section, char* key)
{	
	char *value = malloc(64);	
	if (value == NULL) {	
		printf( "%s:malloc failed\n", __FUNCTION__); 
		return NULL;	
	}	
	value[0] = '\0';	
	if (mozart_ini_getkey(file, section, key, value)|| strlen(value) == 0) {
		if (strlen(value) == 0)
			printf( "file = %s, section = %s, key = %s, value equals \"\"\n", file, section, key); 
		else
			printf( "Could not get ini,file = %s, section = %s, key = %s.\n", file, section, key); 
			safe_free(value);		
		return NULL;			
	}	
	return value;
}
	
#define NETCONFIGPATH "/usr/data/wireless.ini"
#define NETCLIENTSECTION "CLIENT"

char* sys_get_stassid()
{
	return dm_get_ini(NETCONFIGPATH, NETCLIENTSECTION, "ssid");
}
char* sys_get_stakey()
{
	return dm_get_ini(NETCONFIGPATH, NETCLIENTSECTION, "key");
}
/////////////////////////////////////////////////////////////////////}


//dm_specified 
////////////////////////////////////////////////////// 

int get_airkissing()
{
	return g_airkissing;
}

int dm_set_airkissing(int i)
{
	g_airkissing = i;
	return 0;
}


int get_playing()
{
	return g_playing;
}

int domain_playing()
{
	
	memory_domain domain;
	int ret = -1;

	ret = share_mem_get_active_domain(&domain);
	if (ret) {
		printf("get active domain error in %s:%s:%d, do nothing.\n",
		       __FILE__, __func__, __LINE__);
		return -1;
	}
	module_status status;
	if (share_mem_get(domain, &status)) {
		printf("%s: share_mem_get %s fail.\n", __func__, memory_domain_str[domain]);
		return -1;
	}
//	printf("domain:%s status:%s\n", memory_domain_str[domain], module_status_str[status]);

	if (status == STATUS_PLAYING)
		return 1;
	return 0;
}

//monitor play status.if playing , led blinks;
int check_play_status()
{
	int old_playing = g_playing;
	g_playing = domain_playing();
	if (old_playing == 0 && g_playing == 1 ) {
	    printf("JJJHHH old_playing == 0 && g_playing == 1\n");
		update_led_status();
	}
	if (old_playing == 1 && g_playing == 0 ) {
	    printf("JJJHHH old_playing == 1 && g_playing == 0\n");
		update_led_status();
	}
	return 0;
}

int test_domain(char* s)
{

	memory_domain domain;
	int ret = -1;

	ret = share_mem_get_active_domain(&domain);
	if (ret) {
		printf("get active domain error in %s:%s:%d, do nothing.\n",
		       __FILE__, __func__, __LINE__);
		return 0;
	}
	
	if (domain > 0 && domain < MAX_DOMAIN)
		printf("%s: %s domain is active\n",	s, memory_domain_str[domain]);
	else
		printf("%s: Not support domain[%d] is active\n", s, domain);
	return 0;
}

void dm_playpause()
{
	test_domain("33333");
	printf("play dou\n");
	if (domain_playing()) {
		mozart_play_pause();
		//usleep(10000);
		mozart_play_key("dou");	
	}
	else{
		mozart_play_key("dou");	
		mozart_play_pause();
	}
	test_domain("44444");
	return ;
}
void dm_play()
{
	printf("play dou\n");
	if (!domain_playing()) {
		mozart_play_pause();
		//usleep(10000);
		//mozart_play_key("dou");	
	}
	return ;
}


void dm_pause()
{
	printf("pause dou\n");
	if (domain_playing()) {
		mozart_play_pause();
		//usleep(10000);
		
		//mozart_play_key("dou");	
	}
}
//return lt 0,error;eq 0,not in backup; gt 0 in backup
int in_backup_system()
{
	if (access("/tmp/backup_system", F_OK) == 0)
		return 1;
	else
		return 0;
	/*
	char* fw_version = NULL;
	if ( (fw_version = dm_get_ini("/etc/dmconfig.ini", "product", "fw_version") ) == NULL)
		return -1;
	if (strcmp(fw_version, "0.0.00") != 0) {
	//	printf( "fw_version = %s, not in backup system, backup_system expect 0.0.00\n", fw_version);
		return 0;
	}
	return 1;*/
}


/**
 * @brief Local supported audio formats.
 */
 //orgin supported audio formats
 #if 0 
static char *audios[] =
    { ".3gp", ".aac", ".ac3", ".aif", ".aiff", ".mp1", ".mp2",
	".mp3", ".mp4", ".m4a", ".ogg", ".wav",
	".wma", ".mka", ".ra", ".rm", "flac", ".ape", NULL
};
 #endif
static char *audios[] =
    { ".3gp", ".aac", ".ac3", ".aif", ".aiff", ".mp1", ".mp2",
	".mp3",  ".m4a", ".ogg", ".wav",
	".wma", ".mka", ".ra", ".rm", "flac", ".ape", NULL
};




/**
 * @brief To determine whether the specified file is an audio file.
 *
 * @param fileName Specified file.
 *
 * @return Returns true if the specified file is an audio file, and false
 * indicates that the specified file is not an audio file.
 */
int is_audio_file(char *fileName)
{
	int i = 0, len = 0;
	for (i = 0; audios[i]; i++) {
		len = strlen(fileName) - strlen(audios[i]);
		if (len < 0) {
			return 0;
		}
		if (strcmp(fileName + len, audios[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

int dm_key_releaseed(int code)
{
//	led_state_off();
	return 0;
}

int dm_key_pressed(int code)
{
//	led_state_on();
	return 0;
}



#if 0
void sig_handler(int signo)
{
	char cmd[64] = {};
	void *array[10];
	int size = 0;
	char **strings = NULL;
	int i = 0;
#if (DM6291_FIX == 1)
	char record_data[4096]={0};
	FILE *fp = NULL;
	FILE *fpsave = NULL;


    if(signo == MINE_VR_TEST)
    {
         mozart_snd_source_switch();
         //mozart_key_wakeup();
		 printf("MINE_VR_TEST\n");
		 return ;
	}else if(signo == MINE_VR_TEST1)
	{ 

		if(g_busy_flag == 2)
			g_busy_flag = 1;
		else
		{
			fp = fopen("/dev/dsp","r");
			fpsave = fopen("/mnt/sdcard/recodedata","wb");
			if(NULL == fp || fpsave==NULL)
			{
				printf("fopen /dev/dsp failed! fp=%d fpsave=%d\n",(NULL == fp),NULL == fpsave);
				return ;
			}

		    g_busy_flag = 2;
            while(g_busy_flag == 2)
            {
               fread( record_data, 4096, 1, fp);
			   fwrite( record_data, 4096, 1, fpsave);
		    }
			fclose(fp);
			fclose(fpsave);
			fpsave=NULL;
			fp=NULL;
		}
		return ;
	}
	
    if(g_busy_flag)
    {
        printf("[ERROR] HAVE REACH HERER!\n");
		exit(1);
	}
	else
		g_busy_flag = 1;
#endif
	printf("\n\n[%s: %d] mozart crashed by signal %s.\n", __func__, __LINE__, signal_str[signo]);

	printf("Call Trace:\n");
	size = backtrace(array, 10);
	strings = backtrace_symbols(array, size);
	if (strings) {
		for (i = 0; i < size; i++)
			printf ("  %s\n", strings[i]);
		free (strings);
	} else {
		printf("Not Found\n\n");
	}

	if (signo == SIGSEGV || signo == SIGBUS ||
	    signo == SIGTRAP || signo == SIGABRT) {
		sprintf(cmd, "cat /proc/%d/maps", getpid());
		printf("Process maps:\n");
		system(cmd);
	}

	printf("stop all services\n");
	stopall(-1);

	printf("unregister event manager\n");
	mozart_event_handler_put(keyevent_handler);
	mozart_event_handler_put(miscevent_handler);

	printf("unregister network manager\n");
	unregister_from_networkmanager();

#if !defined(DM6291_FIX) || (DM6291_FIX != 1)
	printf("unregister alarm manager\n");
	unregister_from_alarm_manager();
#endif
	share_mem_clear();
	share_mem_destory();

	exit(-1);
}
	signal(MINE_VR_TEST, sig_handler);
	signal(MINE_VR_TEST1, sig_handler);
#endif
