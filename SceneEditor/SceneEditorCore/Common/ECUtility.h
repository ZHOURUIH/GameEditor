#ifndef _EC_UTILITY_H_
#define _EC_UTILITY_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

#include "EditorCoreBase.h"

class ECUtility : public EditorCoreBase, public txEngineBase
{
public:
	// 在Utility中添加logError是避免在其他地方都包含EditorCoreRoot,防止头文件互相包含
	static void logError(const std::string& info);
	static int getScreenWidth();
	static int getScreenHeight();
	static VECTOR2 getScreenSize();
};

#endif