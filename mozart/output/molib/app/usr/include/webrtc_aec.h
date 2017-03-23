#ifndef WEBRTC_AEC_H_
#define WEBRTC_AEC_H_

#ifdef __cplusplus
extern "C" {
#endif
int webrtc_aec_enable(void);
int ingenic_apm_init(void);
int ingenic_apm_destroy(void);
int webrtc_aec_get_buffer_length(void);
void webrtc_aec_calculate(void *buf_record, void *buf_play, void *buf_result, unsigned int size);
#ifdef __cplusplus
}
#endif
#endif

