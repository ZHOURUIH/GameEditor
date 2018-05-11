#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "EngineDefine.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"

const float SPEED_DATA_TIME_OUT = 0.5f;

enum CORE_EVENT_TYPE
{
	CET_INFO_LOG,				// 提示信息,参数1是信息内容
	CET_ERROR_LOG,				// 错误信息,参数1是信息内容
	CET_MAX,
};

// 数据解析结果
enum PARSE_RESULT
{
	PR_SUCCESS,		// 解析成功
	PR_ERROR,		// 内容错误
	PR_NOT_ENOUGH,	// 数据不足
};

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

#endif