#ifndef _LOG_SYSTEM_H_
#define _LOG_SYSTEM_H_

#include "ToolCoreBase.h"
#include "Utility.h"

class LogInfo
{
public:
	std::string mInfo;
	std::string mTime;
};

class CustomThread;
class LogSystem : ToolCoreBase
{
public:
	LogSystem();
	~LogSystem(){ destroy(); }
	void init();
	void destroy();
	void log(const std::string& info);
protected:
	static bool onLogThread(void* args);
protected:
	txVector<LogInfo*> mLogList;
	txVector<LogInfo*> mLogBuffer;
	ThreadLock mLogLock;
	CustomThread* mCustomThread;
	bool mOutputLog;
};

#endif