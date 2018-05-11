#ifndef _EDITOR_CORE_H_
#define _EDITOR_CORE_H_

#include "CommonDefine.h"
#include "txSingleton.h"

class SocketNetManager;
class CTEventSystem;
class txCommandSystem;
class txSystemInfo;
class LibCurlNetManager;
class GymInfo;
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
	void sendEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params = txVector<std::string>(), bool sendImmediately = true);
	void logError(const std::string& info);
	void logInfo(const std::string& info);
	SocketNetManager* getSocketNetManager() { return mSocketNetManager; }
	CTEventSystem* getEventSystem() { return mEventSystem; }
	txCommandSystem* getCommandSystem() { return mCommandSystem; }
	txSystemInfo* getSystemInfo() { return mSystemInfo; }
	LibCurlNetManager* getLibCurlNetManager() { return mLibCurlNetManager; }
	GymInfo* getGymInfo() { return mGymInfo; }
protected:
	SocketNetManager* mSocketNetManager;
	CTEventSystem* mEventSystem;
	txCommandSystem* mCommandSystem;
	txSystemInfo* mSystemInfo;
	LibCurlNetManager* mLibCurlNetManager;
	txVector<std::string> mErrorInfoBuffer;	// 错误信息缓冲,用于保存事件系统创建之前产生的错误信息
	bool mInitFlag;							// 初始化标记,为false表示还未初始化完成,由外部进行标记
	GymInfo* mGymInfo;
};

#endif