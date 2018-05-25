#ifndef _EDITOR_UTILITY_H_
#define _EDITOR_UTILITY_H_

#include "Utility.h"

#include "wxEditorHeader.h"

class EditorUtility
{
public:
	// 获得media的绝对路径
	static std::string getAbsoluteMediaPath()
	{
		char chpath[MAX_PATH];
		GetModuleFileNameA(NULL, chpath, sizeof(chpath));
		std::string path(chpath);
		path = txStringUtility::getFilePath(path);
		path = path + "/" + txUtility::getMediaPath();
		txStringUtility::rightToLeft(path);
		return path;
	}
	static void launchDefaultApplication(const std::string& pathInMedia)
	{
		wxLaunchDefaultApplication(getAbsoluteMediaPath() + "/" + pathInMedia);
	}
	static void launchDefaultBrowser(const std::string& pathInMedia)
	{
		wxLaunchDefaultBrowser(getAbsoluteMediaPath () + "/" + pathInMedia);
	}
};

#endif