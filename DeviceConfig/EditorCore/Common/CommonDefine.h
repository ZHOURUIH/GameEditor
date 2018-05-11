#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "EditorCoreUtility.h"
#include "EngineDefine.h"
#include "txSTLBase.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"

enum SOCKET_PACKET
{
	SP_MAX,
};

enum CORE_EVENT_TYPE
{
	CET_ERROR_LOG,
	CET_INFO_LOG,
	CET_GYM_QUERY_RET,
	CET_GYM_REGISTED,
	CET_MAX,
};

enum LIB_CURL_PACKET
{
	LCP_QUERY_GYM,
	LCP_REGISTE_GYM,
	LCP_MAX,
};

enum HTTP_METHOD
{
	HM_GET,
	HM_POST,
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

const std::string COMPANY_NAME = "北京踏行天际科技发展有限公司";
const std::string GAME_NAME = "电能动力4.0";
const std::string REGISTE_KEY = "电能动力4.3";
const std::string KEY_NAME = "REGISTE_CODE";

#endif