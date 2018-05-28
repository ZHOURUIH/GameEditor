#ifndef _EC_LOG_H_
#define _EC_LOG_H_

#include "txEngineLog.h"

#include "ECUtility.h"

// 向界面层发送错误信息提示
#define EDITOR_CORE_ERROR(...)										\
{																	\
	char buffer[2048];												\
	SPRINTF(buffer, 2048, __VA_ARGS__);								\
	LOGW(__VA_ARGS__);												\
	ECUtility::logError(std::string(buffer) + " " + _FILE_LINE_);	\
}

#endif