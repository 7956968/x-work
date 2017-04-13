#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <fcntl.h>
#include <execinfo.h>

#include "event_interface.h"
#include "wifi_interface.h"
#include "volume_interface.h"
#include "player_interface.h"
#include "tips_interface.h"
#include "json-c/json.h"
#include "utils_interface.h"
#include "power_interface.h"
#include "sharememory_interface.h"
#include "ini_interface.h"
#include "nvrw_interface.h"
#include "updater_interface.h"
#include "localplayer_interface.h"

#include "mozart_config.h"
#include "lapsule_control.h"
#include "battery_capacity.h"
#include "ingenicplayer.h"

#include "mozart_app.h"
#include "mozart_ui.h"
#include "mozart_musicplayer.h"
#include "mozart_key_function.h"

#ifdef SUPPORT_BT
#include "modules/mozart_module_bt.h"
#endif
#ifdef SUPPORT_VR
#include "modules/mozart_module_vr.h"
#include "vr_interface.h"
#endif

#if (SUPPORT_MULROOM == 1)
#include "mulroom_interface.h"
#include "modules/mozart_module_mulroom.h"
#endif /* SUPPORT_MULROOM == 1 */

#include "modules/mozart_module_linein.h"
#include "modules/mozart_module_local_music.h"

#if (SUPPORT_ALARM == 1)
#include "alarm_interface.h"
#endif


//DAMAI ADD
#if 1
#include "dm_mozart.h"
#endif

event_handler *keyevent_handler = NULL;
event_handler *miscevent_handler = NULL;
char *app_name = NULL;
int tfcard_status = -1;
int udisk_status = -1;
static int null_cnt = 0;

#if (SUPPORT_MULROOM == 1)
#define MULROOM_INFO_PATH	"/usr/data/mulroom.json"
static enum mul_state mulroom_state = MUL_IDLE;
#endif /* SUPPORT_MULROOM == 1 */

#if (SUPPORT_BT == BT_BCM)
#define APP_HS_CALL_VOLUME_MAX  15
#define APP_AVK_VOLUME_MAX      17
int bsa_ble_start_regular_enable = 0;
int bsa_start_regular_enable = 0;
int ble_service_create_enable = 0;
int ble_service_add_char_enable = 0;
int ble_service_start_enable = 0;
int ble_client_open_enable = 0;
int ble_client_write_enable = 0;
int ble_hh_add_dev_enable = 0;
int bt_link_state = BT_LINK_DISCONNECTED;
pthread_t bt_reconnect_pthread;
pthread_t bt_ring_pthread;
int bt_reconnect_num = 0;
bool bt_ring_flag;
int disc_num = -1;
pthread_mutex_t bt_reconnect_lock;
static UINT8 avk_volume_set_dsp[APP_AVK_VOLUME_MAX] = {0, 6, 12, 18, 25, 31, 37, 43, 50, 56, 62, 68, 75, 81, 87, 93, 100};
static UINT8 avk_volume_set_phone[APP_AVK_VOLUME_MAX] = {0, 7, 15, 23, 31, 39, 47, 55, 63, 71, 79, 87, 95, 103, 111, 119, 127};
#if (SUPPORT_BSA_BLE == 1)
extern ble_create_service_data ble_heart_rate_service;
extern ble_add_char_data ble_heart_rate_measure_char;
extern ble_add_char_data ble_notify_char;
#endif /* SUPPORT_BSA_BLE */

#if (SUPPORT_BSA_A2DP_SOURCE == 1)
extern int avk_source_set_audio_output();
#endif /* SUPPORT_BSA_A2DP_SOURCE */

#if (SUPPORT_BSA_PBC == 1)
enum {
	TEL_ICH_PATH,		/* Path to local incoming calls history object */
	TEL_OCH_PATH,		/* Path to local outgoing calls history object */
	TEL_MCH_PATH,		/* Path to local missed calls history object */
	TEL_CCH_PATH,		/* Path to local combined calls history object */
	TEL_PB_PATH,		/* Path to local main phone book object */
	SIM1_TEL_ICH_PATH,	/* Path to SIM Card incoming calls history object */
	SIM1_TEL_OCH_PATH,	/* Path to SIM Card outgoing calls history object */
	SIM1_TEL_MCH_PATH,	/* Path to SIM Card missed calls history object */
	SIM1_TEL_CCH_PATH,	/* Path to SIM Card combined calls history object */
	SIM1_TEL_PB_PATH,	/* Path to SIM Card main phone book object */
};

static char *tel_str[] = {
	[TEL_ICH_PATH] 	     = "telecom/ich.vcf",       [TEL_OCH_PATH]       = "telecom/och.vcf",
	[TEL_MCH_PATH] 	     = "telecom/mch.vcf", 	[TEL_CCH_PATH]       = "telecom/cch.vcf",
	[TEL_PB_PATH] 	     = "telecom/pb.vcf",        [SIM1_TEL_ICH_PATH]  = "SIM1/telecom/ich.vcf",
	[SIM1_TEL_OCH_PATH]  = "SIM1/telecom/och.vcf",  [SIM1_TEL_MCH_PATH]  = "SIM1/telecom/mch.vcf",
	[SIM1_TEL_CCH_PATH]  = "SIM1/telecom/cch.vcf",  [SIM1_TEL_PB_PATH]   = "SIM1/telecom/pb.vcf",
};
#endif /* SUPPORT_BSA_PBC */
#endif /* SUPPORT_BT */

static char *signal_str[] = {
	[1] = "SIGHUP",       [2] = "SIGINT",       [3] = "SIGQUIT",      [4] = "SIGILL",      [5] = "SIGTRAP",
	[6] = "SIGABRT",      [7] = "SIGBUS",       [8] = "SIGFPE",       [9] = "SIGKILL",     [10] = "SIGUSR1",
	[11] = "SIGSEGV",     [12] = "SIGUSR2",     [13] = "SIGPIPE",     [14] = "SIGALRM",    [15] = "SIGTERM",
	[16] = "SIGSTKFLT",   [17] = "SIGCHLD",     [18] = "SIGCONT",     [19] = "SIGSTOP",    [20] = "SIGTSTP",
	[21] = "SIGTTIN",     [22] = "SIGTTOU",     [23] = "SIGURG",      [24] = "SIGXCPU",    [25] = "SIGXFSZ",
	[26] = "SIGVTALRM",   [27] = "SIGPROF",     [28] = "SIGWINCH",    [29] = "SIGIO",      [30] = "SIGPWR",
	[31] = "SIGSYS",      [34] = "SIGRTMIN",    [35] = "SIGRTMIN+1",  [36] = "SIGRTMIN+2", [37] = "SIGRTMIN+3",
	[38] = "SIGRTMIN+4",  [39] = "SIGRTMIN+5",  [40] = "SIGRTMIN+6",  [41] = "SIGRTMIN+7", [42] = "SIGRTMIN+8",
	[43] = "SIGRTMIN+9",  [44] = "SIGRTMIN+10", [45] = "SIGRTMIN+11", [46] = "SIGRTMIN+12", [47] = "SIGRTMIN+13",
	[48] = "SIGRTMIN+14", [49] = "SIGRTMIN+15", [50] = "SIGRTMAX-14", [51] = "SIGRTMAX-13", [52] = "SIGRTMAX-12",
	[53] = "SIGRTMAX-11", [54] = "SIGRTMAX-10", [55] = "SIGRTMAX-9",  [56] = "SIGRTMAX-8", [57] = "SIGRTMAX-7",
	[58] = "SIGRTMAX-6",  [59] = "SIGRTMAX-5",  [60] = "SIGRTMAX-4",  [61] = "SIGRTMAX-3", [62] = "SIGRTMAX-2",
	[63] = "SIGRTMAX-1",  [64] = "SIGRTMAX",
};

static void usage(const char *app_name)
{
	printf("%s [-bsh] \n"
		   " -h     help (show this usage text)\n"
		   " -s/-S  TODO\n"
		   " -b/-B  run a daemon in the background\n", app_name);

	return;
}

char buf[16] = {};

//DM6291_FIX
#include "dm_network.h"
#include "dm6291_led.h"
void sig_handler(int signo)
{
	char cmd[64] = {};
	void *array[10];
	int size = 0;
	char **strings = NULL;
	int i = 0;

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

#if (SUPPORT_ALARM == 1)
	printf("unregister alarm manager\n");
	unregister_to_alarm_manager();
#endif

	share_mem_clear();
	share_mem_destory();

	exit(-1);
}

#ifdef SUPPORT_VR
#if (SUPPORT_VR_WAKEUP == VR_WAKEUP_KEY_LONGPRESS)
int vr_flag = 0;
void *vr_longbutton_func(void *arg)
{
	vr_flag = 1;
	mozart_vr_wakeup();
	vr_flag = 0;

	return NULL; // make compile happy.
}

int create_vr_longbutton_pthread()
{
	int ret = 0;
	pthread_t vr_longbutton_thread = 0;

	if(vr_flag == 0) {
		ret = pthread_create(&vr_longbutton_thread, NULL, vr_longbutton_func, NULL);
		if(ret != 0) {
			printf ("Create iflytek pthread failed: %s!\n", strerror(errno));
			return ret;
		}
		pthread_detach(vr_longbutton_thread);
	}

	return ret;
}
#endif
#endif

bool wifi_configing = false;
pthread_t wifi_config_pthread;
pthread_mutex_t wifi_config_lock = PTHREAD_MUTEX_INITIALIZER;

void *wifi_config_func(void *args)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	wifi_configing = true;

	mozart_module_stop();
	stopall(1);
	mozart_config_wifi();

	wifi_configing = false;

	return NULL; // make compile happy.
}

int create_wifi_config_pthread(void)
{
	pthread_mutex_lock(&wifi_config_lock);
	// cancle previous thread.
	if (wifi_configing) {
		pthread_cancel(wifi_config_pthread);
	}
	// enter wifi config mode.
	if (pthread_create(&wifi_config_pthread, NULL, wifi_config_func, NULL) == -1) {
		printf("Create wifi config pthread failed: %s.\n", strerror(errno));
		pthread_mutex_unlock(&wifi_config_lock);
		return -1;
	}
	pthread_detach(wifi_config_pthread);

	pthread_mutex_unlock(&wifi_config_lock);

	return 0;
}

bool wifi_switching = false;
pthread_t wifi_switch_pthread;
pthread_mutex_t wifi_switch_lock = PTHREAD_MUTEX_INITIALIZER;

void *wifi_switch_func(void *args)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	wifi_switching = true;

	mozart_module_stop();
	stopall(1);
	mozart_wifi_mode();

	wifi_switching = false;

	return NULL; // make compile happy.
}

int create_wifi_switch_pthread(void)
{
	pthread_mutex_lock(&wifi_switch_lock);
	// cancle previous thread.
	if (wifi_switching)
		pthread_cancel(wifi_switch_pthread);

	// enter wifi switch mode.
	if (pthread_create(&wifi_switch_pthread, NULL, wifi_switch_func, NULL) == -1) {
		printf("Create wifi switch pthread failed: %s.\n", strerror(errno));
		pthread_mutex_unlock(&wifi_switch_lock);
		return -1;
	}
	pthread_detach(wifi_switch_pthread);

	pthread_mutex_unlock(&wifi_switch_lock);

	return 0;
}

#if (SUPPORT_LOCALPLAYER == 1)
void tfcard_scan_1music_callback(void)
{
#if 0
	tfcard_status = 1;
	mozart_module_local_music_startup();
#endif
}

void tfcard_scan_done_callback(char *musiclist)
{
#if 1
	printf("tfcard_scan_done_callback musiclist:%s\n", musiclist);
	/* TODO; musicplayer add queue */
	//hzb disable this, because maybe udisk
	if (musiclist) {
		if( (strstr(musiclist, "\\/mnt\\/sdcard\\/")) != NULL) {
			printf("find music in sdcard\n");
			tfcard_status = 1;
		}
		else if( (strstr(musiclist, "\\/mnt\\/usb\\/")) != NULL) {
			printf("find music in udisk\n");
			udisk_status = 1;
		}
	}
	//tfcard_status = 1;
	mozart_module_local_music_startup();
#endif
}
#endif	/* SUPPORT_LOCALPLAYER */

static void key_bluetooth_handler(void)
{
#if (SUPPORT_BT == BT_BCM)
	int state = 0;

	state = mozart_bluetooth_hs_get_call_state();
	if (state == CALLSETUP_STATE_INCOMMING_CALL) {
		printf(">>>>>>>>>>>>>answer call>>>>>\n");
		mozart_bluetooth_hs_answer_call();
	} else if (state == CALL_STATE_LEAST_ONE_CALL_ONGOING ||
			   state == CALLSETUP_STATE_OUTGOING_CALL ||
			   state == CALLSETUP_STATE_REMOTE_BEING_ALERTED_IN_OUTGOING_CALL ||
			   state == CALLHELD_STATE_NO_CALL_HELD) {
		printf(">>>>>>>>>>>>>hang up>>>>>\n");
		mozart_bluetooth_hs_hangup_call();
	} else if (state == CALLSETUP_STATE_WAITING_CALL) {
		/* you can do other operation */
		mozart_bluetooth_hs_hold_call(BTHF_CHLD_TYPE_HOLDACTIVE_ACCEPTHELD);
	} else if (state == CALLHELD_STATE_PLACED_ON_HOLD_OR_SWAPPED) {
		/* you can do other operation */
		mozart_bluetooth_hs_hold_call(BTHF_CHLD_TYPE_RELEASEACTIVE_ACCEPTHELD);
	} else {
		printf("not support state : %d\n", state);
	}
#endif
}

#if (SUPPORT_BT == BT_BCM)
static void *bt_auto_reconnect_pthread(void *args)
{
	/* auto reconnect time:
	 * 10 minute */
	int ret = 0;

	pthread_mutex_lock(&bt_reconnect_lock);
	bt_reconnect_num = 10;
	while (bt_reconnect_num > 0) {
		if (BT_LINK_DISCONNECTING == mozart_bluetooth_get_link_status() ||
			BT_LINK_CONNECTING == mozart_bluetooth_get_link_status() ||
			BT_LINK_DISCONNECTED == mozart_bluetooth_get_link_status()) {
			printf("bt_auto_reconnect_pthread start!\n");
			ret = mozart_bluetooth_auto_reconnect(USE_HS_AVK, 0);
			printf("ret = %d\n", ret);
			if (ret == 0) {
				printf("reconnect successful!\n");
				bt_reconnect_num = 0;
			} else if (ret == -1) {
				printf("reconnect failed, retry!\n");
				bt_reconnect_num--;
			} else if (ret == -2) {
				printf("bt_reconnect_devices.xml not existed, no connected device\n");
				bt_reconnect_num = 0;
			} else {
				printf("Not supported this type: %d\n", ret);
			}
		} else if (BT_LINK_CONNECTED == mozart_bluetooth_get_link_status()) {
			printf("bt link status == BT_LINK_CONNECTED !\n");
			bt_reconnect_num = 0;
		}
	}

	bt_reconnect_num = 0;
	pthread_mutex_unlock(&bt_reconnect_lock);
	printf("return bt_auto_reconnect_pthread !\n");

	return NULL;
}

int bluetooth_create_auto_reconnect_pthread()
{
	if (pthread_create(&bt_reconnect_pthread, NULL, bt_auto_reconnect_pthread, NULL) != 0) {
		printf("create bt_auto_reconnect_pthread failed !\n");
		return -1;
	}
	pthread_detach(bt_reconnect_pthread);
	return 0;
}

int bluetooth_cancel_auto_reconnect_pthread()
{
	bt_reconnect_num = 0;
	return 0;
}

static void *bluetooth_ring_pthread(void *args)
{
	bt_ring_flag = true;
	while (bt_ring_flag) {
		printf("mozart_play_key: bluetooth_ring\n");
		mozart_play_key_sync("bluetooth_ring");
		sleep(1);
	}

	return NULL;
}
int bluetooth_create_ring_pthread()
{
	if (pthread_create(&bt_ring_pthread, NULL, bluetooth_ring_pthread, NULL) != 0) {
		printf("create bt_ring_pthread failed !\n");
		return -1;
	}
	pthread_detach(bt_ring_pthread);
	return 0;
}

int bluetooth_cancel_ring_pthread()
{
	printf("bluetooth_cancel_ring_pthread !\n");
	bt_ring_flag = false;

	return mozart_stop_tone();
}
#endif

/*******************************************************************************
 * long press
 *******************************************************************************/
enum key_long_press_state {
	KEY_LONG_PRESS_INVALID = 0,
	KEY_LONG_PRESS_WAIT,
	KEY_LONG_PRESS_CANCEL,
	KEY_LONG_PRESS_DONE,
};

struct input_event_key_info {
	char *name;
	int key_code;
	enum key_long_press_state state;
	pthread_t pthread;
	pthread_mutex_t lock;
	pthread_cond_t cond;
	int timeout_second;
	void (*handler)(bool long_press);
};
//HZB
#if 0
static void wifi_switch_handler(bool long_press)
{
	if (long_press)
		create_wifi_config_pthread();
	else
		create_wifi_switch_pthread();
}
#endif


static void volume_down_handler(bool long_press)
{
	if (long_press)
		mozart_previous_song();
	else
		mozart_volume_down();
}

static void volume_up_handler(bool long_press)
{
	if (long_press)
		mozart_next_song();
	else
		mozart_volume_up();
}
#if 0
static void play_pause_handler(bool long_press)
{
	if (long_press)
		key_bluetooth_handler();
	else
		mozart_play_pause();
}
#endif
static struct input_event_key_info input_event_key_infos[] = {
	//HZB
	#if 0
	{
		.name = "wifi_switch_key",
		.key_code = KEY_MODE,
		.lock = PTHREAD_MUTEX_INITIALIZER,
		.cond = PTHREAD_COND_INITIALIZER,
		.timeout_second = 1,
		.handler = wifi_switch_handler,
	},
	#endif
	{
		.name = "volume_up_key",
		.key_code = KEY_VOLUMEUP,
		.lock = PTHREAD_MUTEX_INITIALIZER,
		.cond = PTHREAD_COND_INITIALIZER,
		.timeout_second = 1,
		.handler = volume_up_handler,
	},
	{
		.name = "volume_down_key",
		.key_code = KEY_VOLUMEDOWN,
		.lock = PTHREAD_MUTEX_INITIALIZER,
		.cond = PTHREAD_COND_INITIALIZER,
		.timeout_second = 1,
		.handler = volume_down_handler,
	},
	/*
	{
		.name = "play_pause_key",
		.key_code = KEY_PLAYPAUSE,
		.lock = PTHREAD_MUTEX_INITIALIZER,
		.cond = PTHREAD_COND_INITIALIZER,
		.timeout_second = 1,
		.handler = play_pause_handler,
	},*/
};

static void *key_long_press_func(void *args)
{
	struct timeval now;
	struct timespec timeout;
	struct input_event_key_info *info = (struct input_event_key_info *)args;
	
	dm_key_pressed(info->key_code);

	pthread_mutex_lock(&info->lock);
	if (info->state == KEY_LONG_PRESS_CANCEL) {
		info->state = KEY_LONG_PRESS_INVALID;
		printf("%s short press\n", info->name);
		if (info->handler)
			info->handler(false);
		pthread_mutex_unlock(&info->lock);
		dm_key_releaseed(info->key_code);
		return NULL;
	}

	gettimeofday(&now, NULL);
	timeout.tv_sec = now.tv_sec + info->timeout_second;
	timeout.tv_nsec = now.tv_usec * 1000;

	pthread_cond_timedwait(&info->cond,
						   &info->lock, &timeout);

	if (info->state == KEY_LONG_PRESS_CANCEL) {
		info->state = KEY_LONG_PRESS_INVALID;
		printf("%s short press\n", info->name);
		if (info->handler)
			info->handler(false);
		pthread_mutex_unlock(&info->lock);
		dm_key_releaseed(info->key_code);
		return NULL;
	}

	info->state = KEY_LONG_PRESS_DONE;

	printf("%s long press\n", info->name);
	if (info->handler)
		info->handler(true);
	pthread_mutex_unlock(&info->lock);
	
	dm_key_releaseed(info->key_code);

	return NULL;
}

static void create_key_long_press_pthread(struct input_event_key_info *info)
{
	pthread_mutex_lock(&info->lock);
	if (info->state != KEY_LONG_PRESS_INVALID) {
		pthread_mutex_unlock(&info->lock);
		return ;
	}
	info->state = KEY_LONG_PRESS_WAIT;
	pthread_mutex_unlock(&info->lock);

	if (pthread_create(&info->pthread, NULL, key_long_press_func, (void *)info) == -1) {
		printf("Create key long press pthread failed: %s.\n", strerror(errno));
		return ;
	}
	pthread_detach(info->pthread);
}

static void key_long_press_cancel(struct input_event_key_info *info)
{
	pthread_mutex_lock(&info->lock);

	if (info->state == KEY_LONG_PRESS_DONE) {
		info->state = KEY_LONG_PRESS_INVALID;
	} else if (info->state == KEY_LONG_PRESS_WAIT) {
		info->state = KEY_LONG_PRESS_CANCEL;
		pthread_cond_signal(&info->cond);
	}

	pthread_mutex_unlock(&info->lock);
}

static int input_event_handler(struct input_event event)
{
	int i;
	struct input_event_key_info *info;

	for (i = 0; i < sizeof(input_event_key_infos) / sizeof(struct input_event_key_info); i++) {
		info = &input_event_key_infos[i];

		if (event.code == info->key_code) {
			if (event.value == 1)
				create_key_long_press_pthread(info);
			else
				key_long_press_cancel(info);

			return 0;
		}
	}

	return -1;
}

void keyevent_callback(mozart_event event, void *param)
{
	switch (event.type) {
	case EVENT_KEY:
		if (event.event.key.key.type != EV_KEY) {
			printf("Only support keyboard now.\n");
			break;
		}
		printf("[DEBUG] key %s %s!!!\n",
			   keycode_str[event.event.key.key.code], keyvalue_str[event.event.key.key.value]);
		if (input_event_handler(event.event.key.key) == 0)
			break;

		if (event.event.key.key.value == 1) {
			//HZB add
			dm_key_pressed(event.event.key.key.code);
			
			switch (event.event.key.key.code)
			{
			case KEY_RECORD:
#ifdef SUPPORT_VR
#if (SUPPORT_VR_WAKEUP == VR_WAKEUP_KEY_SHORTPRESS || SUPPORT_VR_WAKEUP == VR_WAKEUP_VOICE_KEY_MIX)
				if (mozart_vr_get_status() == VR_AEC) {
					printf("AEC mode, wakeup\n");
					mozart_vr_aec_wakeup();
				} else if (mozart_vr_get_status() == VR_ASR) {
					printf("ASR mode, interrupt\n");
					mozart_vr_asr_break();
				}
#elif (SUPPORT_VR_WAKEUP == VR_WAKEUP_KEY_LONGPRESS)
				if(create_vr_longbutton_pthread()) {  // in case of block.
					printf("create_iflytek_pthread failed!\n");
					break;
				}
#endif
#endif
				break;
			case KEY_F1:
				create_wifi_config_pthread(); // in case of block.
				break;
			case KEY_MODE:
				create_wifi_switch_pthread();
				break;
			case KEY_PLAYPAUSE:
				mozart_play_pause();
				break;
			case KEY_BLUETOOTH:
				key_bluetooth_handler();
				break;
			case KEY_PREVIOUSSONG:
				mozart_previous_song();
				break;
			case KEY_NEXTSONG:
				mozart_next_song();
				break;
			case KEY_MENU:
				if (first_class_playing()) 
					set_first_class_disconnect();
				else
					set_switch_souce_snd();
				break;
			case KEY_F3:            /* music music Shortcut key 1 */
				//HZB comment this statement because it will cause dead lock
				//mozart_music_list(0);
				break;
			case KEY_F4:            /* music music Shortcut key 2 */
				mozart_music_list(1);
				break;
			case KEY_F5:            /* music music Shortcut key 3 */
				mozart_music_list(2);
				break;
			case KEY_POWER:
				mozart_power_off();
				break;
			default:
				//printf("UnSupport key down in %s:%s:%d.\n", __FILE__, __func__, __LINE__);
				break;
			}
		} else {
			//HZB add
			dm_key_releaseed(event.event.key.key.code);
			switch (event.event.key.key.code) {
			case KEY_RECORD:
				/*TODO: add VR_WAKEUP_KEY_LONGPRESS support*/
				break;
			default:
				break;
			}
		}
		break;
	case EVENT_MISC:
	default:
		break;
	}

	return;
}

void miscevent_callback(mozart_event event, void *param)
{
	switch (event.type) {
	case EVENT_MISC:
		printf("[misc event] %s : %s.\n", event.event.misc.name, event.event.misc.type);
		if (!strcasecmp(event.event.misc.name, "linein")
#if (SUPPORT_MULROOM == 1)
			&& mulroom_state != MUL_GROUP_RECEIVER
#endif /* SUPPORT_MULROOM == 1 */
			) {
			if (!strcasecmp(event.event.misc.type, "plugin")) { // linein plugin
				mozart_ui_linein_plugin();
				mozart_module_stop();
				mozart_linein_on();
				lapsule_do_linein_on();
			} else if (!strcasecmp(event.event.misc.type, "plugout")) { // linein plugout
				mozart_ui_linein_plugout();
				mozart_linein_off();
				lapsule_do_linein_off();
			}
		} else if (!strcasecmp(event.event.misc.name, "udisk")) {
			if (!strcasecmp(event.event.misc.type, "plugin")) { // plugin
				if (udisk_status != 1) {
					mozart_ui_localplayer_plugin();
					mozart_play_key("tf_add");
					udisk_status = 1;
				} else {
					// do nothing.
				}
			} else if (!strcasecmp(event.event.misc.type, "plugout")) { // tfcard plugout
				if (udisk_status != 0) {
					mozart_ui_localplayer_plugout();
					mozart_play_key("tf_remove");
#if (SUPPORT_LOCALPLAYER == 1)
					if (get_current_playmode() == PM_UDISK)
						set_switch_souce_snd();
#endif
					udisk_status = 0;
				}
			}
		} else if (!strcasecmp(event.event.misc.name, "tfcard")) {
			if (!strcasecmp(event.event.misc.type, "plugin")) { // tfcard plugin
				if (tfcard_status != 1) {
					mozart_ui_localplayer_plugin();
					mozart_play_key("tf_add");
					tfcard_status = 1;
				} else {
					// do nothing.
				}
			} else if (!strcasecmp(event.event.misc.type, "plugout")) { // tfcard plugout
				if (tfcard_status != 0) {
					mozart_play_key("tf_remove");
					mozart_ui_localplayer_plugout();
#if (SUPPORT_LOCALPLAYER == 1)
					if (get_current_playmode() == PM_SDCARD) {
						set_switch_souce_snd();
					}
#endif
					tfcard_status = 0;
				}
			}
		} else if (!strcasecmp(event.event.misc.name, "headset")) {
			if (!strcasecmp(event.event.misc.type, "plugin")) { // headset plugin
				printf("headset plugin.\n");
				// do nothing.
			} else if (!strcasecmp(event.event.misc.type, "plugout")) { // headset plugout
				printf("headset plugout.\n");
				// do nothing.
			}
		} else if (!strcasecmp(event.event.misc.name, "spdif")) {
			if (!strcasecmp(event.event.misc.type, "plugin")) { // spdif-out plugin
				printf("spdif plugin.\n");
				// do nothing.
			} else if (!strcasecmp(event.event.misc.type, "plugout")) { // spdif-out plugout
				printf("spdif plugout.\n");
				// do nothing.
			}
		} else if (!strcasecmp(event.event.misc.name, "volume")) {
			if (!strcasecmp(event.event.misc.type, "update music")) {
				printf("%s volume to %d.\n", event.event.misc.type, event.event.misc.value[0]);
			} else if (!strcasecmp(event.event.misc.type, "update bt_music")) {
				printf("%s volume to %d.\n", event.event.misc.type, event.event.misc.value[0]);
			} else if (!strcasecmp(event.event.misc.type, "update bt_call")) {
				printf("%s volume to %d.\n", event.event.misc.type, event.event.misc.value[0]);
			} else if (!strcasecmp(event.event.misc.type, "update unknown")) {
				printf("%s volume to %d.\n", event.event.misc.type, event.event.misc.value[0]);
			} else {
				printf("unhandle volume event: %s.\n", event.event.misc.type);
			}
#if (SUPPORT_BT == BT_BCM)
		} else if (!strcasecmp(event.event.misc.name, "bluetooth")) {
			if (!strcasecmp(event.event.misc.type, "connecting")) {
				; // do nothing if bluetooth device is connecting.
			} else if (!strcasecmp(event.event.misc.type, "disc_new")) {
			} else if (!strcasecmp(event.event.misc.type, "disc_complete")) {
				bsa_start_regular_enable = 1;
			} else if (!strcasecmp(event.event.misc.type, "sec_link_down")) {
				/*
				 * Reason code 0x13: Mobile terminate bluetooth connection
				 * Reason code 0x8: connection timeout(beyond distance)
				 */
				UINT8 link_down_data = 0;
				link_down_data = mozart_bluetooth_get_link_down_status();
				if (link_down_data == 0x13) {
					printf("Mobile terminate bluetooth connection.\n");
				} else if (link_down_data == 0x8) {
					printf("BT connection timeout(beyond distance maybe)!\n");
					int state = 0;
					state = bluetooth_create_auto_reconnect_pthread();
					if (state == 0) {
						printf("bluetooth_create_auto_reconnect_pthread success !\n");
					} else if (state == -1) {
						printf("bluetooth_create_auto_reconnect_pthread failed !\n");
					}
				} else if (link_down_data == 0x16) {
					printf("connection terminated by local host!\n");
				} else {
					printf("not handle link_down_data %d, Ignore.\n", link_down_data);
				}
			} else {
				printf("unhandle bluetooth event: %s.\n", event.event.misc.type);
			}
		} else if (!strcasecmp(event.event.misc.name, "bt_hs")) {
			if (!strcasecmp(event.event.misc.type, "connected")) {
				if (mozart_bluetooth_get_link_status() == BT_LINK_CONNECTED) {
					if (bt_link_state == BT_LINK_DISCONNECTED) {
						bt_link_state = BT_LINK_CONNECTED;
						mozart_ui_bt_connected();
						mozart_bluetooth_set_visibility(0, 0);
						//mozart_module_stop();
						/* FIXME: do not stop if music playing. will skip tone. */
						mozart_play_key_sync("bluetooth_connect");
						/* snd_source = SND_SRC_BT_AVK; */
					}
#if (SUPPORT_BSA_PBC == 1)
					mozart_bluetooth_pbc_open_connection();
#endif
				}
			} else if (!strcasecmp(event.event.misc.type, "disconnected")) {
				if (mozart_bluetooth_get_link_status() == BT_LINK_DISCONNECTED) {
					if (bt_link_state == BT_LINK_CONNECTED) {
						bt_link_state = BT_LINK_DISCONNECTED;
						mozart_play_key("bluetooth_disconnect");
						mozart_bluetooth_set_visibility(1, 1);
						mozart_ui_bt_disconnected();
					}
#if (SUPPORT_BSA_PBC == 1)
					mozart_bluetooth_pbc_close_connection();
#endif
				}
				if (get_current_playmode() == PM_BTHS)
					set_first_class_disconnect();
			} else if (!strcasecmp(event.event.misc.type, "ring")) {

#ifdef SUPPORT_VR
				if (mozart_vr_get_status()) {
					printf("invoking mozart_vr_stop()\n");
					mozart_vr_stop();
				}
#endif
				//TODO: should pause here, resume music after hangup.
				if (mozart_module_stop()) {
					printf("can not ring.\n");
				} else {
					bluetooth_create_ring_pthread();
					snd_source = SND_SRC_BT_AVK;
				}
			} else if (!strcasecmp(event.event.misc.type, "hangup")) {
				bluetooth_cancel_ring_pthread();
#ifdef SUPPORT_VR
				if(!mozart_vr_get_status()) {
					printf("invoking mozart_vr_start()\n");
					mozart_vr_start();
				}
#endif
			} else if (!strcasecmp(event.event.misc.type, "call")) {
				bluetooth_cancel_ring_pthread();
#ifdef SUPPORT_VR
				if (mozart_vr_get_status()) {
					printf("invoking mozart_vr_stop()\n");
					mozart_vr_stop();
				}
#endif
				//TODO: should pause here, resume music after hangup.
				if (mozart_module_stop()) {
					share_mem_set(BT_HS_DOMAIN, RESPONSE_CANCEL);
				} else {
					current_play_domain_change(PM_BTHS);
					share_mem_set(BT_HS_DOMAIN, RESPONSE_DONE);
					snd_source = SND_SRC_BT_AVK;
				}
			} else if (!strcasecmp(event.event.misc.type, "close")) {
			} else if (!strcasecmp(event.event.misc.type, "vgs")) {
				int volume = event.event.misc.value[0];

				mozart_volume_set(volume * 100 / APP_HS_CALL_VOLUME_MAX, BT_CALL_VOLUME);
				printf("phone volume: %d, mozart set hs volume: %d\n",
					   volume,
					   volume * 100 / APP_HS_CALL_VOLUME_MAX);
			} else {
				printf("unhandle bt phone event: %s.\n", event.event.misc.type);
			}
		} else if (!strcasecmp(event.event.misc.name, "bt_avk")) {
			if (!strcasecmp(event.event.misc.type, "connected")) {
				if (mozart_bluetooth_get_link_status() == BT_LINK_CONNECTED) {
					if (bt_link_state == BT_LINK_DISCONNECTED) {
						bt_link_state = BT_LINK_CONNECTED;
						mozart_ui_bt_connected();
						mozart_bluetooth_set_visibility(0, 0);
						//mozart_module_stop();
						/* FIXME: do not stop if music playing. will skip tone. */
						mozart_play_key_sync("bluetooth_connect");
					}
				}
			} else if (!strcasecmp(event.event.misc.type, "disconnected")) {
				if (mozart_bluetooth_get_link_status() == BT_LINK_DISCONNECTED) {
					if (bt_link_state == BT_LINK_CONNECTED) {
						bt_link_state = BT_LINK_DISCONNECTED;
						mozart_play_key("bluetooth_disconnect");
						mozart_bluetooth_set_visibility(1, 1);
						mozart_ui_bt_disconnected();
					}
				}
				if (get_current_playmode() == PM_BTAVK)
					set_first_class_disconnect();
			} else if (!strcasecmp(event.event.misc.type, "play")) {
				if (mozart_module_stop()) {
					share_mem_set(BT_AVK_DOMAIN, RESPONSE_CANCEL);
				} else {
					current_play_domain_change(PM_BTAVK);
					share_mem_set(BT_AVK_DOMAIN, RESPONSE_DONE);
					snd_source = SND_SRC_BT_AVK;
				}
			} else if (!strcasecmp(event.event.misc.type, "pause")) {
				printf("bt music player paused(or stopped).\n");
			} else if (!strcasecmp(event.event.misc.type, "set_abs_vol")) {
				int index = 0;
				int volume = 0;
				int fd = 0;

				volume = event.event.misc.value[0];
				fd = event.event.misc.value[1];
				for (index = (APP_AVK_VOLUME_MAX -1); index >= 0; index--) {
					if (avk_volume_set_phone[index] <= volume)
						break;
				}
				if (index < 0) {
					printf("failed to get music volume %d from avk_volume_set_dsp\n", volume);
					break;
				}
				if (AUDIO_OSS == get_audio_type()) {
					/* Note: add app_avk_cb.fd judged, because when play tts,
					 * we reviced ABS_VOL_CMD_EVT, and set volume to dsp,
					 * it will generate a case of sound mutation */
					if (fd != -1) {
						mozart_volume_set(avk_volume_set_dsp[index], BT_MUSIC_VOLUME);
					} else {
						char vol[8] = {};
						sprintf(vol, "%d", avk_volume_set_dsp[index]);
						if (mozart_ini_setkey("/usr/data/system.ini", "volume", "bt_music", vol)) {
							printf("save volume to /usr/data/system.ini error.\n");
							break;
						}
					}
				} else if (AUDIO_ALSA == get_audio_type()) {
					mozart_volume_set(avk_volume_set_dsp[index], BT_MUSIC_VOLUME);
				} else {
					printf("Not support audio type: %d!!\n", get_audio_type());
				}
				printf("phone volume: 0x%x, mozart set avk volume: %d\n",
						volume,
						avk_volume_set_dsp[index]);
			} else if (!strcasecmp(event.event.misc.type, "get_elem_attr")) {
				int i = 0;
				int attr_id = 0;
				tBSA_AVK_GET_ELEMENT_ATTR_MSG *p_data = NULL;
				p_data = mozart_bluetooth_avk_get_element_att();
				printf("p_data->num_attr = %d\n", p_data->num_attr);
				printf("p_data->status = %d\n", p_data->status);
				for (i = 0; i < p_data->num_attr; i++) {
					attr_id = p_data->attr_entry[i].attr_id;
					if (attr_id == AVRC_MEDIA_ATTR_ID_TITLE) {
						printf("music Title: %s\n", p_data->attr_entry[i].name.data);
					} else if (attr_id == AVRC_MEDIA_ATTR_ID_ARTIST) {
						printf("music Artist Name: %s\n", p_data->attr_entry[i].name.data);
					} else if (attr_id == AVRC_MEDIA_ATTR_ID_ALBUM) {
						printf("music Album Name: %s\n", p_data->attr_entry[i].name.data);
					} else if (attr_id == AVRC_MEDIA_ATTR_ID_TRACK_NUM) {
						printf("music Track Number: %s\n", p_data->attr_entry[i].name.data);
					} else if (attr_id == AVRC_MEDIA_ATTR_ID_NUM_TRACKS) {
						printf("music Total Number of Tracks: %s\n", p_data->attr_entry[i].name.data);
					} else if (attr_id == AVRC_MEDIA_ATTR_ID_GENRE) {
						printf("music Genre: %s\n", p_data->attr_entry[i].name.data);
					} else if (attr_id == AVRC_MEDIA_ATTR_ID_PLAYING_TIME) {
						printf("music Playing Time: %s\n", p_data->attr_entry[i].name.data);
					}
				}
			} else if (!strcasecmp(event.event.misc.type, "play_status")) {
				tBSA_AVK_GET_PLAY_STATUS_MSG *play_status_msg;
				play_status_msg = mozart_bluetooth_avk_get_play_status();
				printf("play status : %d\n", play_status_msg->play_status);
				printf("play song len : %d\n", play_status_msg->song_len);
				printf("play song pos : %d\n", play_status_msg->song_pos);
			} else {
				printf("unhandle bt music event: %s.\n", event.event.misc.type);
			}
		} else if (!strcasecmp(event.event.misc.name, "bt_av")) {
			if (!strcasecmp(event.event.misc.type, "connected")) {
#if (SUPPORT_BSA_A2DP_SOURCE == 1)
				avk_source_set_audio_output();
#endif
			} else if (!strcasecmp(event.event.misc.type, "disconnected")) {
			} else if (!strcasecmp(event.event.misc.type, "play")) {
			} else if (!strcasecmp(event.event.misc.type, "pause")) {
			} else {
				printf("unhandle bt music event: %s.\n", event.event.misc.type);
			}
		} else if (!strcasecmp(event.event.misc.name, "bt_ble")) {
			if (!strcasecmp(event.event.misc.type, "disc_new")) {
			} else if (!strcasecmp(event.event.misc.type, "disc_complete")) {
				bsa_ble_start_regular_enable = 1;
#if (SUPPORT_BSA_BLE_HH == 0)
				int index;
				tAPP_DISCOVERY_CB *app_discovery_cb = NULL;
				app_discovery_cb = mozart_bluetooth_disc_get_device_info();
				for (index = 0; index < APP_DISC_NB_DEVICES; index++) {
					if (app_discovery_cb->devs[index].in_use != FALSE) {
						printf("Dev: %d\n", index);
						printf("\tBdaddr:%02x:%02x:%02x:%02x:%02x:%02x \n",
								app_discovery_cb->devs[index].device.bd_addr[0],
								app_discovery_cb->devs[index].device.bd_addr[1],
								app_discovery_cb->devs[index].device.bd_addr[2],
								app_discovery_cb->devs[index].device.bd_addr[3],
								app_discovery_cb->devs[index].device.bd_addr[4],
								app_discovery_cb->devs[index].device.bd_addr[5]);
						printf("\tName: %s\n", app_discovery_cb->devs[index].device.name);
						char *ble_name = (char *)app_discovery_cb->devs[index].device.name;
						if (!strcmp(ble_name, "BSA_BLE"))
						{
							bsa_manu_data manu_data;
							int index = 0;
							UINT8 * eir_data = app_discovery_cb->devs[index].device.eir_data;
							mozart_bluetooth_parse_eir_manuf_specific(eir_data, &manu_data);
							printf("manu_data.data_length = %d\n", manu_data.data_length);
							printf("manu_data.company_id = 0x%02x\n", manu_data.company_id);
							printf("manu_data: \n");
							for (index = 0; index < manu_data.data_length; index++)
							{
								printf("%x ", manu_data.p_manu[index]);
							}
							printf("\n");
						}
						printf("\tClassOfDevice: %02x:%02x:%02x => %s\n",
							   app_discovery_cb->devs[index].device.class_of_device[0],
							   app_discovery_cb->devs[index].device.class_of_device[1],
							   app_discovery_cb->devs[index].device.class_of_device[2],
							   mozart_bluetooth_get_cod_string(app_discovery_cb->devs[index].device.class_of_device));

						printf("\tRssi: %d\n", app_discovery_cb->devs[index].device.rssi);
						if (app_discovery_cb->devs[index].device.eir_vid_pid[0].valid) {
							printf("\tVidSrc: %d Vid: 0x%04X Pid: 0x%04X Version: 0x%04X",
								   app_discovery_cb->devs[index].device.eir_vid_pid[0].vendor_id_source,
								   app_discovery_cb->devs[index].device.eir_vid_pid[0].vendor,
								   app_discovery_cb->devs[index].device.eir_vid_pid[0].product,
								   app_discovery_cb->devs[index].device.eir_vid_pid[0].version);
						}
					}
				}
#endif
			} else if (!strcasecmp(event.event.misc.type, "se_open")) {
				printf("se_open!\n");
			} else if (!strcasecmp(event.event.misc.type, "se_close")) {
				printf("se_close!\n");
			} else if (!strcasecmp(event.event.misc.type, "se_create")) {
				ble_service_create_enable = 1;
			} else if (!strcasecmp(event.event.misc.type, "se_addchar")) {
				ble_service_add_char_enable = 1;
			} else if (!strcasecmp(event.event.misc.type, "se_start")) {
				ble_service_start_enable = 1;
			} else if (!strcasecmp(event.event.misc.type, "se_r_complete")) {
			} else if (!strcasecmp(event.event.misc.type, "se_w_complete")) {
#if (SUPPORT_BSA_BLE == 1)
				int i = 0;
				int value_num = 0;
				int server_num = 0;
				int char_attr_num = 0;
				int length_of_data = 3;
				UINT8 value[BSA_BLE_MAX_ATTR_LEN];
				ble_server_indication ble_indication;

				server_num = ble_heart_rate_service.server_num;
				char_attr_num = ble_notify_char.char_attr_num;
				value_num = mozart_bluetooth_server_get_char_value(server_num, char_attr_num, value);
				printf("value_num = %d\n", value_num);
				for (i = 0; i < value_num; i++)
					printf("value[%d] = 0x%x\n", i, value[i]);
				/* Listen for notify */
				if (value[0] == 1) {
					ble_indication.server_num = server_num;
					ble_indication.attr_num = ble_heart_rate_measure_char.char_attr_num;
					ble_indication.length_of_data = length_of_data;
					server_num = ble_heart_rate_service.server_num;
					char_attr_num = ble_heart_rate_measure_char.char_attr_num;
					/* get and set client vaule */
					value_num = mozart_bluetooth_server_get_char_value(server_num, char_attr_num, value);
					printf("value_num1 = %d\n", value_num);
					for (i = 0; i < value_num; i++)
						printf("value1 = 0x%x\n", value[i]);
					value[0] = 0x12;
					value[1] = 0x13;
					value[2] = 0x14;
					mozart_bluetooth_server_set_char_value(server_num, char_attr_num, value, 3);
					/* send client indication */
					mozart_bluetooth_ble_server_send_indication(&ble_indication);
				}
#endif /* SUPPORT_BSA_BLE */
			} else if (!strcasecmp(event.event.misc.type, "cl_open")) {
				ble_client_open_enable = 1;
			} else if (!strcasecmp(event.event.misc.type, "cl_write")) {
				ble_client_write_enable = 1;
			} else if (!strcasecmp(event.event.misc.type, "cl_close")) {
				printf("cl_close!\n");
			} else {
				printf("unhandle bt ble event: %s.\n", event.event.misc.type);
			}
		} else if (!strcasecmp(event.event.misc.name, "bt_hh")) {
			if (!strcasecmp(event.event.misc.type, "add_dev")) {
				ble_hh_add_dev_enable = 1;
			} else if (!strcasecmp(event.event.misc.type, "connected")) {

			} else if (!strcasecmp(event.event.misc.type, "close")) {
			} else {
				printf("unhandle bt hh event: %s.\n", event.event.misc.type);
			}
		} else if (!strcasecmp(event.event.misc.name, "bt_avrcp")) {
			if (!strcasecmp(event.event.misc.type, "playing")) {
				printf("playing!\n");
			} else if (!strcasecmp(event.event.misc.type, "paused")) {
				printf("pause!\n");
			} else if (!strcasecmp(event.event.misc.type, "stopped")) {
				printf("stopped!\n");
			} else if (!strcasecmp(event.event.misc.type, "track_change")) {
				mozart_bluetooth_avk_send_get_element_att_cmd();
			} else if (!strcasecmp(event.event.misc.type, "play_pos")) {
				printf("play_pos: %d\n", event.event.misc.value[0]);
			}
		} else if (!strcasecmp(event.event.misc.name, "bt_ops")) {
			if (!strcasecmp(event.event.misc.type, "connected")) {

			} else if (!strcasecmp(event.event.misc.type, "disconnected")) {

			} else if (!strcasecmp(event.event.misc.type, "send_request")) {
				char *ops_file_name = NULL;
				ops_file_name = mozart_bluetooth_ops_get_object_name();
				printf("ops_file_name = %s\n", ops_file_name);
				printf("len = %d\n", strlen(ops_file_name));
			} else if (!strcasecmp(event.event.misc.type, "send_start")) {

			} else if (!strcasecmp(event.event.misc.type, "send_end")) {

			}
		} else if (!strcasecmp(event.event.misc.name, "bt_dg")) {
			if (!strcasecmp(event.event.misc.type, "connected")) {
				bt_link_state = BT_LINK_CONNECTED;
			} else if (!strcasecmp(event.event.misc.type, "disconnected")) {
				bt_link_state = BT_LINK_DISCONNECTED;
			} else if (!strcasecmp(event.event.misc.type, "find_service")) {
			}
		/* phone book client */
		} else if (!strcasecmp(event.event.misc.name, "bt_pbc")) {
			if (!strcasecmp(event.event.misc.type, "connected")) {
#if (SUPPORT_BSA_PBC == 1)
				mozart_bluetooth_pbc_get_phonebook(tel_str[TEL_PB_PATH]);
#endif /* SUPPORT_BSA_PBC */
			} else if (!strcasecmp(event.event.misc.type, "disconnected")) {

			}
#endif /* SUPPORT_BT */
		} else if (!strcasecmp(event.event.misc.name, "dlna")) {
			if (!strcasecmp(event.event.misc.type, "connected")) {
				; // do nothing on dlna device connected event
			} else if (!strcasecmp(event.event.misc.type, "disconnected")) {
				if (get_current_playmode() == PM_RENDER)
					set_first_class_disconnect();
			} else if (!strcasecmp(event.event.misc.type, "play")){
				if (mozart_module_stop())
					share_mem_set(RENDER_DOMAIN, RESPONSE_CANCEL);
				else {
					current_play_domain_change(PM_RENDER);
					share_mem_set(RENDER_DOMAIN, RESPONSE_DONE);					
				}
			} else if (!strcasecmp(event.event.misc.type, "pause")){
				printf("dlna player paused.\n");
			} else if (!strcasecmp(event.event.misc.type, "resume")){
				printf("dlna player resume.\n");
			} else {
				printf("unhandle dlna event: %s.\n", event.event.misc.type);
			}
		} else if (!strcasecmp(event.event.misc.name, "musicplayer")) {
			if (!strcasecmp(event.event.misc.type, "play")) {
				if (mozart_module_stop())
					share_mem_set(MUSICPLAYER_DOMAIN, RESPONSE_CANCEL);
				else
					share_mem_set(MUSICPLAYER_DOMAIN, RESPONSE_DONE);
			} else if (!strcasecmp(event.event.misc.type, "playing")) {
				printf("music player playing.\n");
			} else if (!strcasecmp(event.event.misc.type, "pause")) {
				printf("music player paused.\n");
			} else if (!strcasecmp(event.event.misc.type, "resume")) {
				printf("music player resume.\n");
			} else {
				printf("unhandle music event: %s.\n", event.event.misc.type);
			}
		} else if (!strcasecmp(event.event.misc.name, "airplay")) {
			if (!strcasecmp(event.event.misc.type, "connected")) {
				if (mozart_module_stop())
					share_mem_set(AIRPLAY_DOMAIN, RESPONSE_CANCEL);
				else {
					current_play_domain_change(PM_AIRPLAY);
					share_mem_set(AIRPLAY_DOMAIN, RESPONSE_DONE);
				}
			} else if (!strcasecmp(event.event.misc.type, "disconnected")) {
				printf("phone disconnected, airplay play stop.\n");
				if (get_current_playmode() == PM_AIRPLAY)
					set_first_class_disconnect();
			} else if (!strcasecmp(event.event.misc.type, "paused")) {
				printf("airplay play paused.\n");
			} else if (!strcasecmp(event.event.misc.type, "resumed")) {
				printf("airplay play resumed.\n");
			}
		} else if (!strcasecmp(event.event.misc.name, "localplayer")) {
			if (!strcasecmp(event.event.misc.type, "play")) {
				if (mozart_module_stop())
					share_mem_set(LOCALPLAYER_DOMAIN, RESPONSE_CANCEL);
				else
					share_mem_set(LOCALPLAYER_DOMAIN, RESPONSE_DONE);
			} else if (!strcasecmp(event.event.misc.type, "playing")) {
				printf("localplayer playing.\n");
			} else if (!strcasecmp(event.event.misc.type,"pause")) {
				printf("localplayer paused.\n");
			} else if (!strcasecmp(event.event.misc.type,"resume")) {
				printf("localplayer resumed.\n");
			}
		} else if (!strcasecmp(event.event.misc.name,"vr")) {
			if (!strcasecmp(event.event.misc.type,"vr wake up"))
				mozart_ui_asr_wakeup();
			else if (!strcasecmp(event.event.misc.type,"vr failed"))
				mozart_ui_asr_failed();
			else if (!strcasecmp(event.event.misc.type,"vr unclear"))
				mozart_ui_asr_unclear();
#if (SUPPORT_MULROOM == 1)
		} else if (!strcasecmp(event.event.misc.name, "multiroom")) {
			if (!strcasecmp(event.event.misc.type, "group_dist")) {
				printf("group dist event\n");
				mozart_mulroom_audio_change(MR_AO_DISTRIBUTOR);
			} else if (!strcasecmp(event.event.misc.type, "dismiss_dist")) {
				printf("dismiss dist event\n");
				mozart_mulroom_audio_change(MR_AO_NORMAL);
			} else if (!strcasecmp(event.event.misc.type, "group_recv")) {
				printf("group recv event\n");
				mozart_module_stop();
				stopall(-1);
			} else if (!strcasecmp(event.event.misc.type, "dismiss_recv")) {
				printf("dismiss recv event\n");
				startall(-1);
			}
#endif /* SUPPORT_MULROOM == 1 */
		} 
//DAMAI add
#if 1
		else if (!strcasecmp(event.event.misc.name, "dm")) {
			if (!strcasecmp(event.event.misc.type, "web_conn")) {
				mozart_module_stop();
				stopall(1);
				if (!get_airkissing())
					dm_switch_sta_by_file();
			}
			#if 0
			else if (!strcasecmp(event.event.misc.type, "web_disconn")) {
				//forgetting ssid is not support in this sdk,so i just let it connect to an invaild ssid to 'forget' the current
				mozart_module_pause();
				dm_switch_sta_by_invaildssid();				
			}
			#endif
		}
#endif
		else {
			printf("Unhandle event: %s-%s.\n", event.event.misc.name, event.event.misc.type);
		}
		
		break;
	case EVENT_KEY:
	default:
		break;
	}

	return;
}

static void *sync_time_func(void *arg)
{
#if (SUPPORT_MULROOM == 1)
	mozart_mulroom_run_ntpd();
#else
	mozart_system("ntpd -nq");
#endif

#if (SUPPORT_ALARM == 1)
	mozart_alarm_set_rtc_time(time(NULL));
#else
	mozart_system("hwclock -w -u -f /dev/rtc0");
#endif

	return NULL;
}

int network_callback(const char *p)
{
	pthread_t sync_time_pthread;
	struct json_object *wifi_event = NULL;
	char test[64] = {0};
	wifi_ctl_msg_t new_mode;
	event_info_t network_event;

	printf("[%s] network event: %s\n", __func__, p);
	wifi_event = json_tokener_parse(p);
	//printf("event.to_string()=%s\n", json_object_to_json_string(event));

	memset(&network_event, 0, sizeof(event_info_t));
	struct json_object *tmp = NULL;
	json_object_object_get_ex(wifi_event, "name", &tmp);
	if(tmp != NULL){
		strncpy(network_event.name, json_object_get_string(tmp), strlen(json_object_get_string(tmp)));
		//printf("name:%s\n", json_object_get_string(tmp));
	}

	json_object_object_get_ex(wifi_event, "type", &tmp);
	if(tmp != NULL){
		strncpy(network_event.type, json_object_get_string(tmp), strlen(json_object_get_string(tmp)));
		//printf("type:%s\n", json_object_get_string(tmp));
	}

	json_object_object_get_ex(wifi_event, "content", &tmp);
	if(tmp != NULL){
		strncpy(network_event.content, json_object_get_string(tmp), strlen(json_object_get_string(tmp)));
		//printf("content:%s\n", json_object_get_string(tmp));
	}

	memset(&new_mode, 0, sizeof(wifi_ctl_msg_t));
	if (!strncmp(network_event.type, event_type_str[STA_STATUS], strlen(event_type_str[STA_STATUS]))) {
		//printf("[%s]: %s\n", network_event.type, network_event.content);
		null_cnt = 0;
		if(!strncmp(network_event.content, "STA_CONNECT_STARTING", strlen("STA_CONNECT_STARTING"))){
			mozart_ui_net_connecting();
			mozart_play_key("wifi_linking");
		}

		if(!strncmp(network_event.content, "STA_CONNECT_FAILED", strlen("STA_CONNECT_FAILED"))){
			json_object_object_get_ex(wifi_event, "reason", &tmp);
			if(tmp != NULL){
				strncpy(test, json_object_get_string(tmp), strlen(json_object_get_string(tmp)));
				printf("STA_CONNECT_FAILED REASON:%s\n", json_object_get_string(tmp));
			}
			{
				mozart_ui_net_connect_failed();
				mozart_play_key("wifi_link_failed");
#if 1	//DM6291_FIX	
	            new_mode.cmd = SW_AP;
	            new_mode.force = true;
	            strcpy(new_mode.name, app_name);
	            if(request_wifi_mode(new_mode) != true)
		            printf("ERROR: [%s] Request Network Failed, Please Register First!!!!\n", app_name);
#else
				new_mode.cmd = SW_NEXT_NET;
				new_mode.force = true;
				strcpy(new_mode.name, app_name);
				new_mode.param.network_config.timeout = -1;
				memset(new_mode.param.network_config.key, 0, sizeof(new_mode.param.network_config.key));
				if(request_wifi_mode(new_mode) != true)
					printf("ERROR: [%s] Request Network Failed, Please Register First!!!!\n", app_name);
#endif
			}
		}
		if(!strncmp(network_event.content, "STA_SCAN_OVER", strlen("STA_SCAN_OVER"))){
//DAMAI add
#if 1
			printf("sta scan over, sw to ap\n");
			dm_wifi_mode_switch(SW_AP);
			//do not thing 
#else
			new_mode.cmd = SW_NETCFG;
			new_mode.force = true;
			strcpy(new_mode.name, app_name);
			new_mode.param.network_config.timeout = -1;
			memset(new_mode.param.network_config.key, 0, sizeof(new_mode.param.network_config.key));
			new_mode.param.network_config.method |= COOEE;
			strcpy(new_mode.param.network_config.wl_module, wifi_module_str[BROADCOM]);
			if(request_wifi_mode(new_mode) != true)
				printf("ERROR: [%s] Request Network Failed, Please Register First!!!!\n", app_name);
#endif
		}
	} else if (!strncmp(network_event.type, event_type_str[NETWORK_CONFIGURE], strlen(event_type_str[NETWORK_CONFIGURE]))) {
		//printf("[%s]: %s\n", network_event.type, network_event.content);
		null_cnt = 0;
		if(!strncmp(network_event.content, network_configure_status_str[AIRKISS_STARTING], strlen(network_configure_status_str[AIRKISS_STARTING]))) {
			mozart_ui_net_config();
			mozart_play_key("airkiss_config");
//DAMAI add
#if 1
			dm_set_airkissing(1);
			update_led_status();
#endif
		}

		if(!strncmp(network_event.content, network_configure_status_str[AIRKISS_FAILED], strlen(network_configure_status_str[AIRKISS_FAILED]))) {
			mozart_ui_net_config_failed();
			mozart_play_key("airkiss_config_fail");
//DAMAI add
#if 1
			dm_set_airkissing(0);
			update_led_status();
#endif
			json_object_object_get_ex(wifi_event, "reason", &tmp);
			if(tmp != NULL){
				strncpy(test, json_object_get_string(tmp), strlen(json_object_get_string(tmp)));
				printf("NETWORK CONFIGURE REASON:%s\n", json_object_get_string(tmp));
			}
			//new_mode.cmd = SW_STA;
			new_mode.cmd = SW_AP;
			//new_mode.cmd = SW_NETCFG;
			new_mode.force = true;
			strcpy(new_mode.name, app_name);
			if(request_wifi_mode(new_mode) != true)
				printf("ERROR: [%s] Request Network Failed, Please Register First!!!!\n", app_name);
		} else if (!strncmp(network_event.content, network_configure_status_str[AIRKISS_SUCCESS], strlen(network_configure_status_str[AIRKISS_SUCCESS]))) {
			mozart_ui_net_config_success();
			mozart_play_key("airkiss_config_success");
//DAMAI add
#if 1
            system("sync");//JJJHHH----ave wifi configure
			dm_set_airkissing(0);
			update_led_status();
#endif
			{
#if 1
				json_object_object_get_ex(wifi_event, "ssid", &tmp);
				if(tmp != NULL){
					strncpy(test, json_object_get_string(tmp), strlen(json_object_get_string(tmp)));
					printf("ssid:%s\n", json_object_get_string(tmp));
				}
				json_object_object_get_ex(wifi_event, "passwd", &tmp);
				if(tmp != NULL){
					strncpy(test, json_object_get_string(tmp), strlen(json_object_get_string(tmp)));
					printf("passwd:%s\n", json_object_get_string(tmp));
				}
				json_object_object_get_ex(wifi_event, "ip", &tmp);
				if(tmp != NULL){
					strncpy(test, json_object_get_string(tmp), strlen(json_object_get_string(tmp)));
					printf("ip:%s\n", json_object_get_string(tmp));
				}
#endif
			}
		} else if(!strncmp(network_event.content, network_configure_status_str[AIRKISS_CANCEL], strlen(network_configure_status_str[AIRKISS_CANCEL]))) {
			mozart_play_key("airkiss_config_quit");
//HZB DM6291
#if 1
			dm_set_airkissing(0);
			update_led_status();
			dm_wifi_mode_switch(SW_AP);
#endif
		}
	} else if (!strncmp(network_event.type, event_type_str[WIFI_MODE], strlen(event_type_str[WIFI_MODE]))) {
		//printf("[%s]: %s\n", network_event.type, network_event.content);
		json_object_object_get_ex(wifi_event, "last", &tmp);
		if(tmp != NULL){
			strncpy(test, json_object_get_string(tmp), strlen(json_object_get_string(tmp)));
			//printf("last:%s\n", json_object_get_string(tmp));
		}
		wifi_info_t infor = get_wifi_mode();
		mozart_system("killall ntpd > /dev/null 2>&1");
		mozart_system("killall dnsmasq > /dev/null 2>&1");
		if (infor.wifi_mode == AP) {
			null_cnt = 0;
			mozart_ui_net_connect_failed();
			mozart_led_turn_off(LED_RECORD);
			mozart_led_turn_slow_flash(LED_WIFI);
			mozart_play_key("wifi_ap_mode");
			startall(1);
			//a little bad, fix it.
			if (in_depend_network_playmode() || mozart_module_get_play_status() == mozart_module_status_stop)
				set_switch_souce_snd();
			else
				if (mozart_module_get_play_status() == mozart_module_status_pause)
				mozart_play_pause();
		} else if (infor.wifi_mode == STA) {
			mozart_ui_net_connected();
			null_cnt = 0;
			mozart_led_turn_off(LED_RECORD);
			mozart_led_turn_on(LED_WIFI);
			mozart_play_key("wifi_sta_mode");

			// XXX
			// 1. sync network time(and update system time with CST format)
			// 2. write it to hardware RTC(with UTC format).

			if (pthread_create(&sync_time_pthread, NULL, sync_time_func, NULL) == -1)
				printf("Create sync time pthread failed: %s.\n", strerror(errno));
			pthread_detach(sync_time_pthread);
			mozart_system("dnsmasq &");

#if (SUPPORT_MULROOM == 1)
			if (mulroom_state != MUL_GROUP_RECEIVER)
				startall(1);

			if (mulroom_state != MUL_IDLE) {
				mozart_play_tone_sync(NULL);
				/* Set under low cpu time */
				mozart_mulroom_restore_mode();
			}
#else
			startall(1);
			if (in_depend_network_playmode() || mozart_module_get_play_status() == mozart_module_status_stop)
				set_switch_souce_snd();
			else
				if (mozart_module_get_play_status() == mozart_module_status_pause)
				mozart_play_pause();
#endif /* SUPPORT_MULROOM == 1 */
		} else if (infor.wifi_mode == WIFI_NULL) {
			null_cnt++;
			if(null_cnt >= 10){
				null_cnt = 0;
				new_mode.cmd = SW_NETCFG;
				new_mode.force = true;
				strcpy(new_mode.name, app_name);
				new_mode.param.network_config.timeout = -1;
				memset(new_mode.param.network_config.key, 0, sizeof(new_mode.param.network_config.key));
				new_mode.param.network_config.method |= COOEE;
				strcpy(new_mode.param.network_config.wl_module, wifi_module_str[BROADCOM]);
				if(request_wifi_mode(new_mode) != true)
					printf("ERROR: [%s] Request Network Failed, Please Register First!!!!\n", app_name);
			}
		} else {
			printf("[ERROR]: Unknown event type!!!!!!\n");
		}
	} else if (!strncmp(network_event.type, event_type_str[AP_STATUS], strlen(event_type_str[AP_STATUS]))) {
		//printf("[%s]: %s\n", network_event.type, network_event.content);
		if(!strncmp(network_event.content, "AP-STA-CONNECTED", strlen("AP-STA-CONNECTED"))) {
			printf("\nThe client has the connection is successful.\n");
		} else if (!strncmp(network_event.content, "AP-STA-DISCONNECTED", strlen("AP-STA-DISCONNECTED"))) {
			printf("\nThe client has been disconnected.\n");
		}
	} else if (!strncmp(network_event.type, event_type_str[STA_IP_CHANGE], strlen(event_type_str[STA_IP_CHANGE]))) {
		printf("[WARNING] STA IP ADDR HAS CHANGED!!\n");
	} else {
		printf("Unknown Network Events-[%s]: %s\n", network_event.type, network_event.content);
	}

	json_object_put(wifi_event);
	return 0;
}

#if (SUPPORT_ALARM == 1)
int alarm_callback(struct alarm_info *a)
{
	printf("alarm callback id: %d, hour: %d, minute: %d, timestamp: %ld info: %s\n",
		   a->alarm_id, a->hour, a->minute, a->timestamp, a->prv_info.info);
	return 0;
}
#endif

static int initall(void)
{
	// recover volume in S01system_init.sh

	//share memory init.
	if(0 != share_mem_init()){
		printf("share_mem_init failure.\n");
	}
	if(0 != share_mem_clear()){
		printf("share_mem_clear failure.\n");
	}

	if (mozart_path_is_mount("/mnt/sdcard")) {
		tfcard_status = 1;
		share_mem_set(SDCARD_DOMAIN, STATUS_INSERT);
	} else {
		share_mem_set(SDCARD_DOMAIN, STATUS_EXTRACT);
	}

	if (mozart_path_is_mount("/mnt/usb")) {
		udisk_status = 1;
		share_mem_set(UDISK_DOMAIN, STATUS_INSERT);
	}else
		share_mem_set(UDISK_DOMAIN, STATUS_EXTRACT);

	// shairport ini_interface.hinit(do not depend network part)

	// TODO: render init if needed.

	// TODO: localplayer init if needed.

	// TODO: bt init if needed.

	// TODO: vr init if needed.

	// TODO: other init if needed.

	return 0;
}

#if 0
void check_mem(int line)
{
	return ;
	pid_t pid = getpid();
	time_t timep;
	struct tm *p;
	FILE *fp;

	char rss[8] = {};
	char cmd[256] = {};
	char path[256] = {};

	// get current time.
	time(&timep);
	p=gmtime(&timep);


	printf("capture %d's mem info to %s.\n", pid, path);

	// get rss
	fp = popen("/bin/ps wl | grep mozart | grep -v grep | tr -s ' ' | cut -d' ' -f6", "r");
	if (!fp) {
		strcpy(rss, "99999");
	} else {
		fgets(rss, 5, fp);
		fclose(fp);
	}

	// capture exmap info.
	sprintf(path, "/tmp/%06d_%04d%02d%02d_%02d:%02d:%02d_%04d_%s.exmap",
			pid, (1900+p->tm_year), (1+p->tm_mon),p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, line, rss);

	sprintf(cmd, "echo %d > /proc/exmap; cat /proc/exmap > %s", pid, path);
	system(cmd);

	sprintf(cmd, "cat /proc/%d/maps >> %s", pid, path);

	system(cmd);
}
#endif

static void dump_compile_info(void)
{
	time_t timep;
	struct passwd *pwd;
	char hostname[16] = "Unknown";

	time(&timep);
	pwd = getpwuid(getuid());
	gethostname(hostname, 16);
	printf("mozart compiled at %s on %s@%s\n", asctime(gmtime(&timep)), pwd->pw_name, hostname);
}

void *mozart_updater_check_version(void *arg)
{
	nvinfo_t *nvinfo = NULL;

	while(1) {
		if ((nvinfo = mozart_updater_chkver()) != NULL) {
			free(nvinfo);
			mozart_system("updater -p");
			break;
		}
		sleep(10);
	}

	return NULL;
}

int main(int argc, char **argv)
{
	int daemonize = 0;
	printf("mozart 4\n");
	app_name = argv[0];
	wifi_ctl_msg_t new_mode;
	struct wifi_client_register wifi_info;
	pthread_t updater_chkver_pthread;

	/* Get command line parameters */
	int c;
	while (1) {
		c = getopt(argc, argv, "bBsSh");
		if (c < 0)
			break;
		switch (c) {
		case 'b':
		case 'B':
			daemonize = 1;
			break;
		case 's':
		case 'S':
			break;
		case 'h':
			usage(app_name);
			return 0;
		default:
			usage(app_name);
			return 1;
		}
	}

	/* run in the background */
	if (daemonize) {
		if (daemon(0, 1)) {
			perror("daemon");
			return -1;
		}
	}
	dump_compile_info();

#if (SUPPORT_MULROOM == 1)
	mulroom_state = mozart_mulroom_check_saved_mode(MULROOM_INFO_PATH);
#endif /* SUPPORT_MULROOM == 1 */


#if (SUPPORT_MULROOM == 1)
	if (mulroom_state != MUL_GROUP_RECEIVER) {
#if (SUPPORT_LOCALPLAYER == 1)
		mozart_localplayer_scan_callback(tfcard_scan_1music_callback, tfcard_scan_done_callback);
#endif
	}
#else
#if (SUPPORT_LOCALPLAYER == 1)
	mozart_localplayer_scan_callback(tfcard_scan_1music_callback, tfcard_scan_done_callback);
#endif
#endif /* SUPPORT_MULROOM == 1 */

	initall();

#if (SUPPORT_MULROOM == 1)
	if (mulroom_state != MUL_GROUP_RECEIVER)
		startall(0);
#else
	// start modules do not depend network.
	startall(0);
#endif /* SUPPORT_MULROOM == 1 */

	register_battery_capacity();

	// register key event
	keyevent_handler = mozart_event_handler_get(keyevent_callback, app_name);
	miscevent_handler = mozart_event_handler_get(miscevent_callback, app_name);

	// register network manager
	memset(&wifi_info, 0, sizeof(wifi_info));
	if(register_to_networkmanager(wifi_info, network_callback) != 0) {
		printf("ERROR: [%s] register to Network Server Failed!!!!\n", app_name);
	} else if(!access("/usr/data/wpa_supplicant.conf", R_OK)) {
		memset(&new_mode, 0, sizeof(new_mode));
		new_mode.cmd = SW_STA;
		new_mode.force = true;
		strcpy(new_mode.name, app_name);
		if(request_wifi_mode(new_mode) != true)
			printf("ERROR: [%s] Request Network Failed, Please Register First!!!!\n", app_name);
	}

#if (SUPPORT_ALARM == 1)
	struct alarm_info *alarm = NULL;
	//register alarm manager
	register_to_alarm_manager(alarm_callback);

	if ((alarm = mozart_alarm_check_wakeup_status()) != NULL) {
		printf("[%s] hour %d minute %d name %s info %s\n",
				app_name, alarm->hour, alarm->minute, alarm->name, alarm->prv_info.info);
		free(alarm);
	}

	mozart_alarm_start_alarm();
#endif

#if (SUPPORT_MULROOM == 1)
	if (mulroom_state != MUL_GROUP_RECEIVER) {
		// process linein insert event on startup.
		if (mozart_linein_is_in()) {
			printf("linein detect, switch to linein mode...\n");
			mozart_linein_on();
		}
	}
#else
	// process linein insert event on startup.
	if (mozart_linein_is_in()) {
		printf("linein detect, switch to linein mode...\n");
		mozart_linein_on();
	}
#endif /* SUPPORT_MULROOM == 1 */

	if (pthread_create(&updater_chkver_pthread, NULL, mozart_updater_check_version, NULL) == -1)
		printf("Create updater check version pthread failed: %s.\n", strerror(errno));
	pthread_detach(updater_chkver_pthread);

	// signal hander,
	signal(SIGINT, sig_handler);
	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2, sig_handler);
	signal(SIGTERM, sig_handler);
	signal(SIGBUS, sig_handler);
	signal(SIGSEGV, sig_handler);
	signal(SIGABRT, sig_handler);
	signal(SIGPIPE, SIG_IGN);

	system("echo 1 > /proc/sys/vm/overcommit_memory");
	
//DAMAI ADD
#if (SUPPORT_ETHLAN == 1)
	add_nat_for_ethlan();
#endif
	
//DAMAI ADD
#if 1
	update_led_status();
#endif

	while(1) {
//DAMAI ADD
#if 1
				static unsigned long long count = 0;
				if(count/20*20==count) //every 20s
					system("echo 3 > /proc/sys/vm/drop_caches");
				if(count/4*4==count)//every 4s
					set_eth_ip();
				check_play_status();
				update_led_status();
				//todo: a little bad, use thread commucation to fix this.
				if (get_first_class_disconnect() > 0)
					handle_first_class_disconnect();
				if (get_switch_souce_snd() > 0)
					handle_switch_souce_snd();
				++count;
				sleep(1);
#else
		sleep(20);
		system("echo 3 > /proc/sys/vm/drop_caches");
#endif
	}

	return 0;
}
