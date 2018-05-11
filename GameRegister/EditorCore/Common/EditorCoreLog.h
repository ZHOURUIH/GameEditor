#ifndef _EDITOR_CORE_LOG_H_
#define _EDITOR_CORE_LOG_H_

#include "txLog.h"

#include "EditorCoreUtility.h"

// 向界面层发送错误信息提示
#define EDITOR_CORE_ERROR(...)																		\
{																									\
	char buffer[2048];																				\
	sprintf_s(buffer, 2048, __VA_ARGS__);															\
	LOGW(__VA_ARGS__);																				\
	std::string file(__FILE__);																		\
	file = file.substr(file.find_last_of('\\') + 1, file.length() - file.find_last_of('\\') - 1);	\
	char fileLine[256];																				\
	sprintf_s(fileLine, 256, ",file : %s, line : %d", file.c_str(), __LINE__);						\
	EditorCoreUtility::logError(std::string(buffer) + std::string(fileLine));						\
}

#define EDITOR_CORE_INFO(...)																		\
{																									\
	char buffer[2048];																				\
	sprintf_s(buffer, 2048, __VA_ARGS__);															\
	EditorCoreUtility::logInfo(std::string(buffer));												\
}

#endif