#ifndef __MUSICLIST_INTERFACE_H__
#define __MUSICLIST_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include <stdbool.h>

	/**
	 * @brief play mode
	 */
	enum play_mode {
		/**
		 * @brief order play mode
		 */
		play_mode_order = 0,
		/**
		 * @brief single cycle play mode
		 */
		play_mode_single,
		/**
		 * @brief random play mode
		 */
		play_mode_random,
	};

	/**
	 * @brief music info
	 */
	struct music_info {
		/**
		 * @brief music id
		 */
		int id;
		/**
		 * @brief music name
		 */
		char *music_name;
		/**
		 * @brief music url
		 */
		char *music_url;
		/**
		 * @brief music picture
		 */
		char *music_picture;
		/**
		 * @brief albums name
		 */
		char *albums_name;
		/**
		 * @brief artists name
		 */
		char *artists_name;
		/**
		 * @brief user data
		 */
		void *data;
	};

	struct music_list;

	/**
	 * @brief  copy a music info
	 */
	extern struct music_info *mozart_musiclist_copy_info(struct music_info *info);

	/**
	 * @brief  create a music info
	 *
	 * @param id [in]		music id
	 * @param music_name [in]	music name
	 * @param music_url [in]	music url
	 * @param music_picture [in]	music picture
	 * @param albums_name [in]	albums name
	 * @param artists name [in]	artists name
	 * @param data [in]		user data
	 *
	 * @return On success returns struct music_info pointer, return NULL if an error occurred.
	 */
	extern struct music_info *mozart_musiclist_get_info(int id, char *music_name,
							    char *music_url, char *music_picture,
							    char *albums_name, char *artists_name,
							    void *data);

	/**
	 * @brief  free music info
	 *
	 * @param free_data [in] handler the free user data
	 */
	extern void mozart_musiclist_free_music_info(void *music_info, void (*free_data)(void *));

	/**
	 * @brief  get the length of the music list.
	 *
	 * @return the length of the music list, return -1 if an error occurred.
	 */
	extern int mozart_musiclist_get_length(struct music_list *list);

	/**
	 * @brief  set max index of the music list
	 *
	 * @return On success returns 0, return -1 if an error occurred.
	 */
	extern int mozart_musiclist_set_max_index(struct music_list *list, int max_index);

	/**
	 * @brief  get max index of the music list
	 *
	 * @return max index of the music list
	 */
	extern int mozart_musiclist_get_max_index(struct music_list *list);

	/**
	 * @brief  set play mode of the music list
	 *
	 * @return On success returns 0, return -1 if an error occurred.
	 */
	extern int mozart_musiclist_set_play_mode(struct music_list *list, enum play_mode mode);

	/**
	 * @brief  get play mode of the music list
	 *
	 * @return play mode of the music list
	 */
	extern enum play_mode mozart_musiclist_get_play_mode(struct music_list *list);

	/**
	 * @brief  get current index of the music list
	 *
	 * @return current index of the music list
	 */
	extern int mozart_musiclist_get_current_index(struct music_list *list);

	/**
	 * @brief  get index's music info of the music list
	 *
	 * @return index music info of the music list
	 */
	extern struct music_info *mozart_musiclist_get_index(struct music_list *list, int index);

	/**
	 * @brief  get current music info
	 *
	 * @return current music info if get it, NULL otherwise
	 */
	extern struct music_info *mozart_musiclist_get_current(struct music_list *list);

	/**
	 * @brief  set previous music info
	 *
	 * @return previous music info if get it, NULL otherwise.
	 */
	extern struct music_info *mozart_musiclist_set_prev(struct music_list *list);

	/**
	 * @brief  set next music info
	 *
	 * @return next music info if get it, NULL otherwise.
	 */
	extern struct music_info *mozart_musiclist_set_next(struct music_list *list, bool force);

	/**
	 * @brief  set index and get index's music info of the music list
	 *
	 * @return index music info of the music list
	 */
	extern struct music_info *mozart_musiclist_set_index(struct music_list *list, int index);

	/**
	 * @brief  inseart a music info to the tail of the music list
	 *
	 * @return index where insert.
	 */
	extern int mozart_musiclist_insert(struct music_list *list, struct music_info *info);

	/**
	 * @brief  delete index's music info
	 *
	 * @return On success returns 0, return -1 if an error occurred.
	 */
	extern int mozart_musiclist_delete_index(struct music_list *list, int index);

	/**
	 * @brief  copy a music list
	 */
	extern struct music_list *mozart_musiclist_copy(struct music_list *list);
	/**
	 * @brief  create a music list
	 *
	 * @param free_data [in] handler the free user data
	 *
	 * @return On success returns 0, return -1 if an error occurred.
	 */
	extern struct music_list *mozart_musiclist_create(void (*free_data)(void *));

	/**
	 * @brief  clean all music info of the music list
	 *
	 * @return On success returns 0, return -1 if an error occurred.
	 */
	extern int mozart_musiclist_clean(struct music_list *list);

	/**
	 * @brief  destory music list
	 *
	 * @return On success returns 0, return -1 if an error occurred.
	 */
	extern int mozart_musiclist_destory(struct music_list *list);

#ifdef __cplusplus
}
#endif


#endif	/* __MUSICLIST_INTERFACE_H__ */
