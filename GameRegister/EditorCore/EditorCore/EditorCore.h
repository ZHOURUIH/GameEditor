#ifndef _CONFIG_TOOL_CORE_H_
#define _CONFIG_TOOL_CORE_H_

#include <vector>
#include <windows.h>

#include "txSingleton.h"

class CTEventSystem;
class txCommandSystem;
class txSystemInfo;
class EditorCore : public txSingleton<EditorCore>
{
public:
	EditorCore();
	virtual ~EditorCore(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime);
	virtual void destroy();
	void notifyInitDone() { mInitFlag = true; }
	void checkErrorInfoBuffer();
	void logError(const std::string& info);
	void logInfo(const std::string& info);
	CTEventSystem* getEventSystem() { return mEventSystem; }
	txCommandSystem* getCommandSystem() { return mCommandSystem; }
	txSystemInfo* getSystemInfo() { return mSystemInfo; }
protected:
	txSystemInfo* mSystemInfo;
	CTEventSystem* mEventSystem;
	txCommandSystem* mCommandSystem;
	std::vector<std::string> mErrorInfoBuffer;	// 错误信息缓冲,用于保存事件系统创建之前产生的错误信息
	bool mInitFlag;								// 初始化标记,为false表示还未初始化完成,由外部进行标记
};

#endif