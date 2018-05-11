#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "EditorCoreUtility.h"
#include "EngineDefine.h"
#include "txSTLBase.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"
#include "txMemeryTrace.h"
#include "txLog.h"

const float TIME_OUT = 0.5f;

enum SOCKET_PACKET
{
	SP_MAX,
};

enum LIB_CURL_PACKET
{
	LCP_MAX,
};

enum HTTP_METHOD
{
	HM_GET,
	HM_POST,
};

enum CORE_EVENT_TYPE
{
	CET_ERROR_LOG,
	CET_INFO_LOG,
	CET_MAX,
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

#define DEBUG_EMPTY "%s", ""

#define COMMAND_PARAM __FILE__, __LINE__

#define COMMAND_CONSTRUCT(className, baseCommand)					\
	className(const char* file, int line, bool showInfo = true)		\
	:																\
	baseCommand(file, line, showInfo)

#define COMMAND_TOOL_CORE_CONSTRUCT(className) COMMAND_CONSTRUCT(className, ToolCoreCommand)

#define COMMAND_DECLARE_FUNCTION					\
	virtual void execute();							\
	virtual std::string showDebugInfo();

#define COMMAND_DEBUG(...)																\
{																						\
	static char strBuf[256];															\
	sprintf_s(strBuf, 256, __VA_ARGS__);												\
	std::string cmdName = typeid(*this).name();											\
	cmdName = cmdName.substr(strlen("class "), cmdName.length() - strlen("class "));	\
	std::string str = cmdName;															\
	if (strlen(strBuf) > 0)																\
	{																					\
		str += std::string(" : ") + strBuf;												\
	}																					\
	return str;																			\
}

#define TOSTRING(t) #t

#endif