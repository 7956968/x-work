#ifndef WEBRTC_AEC_H_
#define WEBRTC_AEC_H_

#ifdef __cplusplus
extern "C" {
#endif
int ingenic_apm_init(unsigned int *rate, unsigned int *channel, unsigned int *bits, unsigned int time);
int ingenic_apm_destroy(void);
int webrtc_aec_get_buffer_length(void);
void webrtc_aec_calculate(void *buf_record, void *buf_play, void *buf_result, unsigned int size);
#ifdef __cplusplus
}
#endif
#endif

