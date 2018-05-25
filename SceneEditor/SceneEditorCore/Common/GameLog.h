#ifndef _GAME_LOG_H_
#define _GAME_LOG_H_

#include "ECUtility.h"

// 向界面层发送错误信息提示
#define GAME_ERROR(...)																				\
{																									\
	char buffer[2048];																				\
	SPRINTF(buffer, 2048, __VA_ARGS__);																\
	ENGINE_ERROR(__VA_ARGS__);																		\
	std::string file(__FILE__);																		\
	file = file.substr(file.find_last_of('\\') + 1, file.length() - file.find_last_of('\\') - 1);	\
	char fileLine[256];																				\
	SPRINTF(fileLine, 256, ",file : %s, line : %d", file.c_str(), __LINE__);						\
	ECUtility::logError(std::string(buffer) + std::string(fileLine));								\
}

#endif