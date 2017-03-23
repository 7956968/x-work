#ifndef __VR_JIETONG_INTERFACE_H
#define __VR_JIETONG_INTERFACE_H

#include <libwakeup_jietong_interface.h>

enum recog_flag_status {
	ASR = 0,
	SEARCH_MUSIC,
	NATIVE_ASR_CMD,
	TTS_ANSWER,
	WAKE_UP,
	WRONG,
};

enum wakeup_mode {
	VOICE_KEY_MIX_WAKEUP = 0,
	VOICE_WAKEUP,
	KEY_SHORTPRESS_WAKEUP,
	KEY_LONGPRESS_WAKEUP,
};
enum asr_mode {
	CLOUD = 0,
	NATIVE,
};

typedef struct vr_info {
	/**
	 * @brief voice recognition status
	 * 0:asring
	 * 1:search music
	 * 2:native cmd
	 * 3:tts answer
	 * 4:wakeup
	 * 5:wrong
	 */
	int recog_flag;
	char recog_result[128];
	int json_len;
	char recog_asr_result[0];
} vr_info;

typedef int (*mozart_vr_jietong_callback)(vr_info *recog_info);
extern int mozart_vr_jietong_get_status(void);
extern int mozart_vr_jietong_shutdown(void);
extern int mozart_key_wakeup(void);
extern int mozart_vr_jietong_startup(int wakeup_mode,
		mozart_vr_jietong_callback callback);

extern int mozart_vr_jietong_asr_cap_init();
extern int mozart_vr_jietong_asr_cap_uninit();
#endif
