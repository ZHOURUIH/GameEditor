#include "LogSystem.h"
#include "CustomThread.h"
#include "txSerializer.h"
#include "GameConfig.h"

LogSystem::LogSystem()
{
	mCustomThread = TRACE_NEW(CustomThread, mCustomThread, "LogSystem");
	mOutputLog = true;
}

void LogSystem::init()
{
	// 根据配置文件中判断是否需要输出日志
	mOutputLog = (int)GameConfig::getFloatParam(GDF_OUTPUT_PACKET_LOG) != 0;
	if (!mOutputLog)
	{
		return;
	}
	mCustomThread->setBackground(false);
	mCustomThread->start(onLogThread, this, 30);
}

void LogSystem::destroy()
{
	TRACE_DELETE(mCustomThread);
	mLogBuffer.merge(mLogList);
	int count = mLogBuffer.size();
	FOR_STL(mLogBuffer, int i = 0; i < count; ++i)
	{
		TRACE_DELETE(mLogBuffer[i]);
	}
	END_FOR_STL(mLogBuffer);
	mLogBuffer.clear();
}

void LogSystem::log(const std::string& info)
{
	if (!mOutputLog)
	{
		return;
	}
	LOCK(mLogLock);
	LogInfo* logInfo = TRACE_NEW(LogInfo, logInfo);
	logInfo->mTime = txUtility::getTime();
	logInfo->mInfo = info;
	mLogList.push_back(logInfo);
	UNLOCK(mLogLock);
}

bool LogSystem::onLogThread(void* args)
{
	LogSystem* logSystem = (LogSystem*)args;
	// 先同步列表
	LOCK(logSystem->mLogLock);
	logSystem->mLogBuffer.merge(logSystem->mLogList);
	logSystem->mLogList.clear();
	UNLOCK(logSystem->mLogLock);
	// 将所有日志写入文件
	int count = logSystem->mLogBuffer.size();
	if (count > 0)
	{
		txSerializer serializer;
		FOR_STL(logSystem->mLogBuffer, int i = 0; i < count; ++i)
		{
			std::string infoString = logSystem->mLogBuffer[i]->mTime + " : " + logSystem->mLogBuffer[i]->mInfo + "\r\n";
			serializer.writeBuffer((char*)infoString.c_str(), infoString.length());
			TRACE_DELETE(logSystem->mLogBuffer[i]);
		}
		END_FOR_STL(logSystem->mLogBuffer);
		logSystem->mLogBuffer.clear();
		FileUtility::writeFile("./log.txt", serializer.getDataSize(), serializer.getBuffer(), true);
	}
	return true;
}