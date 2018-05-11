#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "EngineDefine.h"
#include "txSTLBase.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"
#include "txMemeryTrace.h"
#include "txLog.h"
#include "Utility.h"

const float TIME_OUT = 0.5f;
const int CMD_FIT = 1;
const int CMD_FRICTION = 2;
const int KEY_MACHINE = 1;
const int KEY_FRICTION = 1;

enum SOCKET_PACKET
{
	SP_SPEED,
	SP_FRICTION,
	SP_FRICTION_RET,
	SP_MAX,
};

enum CORE_EVENT_TYPE
{
	CET_INFO_LOG,				// 提示信息,参数1是信息内容
	CET_ERROR_LOG,				// 错误信息,参数1是信息内容
	CET_DEVICE_LIST_CHANGED,	// 检测到有设备连接,或者设备已断开连接,参数1为是否检测到设备
	CET_CUR_DEVICE_CHANGED,		// 选择了其他设备
	CET_DEVICE_OPENED,			// 串口被打开
	CET_DEVICE_CLOSED,			// 串口被关闭
	CET_MAX,
};

// 数据解析结果
enum PARSE_RESULT
{
	PR_SUCCESS,		// 解析成功
	PR_ERROR,		// 内容错误
	PR_NOT_ENOUGH,	// 数据不足
};

class Command;
class CommandReceiver;
struct DelayCommand
{
	DelayCommand(float delayTime, Command* cmd, CommandReceiver* receiver)
	:
	mDelayTime(delayTime),
	mCommand(cmd),
	mReceiver(receiver)
	{}
	float mDelayTime;
	Command* mCommand;
	CommandReceiver* mReceiver;
};

typedef void(*CommandCallback) (void* user_data, Command* cmd);

#endif