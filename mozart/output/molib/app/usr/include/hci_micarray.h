/* -*-     c-basic-offset:4;     indent-tabs-mode: nil     -*- */
/* ==============================================================
 * Copyright (c) 1999-2016 SinoVoice Inc.  All rights reserved.
 */
/**
 * @file	hci_micarray.h 
 * @brief	header file for hci micarray sdk.
 */

#ifndef HCI_MICARRAY_H
#define HCI_MICARRAY_H

#include "hci_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup HCI MicArray SDK 灵云麦克风阵列SDK */
/* @{ */

#if defined(_WIN32) || defined(WIN32) || defined(WIN64)
#define MicArrayAPI _stdcall
#define MicArrayAPIExport 
#else
#define MicArrayAPI
#define MicArrayAPIExport __attribute ((visibility("default")))
#endif

/// 麦克风阵列操作句柄定义
typedef void * MicArrayHandle;

/// 麦克风阵列会话句柄定义
typedef void * MicArraySession;

/// 麦克风阵列会话参数值定义
typedef void * MicArrayParamValue;

/**
 * 返回错误码定义：遵从灵云SDK的sys模块统一定义
 */
#ifndef MicArrayErrCode
#define MicArrayErrCode HCI_ERR_CODE
#endif

/**
 * MIC阵列类型
 */
#define MICARRAY_MODE_LINE                0           	///< MIC线性阵列
#define MICARRAY_MODE_CIRCULAR            1           	///< MIC圆形阵列
const static unsigned int MICARRAY_MODE_DEFAULT = 0;           ///< MIC阵列的缺省类型是线阵

/**
 * MIC通道数
 */
#define MICARRAY_MIC_CHANNELNUM_MIN              1      ///< 最少支持1个MIC通道
#define MICARRAY_MIC_CHANNELNUM_MAX              4      ///< 最多支持4个MIC通道
const static unsigned int MICARRAY_MIC_CHANNELNUM_DEFAULT = 4; ///< MIC缺省通道数是4个

/**
 * 线阵MIC间距范围（单位：毫米）
 */
#define MICARRAY_MICDISTANCE_MIN              35        ///< 线阵MIC间距最小是35mm
#define MICARRAY_MICDISTANCE_MAX              50        ///< 线阵MIC间距最大是50mm
const static unsigned int MICARRAY_MIC_DISTANCE_DEFAULT = 35;  ///< 线阵MIC缺省间距是35mm

/**
 * 圆阵MIC半径范围（单位：毫米）
 */
#define MICARRAY_MICRADIUS_MIN					30		///< 圆阵MIC半径最小是30mm
#define MICARRAY_MICRADIUS_MAX					500		///< 圆阵MIC半径最大是500mm
const static unsigned int MICARRAY_MIC_RADIUS_DEFAULT = 124;	///< 圆阵MIC缺省半径是124mm

/**
 * 参考信道通道数
 */
#define MICARRAY_REF_CHANNELNUM_MIN              0      ///< 最少支持0个REF信道
#define MICARRAY_REF_CHANNELNUM_MAX              2      ///< 最多支持2个REF信道
const static unsigned int MICARRAY_REF_CHANNELS_DEFAULT = 1;   ///< 参考缺省信道数是1个

/**
 * 音频格式类型
 */
typedef enum
{
	MICARRAY_AUDIO_FORMAT_FMT8BITS = 0,
	MICARRAY_AUDIO_FORMAT_FMT16BITS,
	MICARRAY_AUDIO_FORMAT_FMT24BITS
} MICARRAY_AUDIO_FORMAT_E;

/**
 * 音频采样率类型
 */
typedef enum
{
	MICARRAY_SAMPLE_RATE_8K = 8000,    ///< 8k pcm
	MICARRAY_SAMPLE_RATE_16K = 16000   ///< 16k pcm
} MICARRAY_SAMPLE_RATE_E;

const static unsigned int MICARRAY_SAMPLE_RATE_DEFAULT = 16000;///< 音频缺省采样频率是16K

/**
 * 数字旁瓣消除的各通道加权权值
 */
#define MICARRAY_DBF_WEIGHT_MIN		        0.0f        ///< 最小权值
#define MICARRAY_DBF_WEIGHT_MAX	            1.0f        ///< 最大权值
const static float MICARRAY_DBF_WEIGHT_DEFAULT = MICARRAY_DBF_WEIGHT_MAX;                            ///< DBF各通道缺省加权值是1.0f

/**
 * 语音回声消除采用的算法
 */
#define MICARRAY_AEC_ALGORITHM_FREQUENCY		0	///< 频域算法
#define MICARRAY_AEC_ALGORITHM_TIME_NLMS		1	///< 时域的NLMS算法
const static unsigned int MICARRAY_AEC_ALGORITHM_DEFAULT = 0;	///< AEC缺省采用算法是频域算法

/**
 * 圆阵方向计算的算法
 */
#define MICARRAY_DOA_ALGORITHM_TIME			0		///< 圆阵方向计算中对消采用的是时域方法，通过各通道时延进行估计,这是最新的算法
#define MICARRAY_DOA_ALGORITHM_FREQUENCY	1		///< 圆阵方向计算中对消采用的是频域方法，这是老的计算方法（r2057)
const static int MICARRAY_DOA_ALGORITHM_DEFAULT = 0;		///< 圆阵方向计算的算法缺省值为0

#define MICARRAY_WAKECHECK_INDEX_INVALID		(-1)

/**
 * 唤醒检测模式定义
 */
typedef enum
{
	MICARRAY_WAKECHECK_MODE_STOP = 0,			///< 停止唤醒检测
	MICARRAY_WAKECHECK_MODE_ONESHOT,			///< 一次性唤醒检测
	MICARRAY_WAKECHECK_MODE_CONTINUOUS			///< 持续性唤醒检测
} MICARRAY_WAKECHECK_MODE_E;

/**
 * 唤醒状态定义
 */
typedef enum
{
	MICARRAY_WAKESTATUS_SLEEPING = 0,			///< 无需唤醒
	MICARRAY_WAKESTATUS_WAKECHECKING,			///< 唤醒检测
	MICARRAY_WAKESTATUS_WAKEDUP					///< 已唤醒
} MICARRAY_WAKESTATUS_E;

/**
 * 事件类型定义
 */
typedef enum
{
	MICARRAY_EVENT_RECORD_STARTED = 1,			///<  1: 开始录音
	MICARRAY_EVENT_RECORD_STOPPED,				///<  2: 停止录音
	MICARRAY_EVENT_DEVICE_WAKEDUP,				///<  3: 设备唤醒，如果不关注唤醒词序号，可使用该事件
	MICARRAY_EVENT_DEVICE_SLEPT,				///<  4: 设备休眠
	MICARRAY_EVENT_AECMODE_ENABLED,				///<  5: AEC模式开启
	MICARRAY_EVENT_AECMODE_DISABLED,			///<  6: AEC模式关闭
	MICARRAY_EVENT_AGCMODE_ENABLED,				///<  7: AGC模式开启
	MICARRAY_EVENT_AGCMODE_DISABLED,			///<  8: AGC模式关闭
} MICARRAY_EVENT_E;

/**
 * 会话参数类型
 */
typedef enum 
{
	MICARRAY_PARAM_LOG_LEVEL = 1,				///<  1: logLevel, 麦克风阵列SDK日志级别，[1, 4]，可读写

	MICARRAY_PARAM_MIC_VOLUME = 11,				///< 11: micVolume, MIC语音信号的音量，[0, 100]，可读写
	MICARRAY_PARAM_REF_VOLUME,					///< 12: refVolume, REF参考信号的音量，[0, 100]，可读写
	MICARRAY_PARAM_SPEAKER_VOLUME,				///< 13: speakerVolume, 喇叭播放语音的音量，[0, 100]，可读写

	MICARRAY_PARAM_WAKE_CHECK_MODE = 21,		///< 21: wakeCheckMode, 唤醒模式设置：0，不唤醒；1，一次性唤醒；2，持续性唤醒，可读写
	MICARRAY_PARAM_WAKE_STATUS,					///< 22: wakeStatus, 当前唤醒状态：0，无需唤醒；1，唤醒检测中；2，已唤醒。只读

	MICARRAY_PARAM_AEC_STATUS = 31,				///< 31: aecStatus, 语音回声消除状态：0，关闭AEC；1，开启AEC，可读写
	MICARRAY_PARAM_AGC_STATUS,					///< 32: agcStatus, 自动增益控制状态：0，关闭AGC；1，开启AGC，可读写
	MICARRAY_PARAM_DOA_STATUS,					///< 33: doaStatus, 声源定向估算状态：0，关闭DOA；1，开启DOA，可读写

	MICARRAY_PARAM_VQE_DIRECTION = 41,	        ///< 41: vqeDirection, 定向增强角度，线阵范围：[0, 180]；圆阵范围：[0, 360]，可读写
	MICARRAY_PARAM_CURRENT_DIRECTION,			///< 42: currentDirection, 当前语音定向角度，线阵角度范围：[0, 180]，圆阵角度范围：[0, 360]，只读

	MICARRAY_PARAM_CALLBACK_STATEUPGRADED = 91,	///< 91: 系统状态变更回调， hci_micarray_statechange_callback，可读写
    MICARRAY_PARAM_CALLBACK_WAKEUP,				///< 92: 唤醒回调， hci_micarray_wakeup_callback，可读写
    MICARRAY_PARAM_CALLBACK_WAKEDIRECTION,		///< 93: 唤醒角度回调， hci_micarray_wakedirection_callback，可读写
    MICARRAY_PARAM_CALLBACK_VOICEREADY,			///< 94: 音频数据就绪回调， hci_micarray_voiceready_callback，可读写
} MICARRAY_PARAM_E;

	//////////////////////////////////////////////////////////////////////////
	// 数据结构定义

	/**
	 * @brief	语音唤醒音结果
	 */
	typedef struct MICARRAY_WAKE_RESULT_T
	{
		int nWakeIndex;		///< 唤醒词序号：如1表示第一个唤醒词
		int nDetectTime;	///< 检测到的唤醒词时间, 单位为ms，暂未用到
	} MICARRAY_WAKE_RESULT;
	
	// 状态变更回调：系统状态变更后，SDK会调用该回调函数通知用户新的系统状态
	typedef HCI_ERR_CODE (* hci_micarray_statechange_func)(
		_OPT_ _IN_ void	*pUserContext,					// [in] 用户上下文参数
		_MUST_ _IN_ MICARRAY_EVENT_E state);			// [in] 系统状态

	// 唤醒回调：在检测到唤醒词后，SDK会调用该回调函数通知用户已唤醒
	typedef HCI_ERR_CODE (* hci_micarray_wakeup_func)(
		_OPT_ _IN_ void *pUserContext,					// [in] 用户上下文参数
		_MUST_ _IN_ MICARRAY_WAKE_RESULT *pWakeResult);	// [in] 唤醒结果

	// 方向回调：在检测到唤醒词后，SDK会调用该回调函数通知用户声源方位
	typedef HCI_ERR_CODE (* hci_micarray_wakedirection_func)(
		_OPT_ _IN_ void *pUserContext,					// [in] 用户上下文参数
		_MUST_ _IN_ int nDirection);					// [in] 唤醒时的方向[0, 360)

	// 音频处理结果回调，将处理完的单路音频返回给应用层
	typedef HCI_ERR_CODE (* hci_micarray_voiceready_func)(
		_OPT_ _IN_ void *pUserContext,					// [in] 用户上下文参数
		_MUST_ _IN_ short *pVoiceData,					// [in] 输出音频流
		_MUST_ _IN_ int nVoiceSampleCount);				// [in] 输出音频流中包含的采样点个数

/**
*	@brief	 初始化麦克风阵列模块操作句柄
*	@details 麦克风阵列模块初始化。传入的pHandle不能为NULL，否则返回 HCI_ERR_PARAM_INVALID ；
* 		pHandle指针保存的是初始化成功后的操作句柄，调用者需要管理和维护pHandle对象。
*
*	@param pszConfigFile:麦克风阵列SDK配置文件路径
*	@param pHandle:	 操作句柄地址
*	@return
* @n
*	<table>
*		<tr><td>@ref HCI_ERR_NONE</td><td>正常</td></tr>
*		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置存在错误</td></tr>
*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*		<tr><td>@ref HCI_ERR_OUT_OF_MEMORY</td><td>内存分配失败</td></tr>
*		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI_SYS未初始化</td></tr>
*		<tr><td>@ref HCI_ERR_CONFIG_UNSUPPORT</td><td>配置项不支持</td></tr>
*		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>没有找到指定的能力</td></tr>
*	</table>
* @n@n
*
* @note
*  在麦克风阵列初始化时，通过配置文件初始化的配置参数信息将会继承到后面的会话中，除非在会话中对这些参数进行重新设置
* @n@n
* 
*/
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_init(
	_MUST_ _IN_ const char *pszConfigFile,
	_MUST_ _OUT_ MicArrayHandle *pHandle
    );

/**
*	@brief	创建麦克风阵列会话
*	@details 创建麦克风阵列会话
* 
*	@param handle:	模块操作句柄
*	@param scf:		状态变更事件回调
* 	@param wuf:		唤醒通知事件回调
* 	@param wdf:		唤醒角度事件回调
* 	@param vrf:		音频数据就绪回调
*	@param pSession: 会话句柄地址
*	@return
* @n
*	<table>
*		<tr><td>@ref HCI_ERR_NONE</td><td>正常</td></tr>
*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置串参数错误</td></tr>
*		<tr><td>@ref HCI_ERR_OUT_OF_MEMORY</td><td>内存分配失败</td></tr>
*	</table>
* @n@n
*
*/
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_session_start(
	_MUST_ _IN_ const MicArrayHandle handle,
	_MUST_ _IN_ hci_micarray_statechange_func scf,
	_MUST_ _IN_ hci_micarray_wakeup_func wuf,
	_MUST_ _IN_ hci_micarray_wakedirection_func wdf,
	_MUST_ _IN_ hci_micarray_voiceready_func vrf,
	_MUST_ _OUT_ MicArraySession *pSession
    );

/**  
* @brief	会话参数获取
* @details	获取会话参数
* 
* @param    session		会话句柄
* @param	paramKey	要获取的参数类型
* @param	paramValue	获取参数输出缓存
* @return
* @n
*	<table>
*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*		<tr><td>@ref HCI_ERR_UNSUPPORT</td><td>暂不支持</td></tr>
*	</table>
* @n@n
*
*/ 	
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_session_getparam(
	_MUST_ _IN_ MicArraySession session,
	_MUST_ _IN_ MICARRAY_PARAM_E paramKey, 
	_MUST_ _OUT_ MicArrayParamValue *paramValue);

/**  
* @brief	会话参数设置
* @details	设置会话参数
* 
* @param    session		会话句柄
* @param	paramKey	要获取的参数类型
* @param	paramValue	获取参数输出缓存
* @return
* @n
*	<table>
*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*		<tr><td>@ref HCI_ERR_UNSUPPORT</td><td>暂不支持</td></tr>
*	</table>
* @n@n
*
*/ 	
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_session_setparam(
	_MUST_ _IN_ MicArraySession session,
	_MUST_ _IN_ MICARRAY_PARAM_E paramKey, 
	_MUST_ _IN_ MicArrayParamValue paramValue);

/**  
* @brief	会话参数显示
* @details	显示会话参数
* 
* @param    session		会话句柄
* @return
* @n
*	<table>
*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
*	</table>
* @n@n
*
*/ 	
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_session_printparams(
	_MUST_ _IN_ MicArraySession session);

/**
*	@brief   启动麦克风录音
*	@details 启动麦克风录音
*
*	@param session:	 会话句柄
*	@return
* @n
*	<table>
*		<tr><td>@ref MICARRAY_ERR_NONE</td><td>正常</td></tr>
*		<tr><td>@ref MICARRAY_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*	</table>
* @n@n
*
*/
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_session_enablerecording(
	_MUST_ _IN_ MicArraySession session
    );

/**
*	@brief   关闭麦克风录音
*	@details 关闭麦克风录音
*
*	@param session:	 会话句柄
*	@return
* @n
*	<table>
*		<tr><td>@ref MICARRAY_ERR_NONE</td><td>正常</td></tr>
*		<tr><td>@ref MICARRAY_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*	</table>
* @n@n
*
*/
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_session_disablerecording(
	_MUST_ _IN_ MicArraySession session
    );

/**  
*   @brief		重置麦克风阵列会话
*   @details	重置麦克风阵列会话
* 
*	@param session:	 会话句柄
*	@return
* @n
*	<table>
*		<tr><td>@ref HCI_ERR_NONE</td><td>正常</td></tr>
*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*		<tr><td>@ref HCI_ERR_OUT_OF_MEMORY</td><td>内存分配失败</td></tr>
*	</table>
* @n@n
*
*/
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_session_reset(
	_MUST_ _IN_ MicArraySession session
	);

/**
*	@brief   停止麦克风阵列会话
*	@details 停止麦克风阵列会话
*
*	@param pSession: 会话句柄
*	@return
* @n
*	<table>
*		<tr><td>@ref MICARRAY_ERR_NONE</td><td>正常</td></tr>
*		<tr><td>@ref MICARRAY_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*	</table>
* @n@n
*
*/
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_session_stop(
	_MUST_ _IN_OUT_ MicArraySession *pSession
    );

/**
*	@brief	 释放麦克风阵列模块操作句柄
*	@details 释放麦克风阵列模块操作句柄。传入的pHandle不能为NULL，否则返回 HCI_ERR_PARAM_INVALID ；
*
*	@param engineHandle: 要释放的麦克风阵列模块句柄
*	@return
* @n
*	<table>
*		<tr><td>@ref HCI_ERR_NONE</td><td>正常</td></tr>
*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>参数错误</td></tr>
*	</table>
* @n@n
*
*/
MicArrayAPIExport MicArrayErrCode MicArrayAPI hci_micarray_release(
	_MUST_ _IN_OUT_ MicArrayHandle *pHandle
    );

/* @} */
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // HCI_MICARRAY_H

