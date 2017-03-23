#ifndef __JUSHANG_INTERFACE_H__
#define __JUSHANG_INTERFACE_H__

#define VR_DOMAIN_MUSIC "music"

/**
 * @brief Parse txt.
 *
 * @param txt [in] parse keyword
 *
 * @return On success returns parse result, return NULL if an error occurred.
 */
extern char *mozart_jushang_sem(char *txt);
/**
 * @brief Get jushang music list.
 *
 * @return music list(json string format), example:
 * [{ "artist": "汪峰", "songName": "如果风不再吹",
 *		"url": "http:\sic.hezi.360iii.net\/hezimusic\/11372.mp3",
 *		"songID": "11372" },
 * { "artist": "汪峰", "songName": "上千个黎明",
 *		"url": "http:\/\ic.hezi.360iii.net\/hezimusic\/11382.mp3",
 *		"songID": "11382" },
 * { "artist": "汪峰", "songName": "让我们在一起",
 *		"url": "http:usic.hezi.360iii.net\/hezimusic\/11390.mp3",
 *		"songID": "11390" }]
 */
extern char *mozart_jushang_get_musiclist(void);

#endif /* __JUSHANG_INTERFACE_H__ */
