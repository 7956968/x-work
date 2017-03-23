#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>

#include "aispeech/ai_vr.h"


static const char *param = "{"
    "\"coreProvideType\":\"native\","
    "\"app\":{"
        "\"userId\":\"LeChange\""
    "},"
    "\"request\":{"
        "\"coreType\":\"cn.wakeup\","
        "\"env\":\"\""
    "}"
"}";


int _aec_callback(void *usrdata, const char *id, int type, const void *message, int size){
//    printf("resp data: %.*s\n", size, (char *) message);
	json_object *out = NULL;
    json_object *result = NULL;
    json_object *status = NULL;
	out = json_tokener_parse((char*) message);
    if (!out)
    {
        return -1;
    }

	if (json_object_object_get_ex(out, "result", &result)){
		if (json_object_object_get_ex(result, "status", &status)){
	        if (json_object_get_int(status) == 1){
				DEBUG("=======>唤醒成功<=======\n");
				ai_mutex_lock();
				ai_vr_info.from    = VR_FROM_AEC;
				ai_vr_info.aec.wakeup = true;
				if (ai_vr_callback)
					ai_vr_callback(&ai_vr_info);
				ai_mutex_unlock();
	        }
		}
    }
	json_object_put(out);

    return 0;
}

int aec_start(void){
    char uuid[64] = {0};
//	printf("\n%s\n", param);
	return aiengine_start(wakeup_agn, param, uuid, _aec_callback, NULL);
}

int aec_stop(void){
   return aiengine_stop(wakeup_agn);
}

int aec_feed(const void *rec, const void *play, int size)
{
   return aiengine_echo(wakeup_agn, rec, play, size, 0);
}

int aec_key_wakeup(void)
{
	DEBUG("=======>唤醒成功<=======\n");
	ai_mutex_lock();
	ai_vr_info.from    = VR_FROM_AEC;
	ai_vr_info.aec.wakeup = true;
	if (ai_vr_callback)
		ai_vr_callback(&ai_vr_info);
	ai_mutex_unlock();

	return 0;
}
