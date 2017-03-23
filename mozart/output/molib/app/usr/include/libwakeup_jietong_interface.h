#ifndef __WAKEUP_JIETONG_INTERFACE_H
#define __WAKEUP_JIETONG_INTERFACE_H

#include <hci_micarray.h>
#include <hci_sys.h>
#include <hci_ut.h>

enum jietong_wakeup_mode {
	WAKEUP_DISABLE,
	WAKEUP_ONCE,
	WAKEUP_REPEAT,
};

enum jietong_wakeup_status {
	WAKEUP_STATUS_SLEEP,
	WAKEUP_STATUS_VOICE_WAKEUP,
	WAKEUP_STATUS_KEY_WAKEUP,
	WAKEUP_STATUS_ASR,
	WAKEUP_STATUS_STOP,
};

struct jietong_wakeup_parameter {
	int fd_wakeup[2];
	int wakeup_status;
	int voice_data_len;
	char config_file[128];
	int  wakeup_stoped;
	int  asr_stoped;
	MicArraySession session;
	MicArrayHandle handle;
};

#ifdef __cplusplus
extern "C" {
#endif

int mozart_vr_jietong_wakeup_pipe_init();

int mozart_vr_jietong_wakeup_pipe_close();

int mozart_vr_jietong_wakeup_enable_record(void);

int mozart_vr_jietong_wakeup_disable_record(void);

int mozart_vr_jietong_wakeup_init(void * parameter);

int mozart_vr_jietong_wakeup_uninit(void *parameter);

int mozart_vr_jietong_wakeup_asrdata_init(void *parameter);

int mozart_vr_jietong_wakeup_asrdata_uninit(void *parameter);

int mozart_vr_jietong_set_wakeup_mode(int wakeup_mode);

int mozart_vr_jietong_wakeup_start(void *parameter);

int mozart_vr_jietong_wakeup_stop(void *parameter);
struct jietong_wakeup_parameter* mozart_vr_jietong_wakeup_get_param();

#ifdef __cplusplus
}
#endif

#endif
