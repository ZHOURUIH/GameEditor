#ifndef _TX_UTILITY_H_
#define _TX_UTILITY_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txUtility : public txEngineBase
{
public:
	// 获得当前windows系统
	static WINDOWS_SYSTEM getWindowsSystem();
	static void copyTextToClipbord(const std::string& text);
	static std::string txUtility::getTime();
	static void logError(const std::string& info);
	static void sleep(const unsigned long& timeMS);
	static unsigned long getTimeMS();

	// 返回media的路径,不带/
	static std::string getMediaPath()
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		return "../media";
#elif RUN_PLATFORM == PLATFORM_ANDROID
		// 如果从AssetManager中加载资源,则media目录为空字符串
#ifdef LOAD_FROM_ASSETMANAGER
		return EMPTY_STRING;
#else
		return "/sdcard/txtj";
#endif
#endif
	}
};

#endif
