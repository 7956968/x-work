#ifndef DMMOZART_H
#define DMMOZART_H

#include "mozart_musicplayer.h"
#include "sharememory_interface.h"

#define c_Print_Ctrl_Off "\033[0m"
#define c_CharColor_yellow      "\033[1;33m"

enum MUSICPLAYER_TYPE {
	MUSIC_USB,
	MUSIC_SD,
	MUSIC_CLOUD,
};

enum PlayMode {
	PM_NONE = 0,
	PM_SDCARD,
	PM_UDISK,
	PM_RENDER,
	PM_AIRPLAY,
	PM_BTAVK,
	PM_BTHS,
	PM_CLOUD,
	PM_INGENIC,
	PM_VR,
};


#define dm_info(fmt, args...)	fprintf(stdout, c_CharColor_yellow"%s.%s [info] "fmt""c_Print_Ctrl_Off,__FILE__, __func__, ##args)
int check_play_status();
int dm_set_airkissing(int i);
int get_airkissing();
int get_playing();
void dm_playpause();
void dm_play();
void dm_pause();
int in_backup_system();
int domain_playing();
int in_backup_system();
int test_domain(char*);
int get_touch();
int dm_key_releaseed(int code);
int dm_key_pressed(int code);
int get_musicplayer_type();
int  dm_handle_musicplayer_insert_list(struct music_info *info);
int  dm_localplayer_start_insert(const char* music);
int first_class_playing();
enum PlayMode get_current_playmode();
int current_play_domain_change(enum PlayMode mode);
int set_first_class_disconnect();
int handle_first_class_disconnect();
int get_first_class_disconnect();
int in_depend_network_playmode();

#define safe_free(p) do{\
    if((p) != NULL)\
    {\
        free((p));\
        (p) = NULL;\
    }\
    }while(0)

#endif
