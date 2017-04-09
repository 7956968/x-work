#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>

#include "mozart_config.h"
#include "mozart_musicplayer.h"
#include "vr_interface.h"
#include "dm_mozart.h"

#include "mozart_key_function.h"
//在调用此函数前要先调用mozart_module_cloud_music_start hzb
int speech_cloudmusic_playmusic(sds_music_t *music, int index)
{
	int i = 0;
	int ret = 0;
	struct music_info *info = NULL;

	if (!music)
		return -1;
	printf("speech_cloudmusic_playmusic %d\n", snd_source);
	//hzb add.因为有可能:按键切换线程先切到云音乐，正常情况下本线程会执行下面的逻辑。如果在执行下面逻辑前切换线程又切到local音乐，再继续运行下面的逻辑会有问题。所以如果cloud被local挤掉的话，直接退出
	if (snd_source != SND_SRC_CLOUD) {
		printf("in local play mode, do not play cloud music\n");
		return 0;

	}
	mozart_musicplayer_start(mozart_musicplayer_handler);
	current_play_domain_change(PM_CLOUD);

	mozart_musicplayer_musiclist_clean(mozart_musicplayer_handler);
	for (i = 0; i < music->number; i++) {
		info = mozart_musiclist_get_info(-1,/* songid ignore */
										 music->data[i].title,
										 music->data[i].url,
										 NULL,/* song picture ignore */
										 NULL,/* album name ignore */
										 music->data[i].artist,
										 NULL/* private data ignore */);
		if (info)
			mozart_musicplayer_musiclist_insert(mozart_musicplayer_handler, info);
	}

	ret = mozart_musicplayer_play_index(mozart_musicplayer_handler, index);

	return ret;
}

int speech_cloudmusic_playfm(sds_netfm_t *netfm, int index)
{
	int i = 0;
	int ret = 0;
	struct music_info *info = NULL;

	if (!netfm)
		return -1;

	mozart_musicplayer_musiclist_clean(mozart_musicplayer_handler);

	for (i = 0; i < netfm->number; i++) {
		info = mozart_musiclist_get_info(-1,/* songid ignore */
										 netfm->data[i].track,
										 netfm->data[i].url,
										 NULL,/* song picture ignore */
										 NULL,/* album name ignore */
										 NULL, /*artist ignore*/
										 NULL/* private data ignore */);
		if (info)
			mozart_musicplayer_musiclist_insert(mozart_musicplayer_handler, info);
	}

	ret = mozart_musicplayer_play_index(mozart_musicplayer_handler, index);

	return ret;
}

int speech_cloudmusic_playjoke(char *url)
{
	int ret = 0;
	struct music_info *info = NULL;

	if (!url)
		return -1;

	mozart_musicplayer_musiclist_clean(mozart_musicplayer_handler);

	info = mozart_musiclist_get_info(-1,/* songid ignore */
									 NULL,
									 url,
									 NULL,/* song picture ignore */
									 NULL,/* album name ignore */
									 NULL, /*artist ignore*/
									 NULL/* private data ignore */);
	if (info)
		mozart_musicplayer_musiclist_insert(mozart_musicplayer_handler, info);

	ret = mozart_musicplayer_play_index(mozart_musicplayer_handler, 0);

	return ret;
}

int speech_cloudmusic_play(char *key)
{
	/* get result in asr callback */
	return mozart_vr_content_get(key);
}
