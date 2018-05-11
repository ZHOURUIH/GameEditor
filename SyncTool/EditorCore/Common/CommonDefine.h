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
	CET_START_DOWNLOAD_VERSION,		// 正在下载版本号文件
	CET_START_DOWNLOAD_LIST_FILE,	// 正在下载列表文件
	CET_NEW_VERSION,		// 参数1为是否有新版本可以更新
	CET_START_DOWNLOAD,		// 参数1为文件名
	CET_FINISH_DOWNLOAD,	// 参数1为文件名
	CET_START_GENERATE_LOCAL_FILE,	// 参数1文件总数量
	CET_GENERATING_LOCAL_FILE,		// 参数1文件总数量, 参数2已经查找的文件数量
	CET_FINISH_GENERATE_LOCAL_FILE,
	CET_UPDATING_FILE,		// 正在更新所有文件
	CET_UPDATING_PROGRESS,	// 更新文件的进度
	CET_UPDATE_DONE,
	CET_CANCEL_UPDATE,
	CET_NOTHING_UPDATE,
	CET_START_UPLOAD,		// 参数1是文件名
	CET_UPLOADING_FILE,		// 参数1是文件名
	CET_FINISH_UPLOAD,		// 参数1是文件名,参数2是是否成功
	CET_ALL_UPLOADED,
	CET_STOP_PROGRAM,		// 退出程序
	CET_MAX,
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

const int BIG_FILE_SIZE = 1024 * 1024 * 16;		// 大文件大小,文件大小大于等于该大小时认为是大文件
const std::string GAME_NAME = "dndl";			// 固定的游戏名称,即使版本更新也不会修改
const std::string REMOTE_URL = "http://app1.taxingtianji.com/gameResource/dndl/";
const std::string UPLOAD_URL = "http://app1.taxingtianji.com/wechat/gamelogtxt/php_info.php";	// 要给服务器上传错误日志的地址
//const std::string UPLOAD_URL = "http://localhost/php_info.php";								// 本地测试的地址

#endif