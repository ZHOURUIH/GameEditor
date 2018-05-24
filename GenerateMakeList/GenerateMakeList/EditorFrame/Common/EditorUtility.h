#ifndef _EDITOR_UTILITY_H_
#define _EDITOR_UTILITY_H_

#include "EditorBase.h"
#include "CommonDefine.h"

class EditorUtility : public EditorBase
{
public:
	static void logError(const std::string& info);
	static void logInfo(const std::string& info);
	static int askYesNODialog(const std::string& info);
};

#endif