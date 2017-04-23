#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#include "record_interface.h"
#include "utils_interface.h"
#include "vr_interface.h"

static bool asr_stop_flag = false;
static bool asr_break_flag = false;
static mic_record *record = NULL;
bool asr_stopped = false;

/* vr init flag */
extern bool vr_init_flag;

/* vr working flag, change on vr_start() & vr_stop() */
extern bool vr_working_flag;

/* record current vr status */
extern vr_status_t vr_status;

#ifdef DEBUG_RECORD
static int file_count = 0;
char name1[32],name2[32];
static int fd_dmic;
typedef struct{
	unsigned char riff[4];
	unsigned int file_length;
	unsigned char wav_flag[4];
	unsigned char fmt_flag[4];
	unsigned int transition;
	unsigned short format_tag;
	unsigned short channels;
	unsigned int sample_rate;
	unsigned int wave_rate;
	unsigned short block_align;
	unsigned short sample_bits;
	unsigned char data_flag[4];
	unsigned int data_length;
}wav_header;

static void init_wave_head(wav_header *wave,int rate,int bits,int channels, int size)
{
	memcpy(wave->riff,"RIFF",4);
	wave->file_length = size + 36;
	memcpy(wave->wav_flag,"WAVE",4);
	memcpy(wave->fmt_flag,"fmt ",4);
	wave->transition = 0x10;
	wave->format_tag = 0x01;
	wave->channels = channels;
	wave->sample_rate = rate;
	wave->wave_rate = rate*channels*bits/8;
	wave->block_align = channels*bits/8;
	wave->sample_bits = bits;
	memcpy(wave->data_flag,"data",4);
	wave->data_length = size;
}
static void set_wave_head(int file_fd, wav_header *wave)
{
	lseek(file_fd, 0, SEEK_SET);
	write(file_fd, wave, sizeof(wav_header));
}

static int get_file_size(char *file_name)
{
	struct stat statbuf;
	stat(file_name, &statbuf);
	int size = statbuf.st_size;
	return size;
}

static void finally_set_wav_data_size_dmic(char *filename, int fd)
{
	int size = 0;
	wav_header wave;
	size = get_file_size(filename)-44;
	init_wave_head(&wave, ASR_RATE, ASR_BIT, ASR_CHANNEL, size);
	set_wave_head(fd, &wave);
}

static void set_record_file_head()
{
	wav_header *wave;
	sprintf(name2,"/tmp/asr_dmic%d.wav",file_count);
	wave = malloc(sizeof(wav_header));
	fd_dmic = open(name2, O_RDWR | O_CREAT | O_TRUNC);
	init_wave_head(wave, ASR_RATE, ASR_BIT, ASR_CHANNEL, 0);
	set_wave_head(fd_dmic, wave);
	free(wave);
}
#endif

static int soundcard_init(void)
{
	int ret;

	record_param dmic_param = {
		.bits = ASR_BIT,
		.rates = ASR_RATE,
		.channels = ASR_CHANNEL,
		.volume = ASR_VOLUME,
	};

#ifdef DEBUG_RECORD
	set_record_file_head();
#endif

	record = mozart_soundcard_init(dmic_param);
	if (record == NULL) {
		printf("error: soundcard asr init fail.\n");
		ret = -1;
		goto err_init;
	}

	return 0;

err_init:
	mozart_soundcard_uninit(record);
	record = NULL;

	return ret;
}

static int soundcard_uninit(void)
{
	mozart_soundcard_uninit(record);
	record = NULL;

	return 0;
}

static unsigned long soundcard_asr_record(char *dmic_buf, unsigned long len)
{
	if (!record) {
		printf("error: Please init soundcard firstly!\n");
		return 0;
	}

	return mozart_record(record, dmic_buf, ASR_SIZE);
}

int mozart_asr_start(bool sds)
{
	char dmic_buf[ASR_SIZE] = {};
	asr_stop_flag = false;
	asr_break_flag = false;
	asr_stopped = false;

	soundcard_init();

	/* real asr start */
	asr_start(sds);
	printf("===> ASR: What can I do for you?\n");
	while (!asr_stop_flag && !asr_break_flag) {
		soundcard_asr_record(dmic_buf, ASR_SIZE);

#ifdef DEBUG_RECORD
		write(fd_dmic, dmic_buf, ASR_SIZE);
#endif
		asr_feed(dmic_buf, ASR_SIZE);
	}


#ifdef DEBUG_RECORD
	finally_set_wav_data_size_dmic(name2,fd_dmic);
	close(fd_dmic);
	fd_dmic = -1;
	file_count++;
#endif

	soundcard_uninit();

	if (asr_stop_flag) {
		/* real asr stop */
		asr_stop();
	}

	asr_stopped = true;

	return 0;
}

int mozart_asr_stop(int reason)
{
	if (reason == 1)
		asr_break_flag = true;
	else
		asr_stop_flag = true;

	return 0;
}

int mozart_vr_asr_break(void)
{
	if (!vr_init_flag || !vr_working_flag) {
		printf("warning: vr not init or not start, %s fail.\n", __func__);
		return -1;
	}

	vr_status = VR_IDLE;

	/* break asr process */
	asr_break();

	/* waiting for asr stopped */
	while (!asr_stopped)
		usleep(10 * 1000);

	/* close asr connection */
	asr_cancel();

	return 0;
}
