#include "txEngineBase.h"
#include "txEngineRoot.h"

txEngineRoot* txEngineBase::mEngineRoot = NULL;
txCommandSystem* txEngineBase::mCommandSystem = NULL;
txBillboardManager* txEngineBase::mBillboardManager = NULL;
txEntityManager* txEngineBase::mEntityManager = NULL;
txMagicParticleSystem* txEngineBase::mMagicParticleSystem = NULL;
txNodeManager* txEngineBase::mNodeManager = NULL;
txRendererManager* txEngineBase::mRendererManager = NULL;
txSceneManager* txEngineBase::mSceneManager = NULL;
txTextureManager* txEngineBase::mTextureManager = NULL;
txRenderWindow* txEngineBase::mRenderWindow = NULL;
txAudioManager* txEngineBase::mAudioManager = NULL;
txEngineEventSystem* txEngineBase::mEngineEventSystem = NULL;
txTremblingManager* txEngineBase::mTremblingManager = NULL;
txVideoManager* txEngineBase::mVideoManager = NULL;
txFreeTypeFont* txEngineBase::mFreeTypeFont = NULL;
txODEPhysics* txEngineBase::mODEPhysics = NULL;
txPropertyManager* txEngineBase::mPropertyManager = NULL;
txActionTreeManager* txEngineBase::mActionTreeManager = NULL;
txComponentFactoryManager* txEngineBase::mComponentFactoryManager = NULL;
txActionFactoryManager* txEngineBase::mActionFactoryManager = NULL;
txSystemInfo* txEngineBase::mSystemInfo = NULL;
InputSystem* txEngineBase::mInputSystem = NULL;
CameraManager* txEngineBase::mCameraManager = NULL;
LayoutManager* txEngineBase::mLayoutManager = NULL;
WindowTemplateManager* txEngineBase::mWindowTemplateManager = NULL;
WindowPrefabManager* txEngineBase::mWindowPrefabManager = NULL;
WindowFactoryManager* txEngineBase::mWindowFactoryManager = NULL;
LayoutUtility* txEngineBase::mLayoutUtility = NULL;
LayoutScriptFactoryManager* txEngineBase::mLayoutScriptFactoryManager = NULL;

void txEngineBase::notifyConstructDone()
{
	if (mEngineRoot == NULL)
	{
		mEngineRoot = txEngineRoot::getSingletonPtr();
		mCommandSystem = mEngineRoot->getCommandSystem();
		mBillboardManager = mEngineRoot->getBillboardManager();
		mEntityManager = mEngineRoot->getEntityManager();
		mMagicParticleSystem = mEngineRoot->getMagicParticleSystem();
		mNodeManager = mEngineRoot->getNodeManager();
		mRendererManager = mEngineRoot->getRendererManager();
		mSceneManager = mEngineRoot->getSceneManager();
		mTextureManager = mEngineRoot->getTextureManager();
		mRenderWindow = mEngineRoot->getRenderWindow();
		mAudioManager = mEngineRoot->getAudioManager();
		mEngineEventSystem = mEngineRoot->getEngineEventSystem();
		mTremblingManager = mEngineRoot->getTremblingManager();
		mVideoManager = mEngineRoot->getVideoManager();
		mFreeTypeFont = mEngineRoot->getFreeTypeFont();
		mODEPhysics = mEngineRoot->getODEPhysics();
		mPropertyManager = mEngineRoot->getPropertyManager();
		mActionTreeManager = mEngineRoot->getActionTreeManager();
		mComponentFactoryManager = mEngineRoot->getComponentFactoryManager();
		mActionFactoryManager = mEngineRoot->getActionFactoryManager();
		mSystemInfo = mEngineRoot->getSystemInfo();
		mInputSystem = mEngineRoot->getInputSystem();
		mCameraManager = mEngineRoot->getCameraManager();
		mLayoutManager = mEngineRoot->getLayoutManager();
		mWindowTemplateManager = mEngineRoot->getWindowTemplateManager();
		mWindowPrefabManager = mEngineRoot->getWindowPrefabManager();
		mWindowFactoryManager = mEngineRoot->getWindowFactoryManager();
		mLayoutUtility = mEngineRoot->getLayoutUtility();
		mLayoutScriptFactoryManager = mEngineRoot->getLayoutScriptFactoryManager();
	}
}