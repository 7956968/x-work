/**
 * @file localstream_interface.h
 * @brief local stream traslate server library
 * @author Su Xiaolong <xiaolong.su@ingenic.com>
 * @version 1.0.0
 * @date 2016-09-07
 *
 * Copyright (C) 2016 Ingenic Semiconductor Co., Ltd.
 *
 * The program is not free, Ingenic without permission,
 * no one shall be arbitrarily (including but not limited
 * to: copy, to the illegal way of communication, display,
 * mirror, upload, download) use, or by unconventional
 * methods (such as: malicious intervention Ingenic data)
 * Ingenic's normal service, no one shall be arbitrarily by
 * software the program automatically get Ingenic data
 * Otherwise, Ingenic will be investigated for legal responsibility
 * according to law.
 */

#ifndef __LOCALSTREAM_H__
#define __LOCALSTREAM_H__

#include <stddef.h>
#include <pthread.h>

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief [domain ipc] struct header fore
 */
struct domain_header_fore {
	short	type;
#define TYPE_INIT	0x10
#define TYPE_STREAM	0x30
#define TYPE_STOP	0x34
#define TYPE_SPACE	0x38
#define TYPE_DELAY	0x3C
#define TYPE_REPLY      0x50

	short	code;
#define CODE_OK		0x01
#define CODE_FAILED	0x00
};

/**
 * @brief [domain ipc] struct audio stream info
 */
struct domain_stream_info {
	int	channels;
	int	samplerate;
	/**
	 * @brief sample width size. unit: Byte
	 */
	int	samplesize;
	int 	format;
	int	bps;
};

/**
 * @brief [domain ipc] struct stream init result
 */
struct domain_init_result {
	int	burst;
	int	total;
};

/**
 * @brief [domain ipc] struct stream init
 */
struct domain_init_hdr {
	struct domain_header_fore	fore;
	struct domain_stream_info	info;
	struct domain_init_result	res;
};

/**
 * @brief [domain ipc] struct stream trans header
 */
struct domain_stream_hdr {
	struct domain_header_fore	fore;
	int		stream_length;
};

/**
 * @brief [domain ipc] struct stream trans reply
 */
struct domain_stream_reply {
	struct domain_header_fore	fore;
	int		recv_length;
};

/**
 * @brief [domain ipc] struct get free space
 */
struct domain_getspace_hdr {
	struct domain_header_fore	fore;
	int		space;
};

/**
 * @brief struct local stream buffer head
 */
struct loStreamBuffer {
	char	*buf;
	size_t	wanted;
	size_t	size;
};

/**
 * @brief struct local stream callbacks
 */
struct loStream_callback {
	/**
	 * @brief stream init callback
	 *
	 * @param header[in] stream init info
	 * @param para[in] user data
	 *
	 * @return 0 init success, -1 failed.
	 */
	int (*initStream)(struct domain_init_hdr *header, void *para);
	/**
	 * @brief get free buffer for stream
	 *
	 * @param sBuf[out] stream buffer info
	 * @param para[in] user data
	 *
	 * @return 0 get buffer success, -1 failed.
	 */
	int (*getBuffer)(struct loStreamBuffer *sBuf, void *para);
	/**
	 * @brief get free buffer space free space for receive stream data
	 *
	 * @param header[out] get o_space header
	 * @param para[in] user data
	 *
	 * @return 0 get o_space sucess, -1 failed.
	 */
	void (*getSpace)(struct domain_getspace_hdr *header, void *para);
	/**
	 * @brief receive stream handle
	 *
	 * @param header[out] stream reply header
	 * @param loadSize[in] received stream data size
	 * @param para[in] user data
	 *
	 * @return 0 stream hanlde ok, -1 failed.
	 */
	int (*handleRecv)(struct domain_stream_reply *header, int loadSize, void *para);
	/**
	 * @brief stop stream
	 *
	 * @param header [reserve]
	 * @param para[in] user data
	 *
	 * @return 0 stop stream success, -1 failed.
	 */
	int (*stopStream)(void *header, void *para);
};

/**
 * @brief struct local stream handle
 */
struct localInStream {
	int		sock_fd;
	char		*sockpath;

	int		localBurst;

	int		cli_fd;
	int		pipe_fd[2];
	pthread_t	p_lostream;

	struct loStream_callback *cbs;
	/* All callback fuctions general parameter */
	void		*para;
};

/* Local Stream APIs */

/**
 * @brief create local stream handle
 *
 * @param domainPath[in] unix domain socket file path
 * @param localburst[in] the maxmum length for socket transfer once
 *
 * @return On succes return the localInStream * , and return NULL if an error occurred
 */
extern struct localInStream *mozart_local_stream_create(char *domainPath, int localburst);

/**
 * @brief local stream handle release
 *
 * @param stream[in] local stream handle
 */
extern void mozart_local_stream_release(struct localInStream *stream);

/**
 * @brief establish connect and start server
 *
 * @param stream[in] local stream handle
 * @param cbs[in] local stream callbacks defined by user
 * @param para[in] callbacks user param
 *
 * @return 0 start success, -1 failed.
 */
extern int mozart_local_stream_init(
	struct localInStream *stream,
	struct loStream_callback *cbs,
	void *para);

/**
 * @brief stop local stream server and clean source
 *
 * @param stream[in] local stream handle
 */
extern void mozart_local_stream_uninit(struct localInStream *stream);

#ifdef  __cplusplus
}
#endif

#endif /* __LOCALSTREAM_H__ */
