#ifndef __ALARM_INTERFACE_H__
#define __ALARM_INTERFACE_H__

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief alarm private infomation.
 */
typedef struct alarm_private_info {
	/**
	 * @brief the length of info.
	 */
	int len;
	/**
	 * @brief alarm private infomation.
	 */
	char info[0];
} alarm_private_info_t;

/**
 * @brief alarm infomation.
 */
typedef struct alarm_info {
	/**
	 * @brief the number of hours past midnight, in the range 0 to 23.
	 */
	int hour;
	/**
	 * @brief the number of minutes after the hour, in the range 0 to 59.
	 */
	int minute;
	/**
	 * @brief the alarm day of a week.
	 * Mon : 0x1
	 * Tue : 0x2
	 * Wed : 0x4
	 * Thu : 0x8
	 * Fri : 0x10
	 * Sat : 0x20
	 * Sun : 0x40
	 */
	int week_active;
	/**
	 * @brief repeat every week, 1 : repeat 0 : no-repeat.
	 */
	int weekly_repeat;
	/**
	 * @brief enable the alarm, 1 : enable 0 : disable.
	 */
	int enable;
	/**
	 * @brief the volume of alarm, int the range 0 to 100.
	 */
	int alarm_id;
	/**
	 * @brief the timestamp of alarm.
	 */
	time_t timestamp;
	/**
	 * @brief the unique name of the client.
	 */
	char name[32];
	/**
	 * @brief the private info of alarm.
	 */
	struct alarm_private_info prv_info;
} ALARM_INFO;

/**
 * @brief alarm-get and alarm-set command.
 */
typedef enum alarm_cmd {
	/**
	 * @brief add a new alarm.
	 */
	ADD_ALARM = 0,
	/**
	 * @brief delete a alarm.
	 */
	DELETE_ALARM,
	/**
	 * @brief update a alarm.
	 */
	UPDATE_ALARM,
	/**
	 * @brief get alarm list.
	 */
	GET_ALARM_LIST,
	/**
	 * @brief start alarm.
	 */
	START_ALARM,
	/**
	 * @brief stop alarm.
	 */
	STOP_ALARM,
	/**
	 * @brief get rtc time.
	 */
	GET_RTC_TIME,
	/**
	 * @brief set rtc time.
	 */
	SET_RTC_TIME,
	/**
	 * @brief check wakeup status.
	 */
	CHECK_WAKEUP_STATUS
} alarm_cmd_t;

/**
 * @brief alarm request ctrl msg
 */
typedef struct alarm_ctl_msg_s {
	/**
	 * @brief alarm request command.
	 */
	alarm_cmd_t cmd;
	/**
	 * @brief parameter
	 * ADD_ALARM,UPDATE_ALARM,DELETE_ALARM: len is alarm info length.
	 * OPEN_RTC: len is open mode
	 * CLOSE_RTC: len is close fd
	 * Otherwise it is 0.
	 */
	long len;
} alarm_ctl_msg_t;

/**
 * @brief register infomation to alarm_manager.
 */
typedef struct alarm_client_register {
	/**
	 * @brief the pid of client
	 */
	long pid;
	/**
	 * @brief priority of client, Reserved.
	 */
	int priority;
	/**
	 * @brief the unique name of the client.
	 */
	char name[32];
	/**
	 * @brief remove from cli_list
	 */
	int remove;
	/**
	 * @brief notify socket fd
	 */
	int client_sockfd;
} ALARM_CLI_REG;

/**
 * @brief register to alarm_manager
 *
 * @param ptr [in] callback func on alarm mode changed.
 *
 * @return return 0 on Success, -1 on failure.
 */
extern int register_to_alarm_manager(int (*ptr)(ALARM_INFO * alarm));

/**
 * @brief un register from alarm_manager
 *
 * @return return 0 on Success, -1 on failure.
 */
extern int unregister_to_alarm_manager(void);

/**
 * @brief add a new alarm
 *
 * @param alarm [in] the new alarm info.
 * without entering alarm_id and timestamp, it is set in alarm_manager.
 *
 * @return return 0 on Success, -1 on failure.
 */
extern int mozart_alarm_add(struct alarm_info *alarm);

/**
 * @brief delete a alarm
 *
 * @param alarm [in] the delete alarm info.
 * First, get alarm list to have the correct alarm_id,
 * Second, delete the alarm rely on alarm_id.
 *
 * @return return 0 on Success, -1 on failure.
 */
extern int mozart_alarm_delete(struct alarm_info *alarm);

/**
 * @brief update the parameter of alarm
 *
 * @param alarm [in] the update alarm info.
 * First, get alarm list to have the correct alarm_id,
 * Second, update the alarm rely on alarm_id.
 *
 * @return return 0 on Success, -1 on failure.
 */
extern int mozart_alarm_update(struct alarm_info *alarm);

/**
 * @brief get alarm list
 * [ { "hour": 15, "minute": 9, "week_active": 123, "weekly_repeat": 0, "enable": 1,
 *     "alarm_id": 0, "timestamp": 1467788940, "name": "alarm_test",
 *     "alarm_private_info":
 *			{ "length": 47,
 *			  "private_info":
 *				{ "volume": 50, "url": "\/mnt\/sdcard\/1.mp3" }
 *			}
 * } ]
 * @return return alarm json on Success, NULL on failure.
 */
extern char *mozart_alarm_get_list(void);

/**
 * @brief start alarm
 *
 * @return return 0 on Success, -1 on failure.
 */
extern int mozart_alarm_start_alarm(void);

/**
 * @brief stop alarm
 *
 * @return return 0 on Success, -1 on failure.
 */
extern int mozart_alarm_stop_alarm(void);

/**
 * @brief get rtc time
 *
 * @return return local timestamp on Success, -1 on failure.
 */
extern long mozart_alarm_get_rtc_time(void);

/**
 * @brief set rtc time
 *
 * @param timestamp [in] the new local timestamp.
 *
 * @return return 0 on Success, -1 on failure.
 */
extern int mozart_alarm_set_rtc_time(long timestamp);

/**
 * @brief check wakeup status
 *
 * @return return alarm info on Success, NULL on failure.
 */
extern struct alarm_info *mozart_alarm_check_wakeup_status(void);

#ifdef  __cplusplus
}
#endif

#endif /* __ALARM_INTERFACE_H__ */
