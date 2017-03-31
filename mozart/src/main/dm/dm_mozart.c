#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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

memory_domain get_current_domain()
{
	memory_domain domain;
	int ret = -1;

	ret = share_mem_get_active_domain(&domain);
	if (ret) {
		printf("get active domain error in %s:%s:%d, do nothing.\n",
		       __FILE__, __func__, __LINE__);
		return -1;
	}
	return domain;
}


//monitor play status.if playing , led blinks;
int check_play_status()
{
	g_playing = domain_playing();
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

static int g_touch = 0;
int get_touch()
{
	return g_touch;
}
int dm_key_releaseed(int code)
{
	g_touch = 0;
	update_led_status();
	return 0;
}

int dm_key_pressed(int code)
{
	g_touch = 1;
	update_led_status();
	return 0;
}

static int g_musicplayer_type = MUSIC_CLOUD;
int get_musicplayer_type()
{
	return g_musicplayer_type;
}
int  dm_handle_musicplayer_insert_list(struct music_info *info)
{
	if (strncmp(info->music_url, "/mnt/usb", strlen("/mnt/usb")) == 0) {
		g_musicplayer_type = MUSIC_USB;
		return 0;
	}
	if (strncmp(info->music_url, "/mnt/sdcard", strlen("/mnt/sdcard")) == 0) {
		g_musicplayer_type = MUSIC_SD;
		return 0;
	}
	g_musicplayer_type = MUSIC_CLOUD;	
	return 0;
}
int  dm_localplayer_start_insert(const char* music)
{
	if(!music) {
		printf("music is null\n");
		return -1;
	}
	if (strncmp(music, "/mnt/usb", strlen("/mnt/usb")) == 0) {
		current_play_domain_change(PM_UDISK);
	}else
		current_play_domain_change(PM_SDCARD);
	return 0;
}

//由于原来sdk逻辑中的domain和snd_source都不能用于准确指示当前播放模式，因此定义此枚举变量用于指示当前播放模式
static enum PlayMode g_current_play_mode = PM_NONE;
int current_play_domain_change(enum PlayMode new_mode)
{
	printf("playmode change, last(%d), current(%d)\n", g_current_play_mode, new_mode);
	if ( (PM_BTAVK == g_current_play_mode || PM_BTHS == g_current_play_mode) && (new_mode !=PM_BTAVK && new_mode != PM_BTHS)) {
		mozart_refresh_volume();
	}
	g_current_play_mode = new_mode;
	led_play_mode(new_mode);
	return 0;
}

enum PlayMode get_current_playmode()
{
	return g_current_play_mode;
}

int first_class_playing()
{
	enum PlayMode  mode = get_current_playmode();
	if (mode == PM_AIRPLAY ||mode == PM_RENDER || mode == PM_INGENIC || mode == PM_BTAVK || mode == PM_BTHS)
		return 1;
	else 
		return 0;
}

static int first_class_disconnect = 0;
int set_first_class_disconnect()
{
	first_class_disconnect = 1;
	return 0;
}

int get_first_class_disconnect()
{
	return first_class_disconnect;
}

int handle_first_class_disconnect()
{
	first_class_disconnect_switch_source();
	first_class_disconnect = 0;
	return 0;	
}

int in_depend_network_playmode()
{
	enum PlayMode mode = get_current_playmode();
	if (mode == PM_CLOUD || mode == PM_AIRPLAY || mode == PM_RENDER|| mode == PM_INGENIC)
		return 1;
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
