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
	CET_DOWNLOAD_VERSION,				// 正在下载版本文件,参数1为是否下载完毕,false为正在下载,true为下载完毕
	CET_DOWNLOAD_LIST_FILE,				// 正在下载列表文件,参数1为是否下载完毕,false为正在下载,true为下载完毕
	CET_START_GENERATE_LOCAL_FILE,		// 参数1本地文件总数量
	CET_GENERATING_LOCAL_FILE,			// 参数1本地文件总数量, 参数2已经查找的本地文件数量
	CET_FINISH_GENERATE_MODIFY_FILE,	// 参数1为未与服务器同步的文件,中间以%分隔
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

const int BIG_FILE_SIZE = 1024 * 1024 * 16;		// 大文件大小,文件大小大于等于该大小时认为是大文件
const std::string GAME_NAME = "dndl4.0";
const std::string REMOTE_URL = "http://app1.taxingtianji.com/gameResource/dndl/";
const std::string UPLOAD_URL = "http://app1.taxingtianji.com/wechat/gamelogtxt/php_info.php";	// 要给服务器上传错误日志的地址
//const std::string UPLOAD_URL = "http://localhost/php_info.php";								// 本地测试的地址

#endif