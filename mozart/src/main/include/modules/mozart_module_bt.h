#ifndef __MOZART_MODULE_BT_H__
#define __MOZART_MODULE_BT_H__

#include <inttypes.h>
#include "resample_interface.h"
#include "channels_interface.h"
#include "bluetooth_interface.h"

/* A2DP Source */
#define SUPPORT_BSA_A2DP_SOURCE			0
/* A2DP Synk */
#define SUPPORT_BSA_A2DP_SYNK			1

/* HFP AG */
#define SUPPORT_BSA_HFP_AG			0
/* HFP HF */
#define SUPPORT_BSA_HFP_HF			1

/* phone book server */
#define SUPPORT_BSA_PBS				0
/* phone book client */
#define SUPPORT_BSA_PBC				0

/* OPP Server */
#define SUPPORT_BSA_OPS				0
/* OPP Client */
#define SUPPORT_BSA_OPC				0

/* SPP Server */
#define SUPPORT_BSA_SPPS			0
/* SPP Client */
#define SUPPORT_BSA_SPPC			0

//hzb add
#if SUPPORT_BTREMOTE !=  1
/* BSA BLE SUPPORT */
#define SUPPORT_BSA_BLE				0
#define SUPPORT_BSA_BLE_SERVER			0
#define SUPPORT_BSA_BLE_CLIENT			0

/* BSA BLE HID SUPPORT */
#define SUPPORT_BSA_BLE_HH			0

/* BLE HID DIALOG DEVICE */
#define SUPPORT_BSA_BLE_HH_DIALOG_AUDIO		0
#else
/* BSA BLE SUPPORT */
#define SUPPORT_BSA_BLE				1
#define SUPPORT_BSA_BLE_SERVER			0
#define SUPPORT_BSA_BLE_CLIENT			1

/* BSA BLE HID SUPPORT */
#define SUPPORT_BSA_BLE_HH			1

/* BLE HID DIALOG DEVICE */
#define SUPPORT_BSA_BLE_HH_DIALOG_AUDIO		1
#endif

/* Automatic Echo Cancellation RESAMPLE SUPPORT */
#define SUPPORT_AEC_RESAMPLE			0
#define SUPPORT_AEC_RESAMPLE_48K_TO_8K		0

//HZB add
#if (BT_CALL_RESAMPLE_96K == 1)
/* BSA HFP HF RESAMPLE SUPPORT */
#define SUPPORT_BSA_HS_RESAMPLE			1
#define SUPPORT_BSA_HS_RESAMPLE_8K_to_48K	1
#else
/* BSA HFP HF RESAMPLE SUPPORT */
#define SUPPORT_BSA_HS_RESAMPLE			0
#define SUPPORT_BSA_HS_RESAMPLE_8K_to_48K	0
#endif

#if (SUPPORT_BSA_HFP_HF == 1)
typedef struct {
	int  in_len;
	char *in_buf;
	char *out_buf;
	int  out_len;
	int  max_outlen;
	af_resample_t *resample_t;
} hs_resample_info;

typedef struct {
	int  in_len;
	char *in_buf;
	char *out_buf;
	int  out_len;
	int  max_outlen;
	af_channels_t *channel_t;
} hs_rechannel_info;

typedef struct {
	hs_resample_info  res;
	hs_rechannel_info rec;
} hs_resample_param;

hs_resample_param hs_bt;
hs_resample_param hs_record;

#if (SUPPORT_WEBRTC == 1)
typedef struct {
	unsigned int outlen;
	char *outbuf;
	hs_resample_param aec;
} hs_aec_param;

hs_resample_param hs_ref;
hs_aec_param hs_aec;
#endif /* SUPPORT_WEBRTC */
#endif /* SUPPORT_BSA_HFP_HF */


extern int start_bt(void);
extern int stop_bt(void);
#endif /* __MOZART_MODULE_BT_H__ */
