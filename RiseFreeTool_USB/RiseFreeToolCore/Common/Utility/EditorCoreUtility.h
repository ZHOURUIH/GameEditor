#ifndef _EDITOR_CORE_UTILITY_H_
#define _EDITOR_CORE_UTILITY_H_

#include "EngineDefine.h"

class EditorCoreUtility
{
public:
	static void logError(const std::string& info, const bool& delay = false);
	static void logInfo(const std::string& info, const bool& delay = false);
};

#endif