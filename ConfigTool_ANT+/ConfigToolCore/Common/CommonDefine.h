#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "EngineDefine.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"

const float SPEED_DATA_TIME_OUT = 0.5f;

enum SOCKET_PACKET
{
	SP_SPEED,
	SP_FRICTION,
	SP_FRICTION_RET,
	SP_HEART_RATE,
	SP_COLOUR,
	SP_MAX,
};

enum CORE_EVENT
{
	CE_INFO_LOG,			// 提示信息,参数1是信息内容
	CE_ERROR_LOG,			// 错误信息,参数1是信息内容
	CE_HEART_RATE_TIME,		// 心跳超时时间改变
	CE_REGISTE_DEVICE_LIST_MODIFIED,	// 设备绑定信息列表修改,参数1为是否修改以后未保存
	CE_UPLOAD_STATE,		// 设备绑定信息上传状态,参数1为状态
	CE_US_UNUPLOAD,			// 设备绑定信息上传完成,参数1为状态
	CE_UPLOAD_FAILED,		// 设备绑定信息上传失败,参数1为状态
	CE_MAX,
};

enum LIBCURL_PACKET
{
	LP_SETUP_DEVICE,
	LP_MAX,
};

enum HTTP_METHOD
{
	HM_GET,
	HM_POST,
};

enum UPLOAD_STATE
{
	US_NONE,
	US_UNUPLOAD,
	US_UPLOADING,
	US_UPLOADED,
	US_UPLOAD_FAILED,
};

// 数据解析结果
enum PARSE_RESULT
{
	PR_SUCCESS,		// 解析成功
	PR_ERROR,		// 内容错误
	PR_NOT_ENOUGH,	// 数据不足
};

enum ANT_PLUS_PACKET_TYPE
{
	APPT_NONE,
	APPT_HEART_BEAT,	// 接收器心跳
	APPT_HEART_RATE,	// 心率
	APPT_CADENCE,		// 单踏频
	APPT_SPEED,			// 单速度
	APPT_CADENCE_SPEED,	// 速度踏频
};

enum PACKET_CMD
{
	PC_RIDING = 0x01,
	PC_HEART_BEAT = 0x02,
};

enum RIDING_DEVICE
{
	RD_NONE,
	RD_HEART_RATE = 0x78,
	RD_CADENCE_SPEED = 0x79,
	RD_CADENCE = 0x7A,
	RD_SPEED = 0x7B,
};

const float PACKET_TIME_OUT = 3.0f;

class txCommand;
class txCommandReceiver;
struct DelayCommand
{
	DelayCommand(float delayTime, txCommand* cmd, txCommandReceiver* receiver)
	:
	mDelayTime(delayTime),
	mCommand(cmd),
	mReceiver(receiver)
	{}
	float mDelayTime;
	txCommand* mCommand;
	txCommandReceiver* mReceiver;
};

typedef void(*CommandCallback) (void* user_data, txCommand* cmd);

const std::string EMPTY_STRING = "";

#endif