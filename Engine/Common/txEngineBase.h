#ifndef _TX_ENGINE_BASE_H_
#define _TX_ENGINE_BASE_H_

class txEngineRoot;
class txCommandSystem;
class txBillboardManager;
class txEntityManager;
class txMagicParticleSystem;
class txNodeManager;
class txRendererManager;
class txSceneManager;
class txTextureManager;
class txRenderWindow;
class txAudioManager;
class txEngineEventSystem;
class txTremblingManager;
class txVideoManager;
class txFreeTypeFont;
class txODEPhysics;
class txPropertyManager;
class txActionTreeManager;
class txComponentFactoryManager;
class txActionFactoryManager;
class txSystemInfo;
class InputSystem;
class CameraManager;
class LayoutManager;
class WindowTemplateManager;
class WindowPrefabManager;
class WindowFactoryManager;
class LayoutUtility;
class GUIEventSystem;
class LayoutScriptFactoryManager;
class txEngineBase
{
public:
	static void notifyConstructDone();
public:
	static txEngineRoot* mEngineRoot;
	static txCommandSystem* mCommandSystem;
	static txBillboardManager* mBillboardManager;
	static txEntityManager* mEntityManager;
	static txMagicParticleSystem* mMagicParticleSystem;
	static txNodeManager* mNodeManager;
	static txRendererManager* mRendererManager;
	static txSceneManager* mSceneManager;
	static txTextureManager* mTextureManager;
	static txRenderWindow* mRenderWindow;
	static txAudioManager* mAudioManager;
	static txEngineEventSystem* mEngineEventSystem;
	static txTremblingManager* mTremblingManager;
	static txVideoManager* mVideoManager;
	static txFreeTypeFont* mFreeTypeFont;
	static txODEPhysics* mODEPhysics;
	static txPropertyManager* mPropertyManager;
	static txActionTreeManager* mActionTreeManager;
	static txComponentFactoryManager* mComponentFactoryManager;
	static txActionFactoryManager* mActionFactoryManager;
	static txSystemInfo* mSystemInfo;
	static InputSystem* mInputSystem;
	static CameraManager* mCameraManager;
	static LayoutManager* mLayoutManager;
	static WindowTemplateManager* mWindowTemplateManager;
	static WindowPrefabManager* mWindowPrefabManager;
	static WindowFactoryManager* mWindowFactoryManager;
	static LayoutUtility* mLayoutUtility;
	static LayoutScriptFactoryManager* mLayoutScriptFactoryManager;
};

#endif