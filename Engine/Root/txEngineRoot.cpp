#include "txEngineRoot.h"
#include "txBillboardManager.h"
#include "txEntityManager.h"
#include "txMagicParticleSystem.h"
#include "txNodeManager.h"
#include "txRendererManager.h"
#include "txRenderWindow.h"
#include "txSceneManager.h"
#include "txTextureManager.h"
#include "txAudioManager.h"
#include "txEngineEventSystem.h"
#include "Utility.h"
#include "txTremblingManager.h"
#include "txVideoManager.h"
#include "txFreeTypeFont.h"
#include "txCommandSystem.h"
#include "txODEPhysics.h"
#include "txPropertyManager.h"
#include "txActionTreeManager.h"
#include "txComponentFactory.h"
#include "txComponentFactoryManager.h"
#include "PropertyHeader.h"
#include "txComponentHeader.h"
#include "txActionFactory.h"
#include "txActionFactoryManager.h"
#include "txSystemInfo.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include "LayoutManager.h"
#include "LayoutUtility.h"
#include "WindowPrefabManager.h"
#include "WindowTemplateManager.h"
#include "WindowFactoryManager.h"
#include "LayoutScriptFactory.h"
#include "LayoutScriptFactoryManager.h"

template<>txEngineRoot* txSingleton<txEngineRoot>::ms_Singleton = NULL;

txEngineRoot::txEngineRoot()
{
	mInitFlag = false;
	mBillboardManager = NULL;
	mEntityManager = NULL;
	mNodeManager = NULL;
	mRendererManager = NULL;
	mRenderWindow = NULL;
	mSceneManager = NULL;
	mTextureManager = NULL;
	mAudioManager = NULL;
	mMagicParticleSystem = NULL;
	mEngineEventSystem = NULL;
	mVideoManager = NULL;
	mFreeTypeFont = NULL;
	mCommandSystem = NULL;
	mODEPhysics = NULL;
	mPropertyManager = NULL;
	mActionTreeManager = NULL;
	mComponentFactoryManager = NULL;
	mSystemInfo = NULL;
	mInputSystem = NULL;
	mCameraManager = NULL;
	mLayoutScriptFactoryManager = NULL;
}

txEngineRoot::~txEngineRoot()
{
	destroy();
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
void txEngineRoot::initialise(const HWND& hwnd, const int& colourBits, const int& width, const int& height)
#elif RUN_PLATFORM == PLATFORM_ANDROID
void txEngineRoot::initialise(const int& width, const int& height)
#endif
{
	// 创建所有管理器类
	// 首先创建事件系统
	mEngineEventSystem = TRACE_NEW(txEngineEventSystem, mEngineEventSystem);
	// 事件系统创建后,需要检查是否有错误信息
	checkErrorBuffer();
	mComponentFactoryManager = TRACE_NEW(txComponentFactoryManager, mComponentFactoryManager);
	mActionFactoryManager = TRACE_NEW(txActionFactoryManager, mActionFactoryManager);
	mCommandSystem = TRACE_NEW(txCommandSystem, mCommandSystem);
	mBillboardManager = TRACE_NEW(txBillboardManager, mBillboardManager);
	mEntityManager = TRACE_NEW(txEntityManager, mEntityManager);
	mMagicParticleSystem = TRACE_NEW(txMagicParticleSystem, mMagicParticleSystem);
	mNodeManager = TRACE_NEW(txNodeManager, mNodeManager);
	mRendererManager = TRACE_NEW(txRendererManager, mRendererManager);
	mTextureManager = TRACE_NEW(txTextureManager, mTextureManager);
	mAudioManager = TRACE_NEW(txAudioManager, mAudioManager);
	mTremblingManager = TRACE_NEW(txTremblingManager, mTremblingManager);
	mVideoManager = TRACE_NEW(txVideoManager, mVideoManager);
	mFreeTypeFont = TRACE_NEW(txFreeTypeFont, mFreeTypeFont);
	mODEPhysics = TRACE_NEW(txODEPhysics, mODEPhysics);
	mPropertyManager = TRACE_NEW(txPropertyManager, mPropertyManager);
	mSceneManager = TRACE_NEW(txSceneManager, mSceneManager);
	mActionTreeManager = TRACE_NEW(txActionTreeManager, mActionTreeManager);
	mRenderWindow = TRACE_NEW(txRenderWindow, mRenderWindow);
	mSystemInfo = TRACE_NEW(txSystemInfo, mSystemInfo);
	mInputSystem = TRACE_NEW(InputSystem, mInputSystem);
	mCameraManager = TRACE_NEW(CameraManager, mCameraManager);
	mLayoutManager = TRACE_NEW(LayoutManager, mLayoutManager);
	mWindowFactoryManager = TRACE_NEW(WindowFactoryManager, mWindowFactoryManager);
	mLayoutUtility = TRACE_NEW(LayoutUtility, mLayoutUtility);
	mWindowPrefabManager = TRACE_NEW(WindowPrefabManager, mWindowPrefabManager);
	mWindowTemplateManager = TRACE_NEW(WindowTemplateManager, mWindowTemplateManager);
	mLayoutScriptFactoryManager = TRACE_NEW(LayoutScriptFactoryManager, mLayoutScriptFactoryManager);
	// 所有类都构造完成后通知txEngineBase
	txEngineBase::notifyConstructDone();

	mSystemInfo->init();
#if RUN_PLATFORM == PLATFORM_WINDOWS
	mRenderWindow->init("main render window", hwnd, colourBits, width, height);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	mRenderWindow->init("main render window", width, height);
#endif
	mComponentFactoryManager->init();
	mActionFactoryManager->init();
	mCommandSystem->init();
	mBillboardManager->init();
	mEntityManager->init();
	mSceneManager->init();
	mRendererManager->init();
	mMagicParticleSystem->init();
	mNodeManager->init();
	mTextureManager->init();
	mAudioManager->init();
	mTremblingManager->init();
	mVideoManager->init();
	mFreeTypeFont->init();
	mODEPhysics->init();
	mPropertyManager->init();
	mActionTreeManager->init();
	mInputSystem->init();
	mCameraManager->init();
	mLayoutManager->init();
	mLayoutUtility->init();
	mWindowFactoryManager->init();
	mWindowPrefabManager->init();
	mWindowTemplateManager->init();
	mLayoutUtility->init();
	mLayoutScriptFactoryManager->init();
}

void txEngineRoot::preUpdate(float elapsedTime)
{
	mInputSystem->preUpdate(elapsedTime);
}

void txEngineRoot::update(float elapsedTime)
{
	// 先调用纹理管理器的更新,释放需要释放的纹理
	mTextureManager->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mSceneManager->update(elapsedTime);
	mAudioManager->update(elapsedTime);
	mEngineEventSystem->update(elapsedTime);
	mBillboardManager->update(elapsedTime);
	mNodeManager->update(elapsedTime);
	mVideoManager->update(elapsedTime);
	mFreeTypeFont->update(elapsedTime);
	mODEPhysics->update(elapsedTime);
	mActionTreeManager->update(elapsedTime);
	mCameraManager->update(elapsedTime);
	mInputSystem->update(elapsedTime);
	mLayoutManager->update(elapsedTime);
}

void txEngineRoot::lateUpdate(float elapsedTime)
{
	mInputSystem->lateUpdate(elapsedTime);
}

// 开始渲染
void txEngineRoot::beginRender()
{
	mRenderWindow->Clear();
}

// 结束渲染
void txEngineRoot::endRender()
{
	mRenderWindow->SwapBuffer();
}

void txEngineRoot::destroy()
{
	TRACE_DELETE(mCameraManager);
	TRACE_DELETE(mTremblingManager);
	TRACE_DELETE(mBillboardManager);
	TRACE_DELETE(mEntityManager);
	TRACE_DELETE(mMagicParticleSystem);
	TRACE_DELETE(mRendererManager);
	TRACE_DELETE(mRenderWindow);
	TRACE_DELETE(mSceneManager);
	TRACE_DELETE(mVideoManager);
	TRACE_DELETE(mTextureManager);
	TRACE_DELETE(mAudioManager);
	TRACE_DELETE(mNodeManager);
	TRACE_DELETE(mEngineEventSystem);
	TRACE_DELETE(mFreeTypeFont);
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mPropertyManager);
	TRACE_DELETE(mActionTreeManager);
	TRACE_DELETE(mSystemInfo);
	TRACE_DELETE(mInputSystem);
	TRACE_DELETE(mWindowPrefabManager);
	TRACE_DELETE(mWindowTemplateManager);
	TRACE_DELETE(mLayoutUtility);
	TRACE_DELETE(mLayoutManager);
	TRACE_DELETE(mODEPhysics);
	TRACE_DELETE(mWindowFactoryManager);
	TRACE_DELETE(mComponentFactoryManager);
	TRACE_DELETE(mLayoutScriptFactoryManager);
}

void txEngineRoot::logError(const std::string& info)
{
	if (mEngineEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	// 如果还没有初始化完成,则先将消息放入事件缓冲中
	sendEvent(EE_ENGINE_ERROR_INFO, info);
}

void txEngineRoot::sendEvent(const ENGINE_EVENT& type, const std::string& param, bool sendImmediately)
{
	txVector<std::string> paramList;
	paramList.push_back(param);
	sendEvent(type, paramList, sendImmediately);
}

void txEngineRoot::sendEvent(const ENGINE_EVENT& type, const txVector<std::string>& params, bool sendImmediately)
{
	// 如果是立即发送的事件,则需要根据初始化标记判断是否应该立即发送
	if (sendImmediately)
	{
		sendImmediately = mInitFlag;
	}
	if (mEngineEventSystem != NULL)
	{
		mEngineEventSystem->pushEvent(type, params, sendImmediately);
	}
}

void txEngineRoot::checkErrorBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	FOR_STL(mErrorInfoBuffer, int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	END_FOR_STL(mErrorInfoBuffer);
	mErrorInfoBuffer.clear();
}