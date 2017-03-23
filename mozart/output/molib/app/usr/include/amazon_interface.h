#ifndef __AMAZON_INTERFACE_H__
#define __AMAZON_INTERFACE_H__

/**
 * @brief voice recognition state
 */
enum amazon_state_type {
	/**
	 * @brief shutdown state
	 */
	AMAZON_STATE_SHUTDOWN = 0,
	/**
	 * @brief idle state
	 */
	AMAZON_STATE_IDLE,
	/**
	 * @brief recognize state
	 */
	AMAZON_STATE_RECOG,
	/**
	 * @brief quit state
	 */
	AMAZON_STATE_QUIT,
	/**
	 * @brief error state
	 */
	AMAZON_STATE_ERROR,
};

struct amazon_authorization {
	char *authorizationCode;
	char *redirectUri;
	char *clientId;
	char *codeVerifier;
};

struct access_token {
	char *access_token;
	char *refresh_token;
	long int expires_in;
};

typedef int (*mozart_amazon_callback)(void *);

/**
 * @brief start voice recognition of amazon
 */
extern int mozart_amazon_startup(mozart_amazon_callback callback);

/**
 * @brief quit voice recognition of amazon
 */
extern int mozart_amazon_shutdown(void);

/**
 * @brief wakeup voice recognition of amazon
 */
extern int mozart_amazon_key_wakeup(void);
/**
 * @brief wakeup voice recognition of amazon
 */
extern void mozart_amazon_key_cancel(void);
extern int mozart_amazon_get_status(void);
extern void mozart_amazon_set_authorization(char *authorizationCode, char *redirectUri, char *clientId, char *codeVerifier);


#endif	/* __AMAZON_INTERFACE_H__ */
