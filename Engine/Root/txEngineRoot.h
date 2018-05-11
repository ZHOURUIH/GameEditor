#ifndef _TX_ENGINE_ROOT_H_
#define _TX_ENGINE_ROOT_H_

#include "txEngineDefine.h"
#include "txSingleton.h"

class txBillboardManager;
class txEntityManager;
class txMagicParticleSystem;
class txNodeManager;
class txRendererManager;
class txRenderWindow;
class txSceneManager;
class txTextureManager;
class txAudioManager;
class txEngineEventSystem;
class txTremblingManager;
class txVideoManager;
class txFreeTypeFont;
class txCommandSystem;
class txODEPhysics;
class txPropertyManager;
class txActionTreeManager;
class txComponentFactoryManager;
class txActionFactoryManager;
class txSystemInfo;
class InputSystem;
class CameraManager;
class LayoutManager;
class WindowPrefabManager;
class WindowTemplateManager;
class WindowFactoryManager;
class LayoutUtility;
class LayoutScriptFactoryManager;

class txEngineRoot : public txSingleton<txEngineRoot>
{
public:
	txEngineRoot();
	virtual ~txEngineRoot();
	// 初始化引擎的所有组件
#if RUN_PLATFORM == PLATFORM_WINDOWS
	void initialise(const HWND& hwnd, const int& colourBits, const int& width, const int& height);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	void initialise(const int& width, const int& height);
#endif
	// 更新逻辑
	void preUpdate(float elapsedTime);
	void update(float elapsedTime);
	void lateUpdate(float elapsedTime);
	// 开始渲染
	void beginRender();
	// 结束渲染
	void endRender();
	// 退出并销毁所有组件
	void destroy();
	void notifyInitDone(){mInitFlag = true;}
	void logError(const std::string& info);
	// 尽量使用txEngineRoot的sendEvent来替代txEventSystem的pushEvent
	void sendEvent(const ENGINE_EVENT& type, const std::string& param, bool sendImmediately = true);
	void sendEvent(const ENGINE_EVENT& type, const txVector<std::string>& params = txVector<std::string>(), bool sendImmediately = true);
public:
	txBillboardManager* getBillboardManager()					{ return mBillboardManager; }
	txEntityManager* getEntityManager()							{ return mEntityManager; }
	txMagicParticleSystem* getMagicParticleSystem()				{ return mMagicParticleSystem;}
	txNodeManager* getNodeManager()								{ return mNodeManager; }
	txRendererManager* getRendererManager()						{ return mRendererManager; }
	txRenderWindow* getRenderWindow()							{ return mRenderWindow; }
	txSceneManager* getSceneManager()							{ return mSceneManager; }
	txTextureManager* getTextureManager()						{ return mTextureManager; }
	txAudioManager* getAudioManager()							{ return mAudioManager; }
	txEngineEventSystem* getEngineEventSystem()					{ return mEngineEventSystem; }
	txTremblingManager* getTremblingManager()					{ return mTremblingManager; }
	txVideoManager* getVideoManager()							{ return mVideoManager; }
	txFreeTypeFont* getFreeTypeFont()							{ return mFreeTypeFont; }
	txCommandSystem* getCommandSystem()							{ return mCommandSystem; }
	txODEPhysics* getODEPhysics()								{ return mODEPhysics; }
	txPropertyManager* getPropertyManager()						{ return mPropertyManager;}
	txActionTreeManager* getActionTreeManager()					{ return mActionTreeManager; }
	txComponentFactoryManager* getComponentFactoryManager()		{ return mComponentFactoryManager; }
	txActionFactoryManager* getActionFactoryManager()			{ return mActionFactoryManager; }
	txSystemInfo* getSystemInfo()								{ return mSystemInfo; }
	InputSystem* getInputSystem()								{ return mInputSystem; }
	CameraManager* getCameraManager()							{ return mCameraManager; }
	LayoutManager* getLayoutManager()							{ return mLayoutManager; }
	WindowPrefabManager* getWindowPrefabManager()				{ return mWindowPrefabManager; }
	WindowTemplateManager* getWindowTemplateManager()			{ return mWindowTemplateManager; }
	WindowFactoryManager* getWindowFactoryManager()				{ return mWindowFactoryManager; }
	LayoutUtility* getLayoutUtility()							{ return mLayoutUtility; }
	LayoutScriptFactoryManager* getLayoutScriptFactoryManager() { return mLayoutScriptFactoryManager; }
protected:
	void checkErrorBuffer();
protected:
	txBillboardManager* mBillboardManager;
	txEntityManager* mEntityManager;
	txMagicParticleSystem* mMagicParticleSystem;
	txNodeManager* mNodeManager;
	txRendererManager* mRendererManager;
	txSceneManager* mSceneManager;
	txTextureManager* mTextureManager;
	txRenderWindow* mRenderWindow;
	txAudioManager* mAudioManager;
	txEngineEventSystem* mEngineEventSystem;
	txTremblingManager* mTremblingManager;
	txVideoManager* mVideoManager;
	txFreeTypeFont* mFreeTypeFont;
	txCommandSystem* mCommandSystem;
	txODEPhysics* mODEPhysics;
	txPropertyManager* mPropertyManager;
	txActionTreeManager* mActionTreeManager;
	txComponentFactoryManager* mComponentFactoryManager;
	txActionFactoryManager* mActionFactoryManager;
	txSystemInfo* mSystemInfo;
	InputSystem* mInputSystem;
	CameraManager* mCameraManager;
	LayoutManager* mLayoutManager;
	WindowPrefabManager* mWindowPrefabManager;
	WindowTemplateManager* mWindowTemplateManager;
	WindowFactoryManager* mWindowFactoryManager;
	LayoutUtility* mLayoutUtility;
	LayoutScriptFactoryManager* mLayoutScriptFactoryManager;
	txVector<std::string> mErrorInfoBuffer;	// 仅在事件系统初始化完成之前使用
	bool mInitFlag;								// 初始化标记,为false则表示还未初始化完成
};

#endif