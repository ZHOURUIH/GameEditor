#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include <windows.h>
#include <string>

const float SPEED_DATA_TIME_OUT = 0.5f;

enum SOCKET_PACKET
{
	SP_SPEED,
	SP_CARD,
	SP_FRICTION,
	SP_FRICTION_RET,
	SP_HEART_RATE,
	SP_COLOUR,
	SP_MAX,
};

enum CORE_EVENT_TYPE
{
	CET_INFO_LOG,				// 提示信息,参数1是信息内容
	CET_ERROR_LOG,				// 错误信息,参数1是信息内容
	CET_DEVICE_CHANGED,			// 检测到有设备连接,或者设备已断开连接,参数1为是否检测到设备
	CET_RECEIVE_DATA,			// 接收到数据,参数1是接收的数据,参数2是接收数据的长度
	CET_SEND_DATA,				// 发送了数据,参数1是发送的数据,参数2是发送数据的长度
	CET_HEART_RATE_TIME,		// 心跳超时时间改变
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


#endif