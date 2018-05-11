#include "txRendererInstance.h"
#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txMathUtility.h"
#include "txTexture.h"
#include "txRendererManager.h"
#include "txEngineLog.h"

txRendererColor* txRendererInstance::mColourRender = NULL;
txRendererLight* txRendererInstance::mLightRender = NULL;
txRendererShadowMap* txRendererInstance::mShadowMapRender = NULL;
txRendererShadowMapLight* txRendererInstance::mShadowMapLightRender = NULL;
txRendererShadowMapSkinAnim* txRendererInstance::mShadowMapSkinAnimRender = NULL;
txRendererSkinAnimation* txRendererInstance::mSkinAnimationRender = NULL;
txRendererSkybox* txRendererInstance::mSkyboxRender = NULL;
txRendererTexture* txRendererInstance::mTextureRender = NULL;
txRendererUI* txRendererInstance::mUIRender = NULL;
txRendererWater* txRendererInstance::mWaterRender = NULL;

txRendererInstance::txRendererInstance(const std::string& vertName, const std::string& fragName)
	:
	mVertFileName(vertName),
	mFragFileName(fragName),
	mProgram(0)
{
	loadProgram();
}

void txRendererInstance::notifyRendererDone()
{
	mColourRender = mRendererManager->getInstance<txRendererColor>();
	mLightRender = mRendererManager->getInstance<txRendererLight>();
	mShadowMapRender = mRendererManager->getInstance<txRendererShadowMap>();
	mShadowMapLightRender = mRendererManager->getInstance<txRendererShadowMapLight>();
	mShadowMapSkinAnimRender = mRendererManager->getInstance<txRendererShadowMapSkinAnim>();
	mSkinAnimationRender = mRendererManager->getInstance<txRendererSkinAnimation>();
	mSkyboxRender = mRendererManager->getInstance<txRendererSkybox>();
	mTextureRender = mRendererManager->getInstance<txRendererTexture>();
	mUIRender = mRendererManager->getInstance<txRendererUI>();
	mWaterRender = mRendererManager->getInstance<txRendererWater>();
}

void txRendererInstance::loadProgram()
{
	//create program
	mProgram = mRendererManager->createProgram(txFileUtility::openTxtFile(mVertFileName), txFileUtility::openTxtFile(mFragFileName));
	if (mProgram == 0)
	{
		ENGINE_ERROR("error : Create program failed! vert name : %s, frag name : %s", mVertFileName.c_str(), mFragFileName.c_str());
	}
}