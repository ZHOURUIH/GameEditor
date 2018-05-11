#ifndef _TX_ENGINE_LOG_H_
#define _TX_ENGINE_LOG_H_

#include "txEngineDefine.h"

#if RUN_PLATFORM == PLATFORM_ANDROID
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "txtj", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "txtj", __VA_ARGS__))
#elif RUN_PLATFORM == PLATFORM_WINDOWS
#define LOGI(...)												\
{																\
	char buffer[2048];											\
	SPRINTF(buffer, 2048, __VA_ARGS__);							\
	OutputDebugStringA((std::string(buffer) + "\n").c_str());	\
}
#define LOGW(...)												\
{																\
	char buffer[2048];											\
	SPRINTF(buffer, 2048, __VA_ARGS__);							\
	OutputDebugStringA((std::string(buffer) + "\n").c_str());	\
}
#endif

#define _FILE_LINE_ "file : " + txStringUtility::getFileName(__FILE__) + ", line : " + txStringUtility::intToString(__LINE__)

#define ENGINE_ERROR(...)																			\
{																									\
	char buffer[2048];																				\
	SPRINTF(buffer, 2048, __VA_ARGS__);																\
	LOGW(__VA_ARGS__);																				\
	txUtility::logError(std::string(buffer) + " " + _FILE_LINE_);									\
}

#define ENGINE_INFO(...)																			\
{																									\
	char buffer[2048];																				\
	SPRINTF(buffer, 2048, __VA_ARGS__);																\
	LOGW(__VA_ARGS__);																				\
}

#endif