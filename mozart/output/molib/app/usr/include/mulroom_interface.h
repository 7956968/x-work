/**
 * @file mulroom_interface.h
 * @briefi multiroom control API
 * @author Su Xiaolong <xiaolong.su@ingenic.com>
 * @version 2.0.0
 * @date 2017-01-06
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
#ifndef __MULROOM_INTERFACE_H__
#define __MULROOM_INTERFACE_H__

/* Make this header file easier to include in C++ code */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Multiroom service state
 */
enum mul_state {
	/**
	 * @brief Not get multiroom state
	 */
	MUL_EMPTY,
	/**
	 * @brief this device is idle, can group a multiroom
	 */
	MUL_IDLE,
	/**
	 * @brief this device is a disributor of group already
	 */
	MUL_GROUP_DISTRIBUTOR,
	/**
	 * @brief this device is a receiver of group already
	 */
	MUL_GROUP_RECEIVER,
};

/**
 * @brief get device multiroom state
 *
 * @param json_str[out] multiroom receive devices json list or distribute device info
 *
 * @return this device multiroom state
 */
extern enum mul_state mozart_mulroom_get_state(char **json_str);

/**
 * @brief set device function through distributor
 *
 * @param json_str[in] device name and set args json string
 * @param json_res[out] device set args json_string
 *
 * @return 0 set success, -1 failed.
 */
extern int mozart_mulroom_distributor_set_device(char *json_str, char **json_res);

/**
 * @brief make device to a multiroom group as distributor
 *
 * @param json_str[in] recevier devices json list
 * @param json_res[out] group succuss receive devices json list
 *
 * @return 0 groups success, -1 failed.
 */
extern int mozart_mulroom_group_distributor(char *json_str, char **json_res);

/**
 * @brief make device to a multiroom group as receiver
 *
 * @param json_str[in] distribute device info
 * @param json_res[out] distribute device final info
 *
 * @return 0 groups success, -1 failed.
 */
extern int mozart_mulroom_group_receiver(char *json_str, char **json_res);

/**
 * @brief dismiss multiroom group include distributor and receiver
 *
 * @return 0 dismiss success, -1 failed.
 */
extern int mozart_mulroom_dismiss_group(void);

/**
 * @brief append receivers into multiroom groups, only use for distributor
 *
 * @param json_str[in] added receive target devices json list
 * @param json_res[out] group succuss receive devices json list
 *
 * @return 0 append success, -1 failed.
 */
extern int mozart_mulroom_append_member(char *json_str, char **json_res);

/**
 * @brief remove receivers from multiroom groups, only use for distributor
 *
 * @param json_str[in] remove receive target devices json list
 * @param json_res[out] group left receive devieces json list
 *
 * @return 0 remove success, -1 failed.
 */
extern int mozart_mulroom_remove_member(char *json_str, char **json_res);

/**
 * @brief restore multiroom config of stored in storage device and ready device
 *
 * @return 0 restore success, -1 failed.
 */
extern int mozart_mulroom_restore_mode(void);

/**
 * @brief set multiroom system time change offset by systime change
 *
 * @param offset_sec[in] change offset second integer
 * @param offset_usec[in] change offset fractional part to micro-second
 *
 * @return 0 change success, -1 failed.
 */
extern int mozart_mulroom_ntp_change(int offset_sec, int offset_usec);

/**
 * @brief pause app command json, than control mulroom
 *
 * @param cmd_json[in] command json string
 * @param ret_json result json string
 *
 * @return 0 app command execute successs, -1 failed.
 */
extern int mozart_mulroom_app_command_handle(char *cmd_json, char **ret_json);

/**
 * @brief parse savefile multiroom mode
 *
 * @param info_file[in] multiroom state save file
 *
 * @return this device multiroom state
 */
extern enum mul_state mozart_mulroom_check_saved_mode(char *info_file);

#ifdef __cplusplus
}
#endif

#endif /* __MULROOM_INTERFACE_H__ */
