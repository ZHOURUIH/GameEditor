#ifndef _TX_RENDERER_INSTANCE_H_
#define _TX_RENDERER_INSTANCE_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"
#include "txEngineLog.h"

class txTexture;
class txRenderWindow;
class txRendererColor;
class txRendererLight;
class txRendererShadowMap;
class txRendererShadowMapLight;
class txRendererShadowMapSkinAnim;
class txRendererSkinAnimation;
class txRendererSkybox;
class txRendererTexture;
class txRendererUI;
class txRendererWater;

class txRendererInstance : public txEngineBase
{
public:
	txRendererInstance(const std::string& vertName, const std::string& fragName);
	virtual ~txRendererInstance(){}
	virtual void init() = 0;
	virtual void loadProgram();
	const GLuint& getProgram() { return mProgram; }
	static void notifyRendererDone();
	static void checkGlError(const char* op)
	{
		for (GLint error = glGetError(); error; error = glGetError())
		{
#if RUN_PLATFORM == PLATFORM_WINDOWS
			ENGINE_INFO("after %s() glError (0x%x), error string : %s", op, error, gluErrorString(error));
#elif RUN_PLATFORM == PLATFORM_ANDROID
			ENGINE_INFO("after %s() glError (0x%x)", op, error);
#endif
		}
	}
	void getAttributeHandle(GLuint& handle, const std::string& variableName)
	{
		handle = glGetAttribLocation(mProgram, variableName.c_str());
		checkGlError("glGetAttribLocation");
	}
	void getUniformHandle(GLuint& handle, const std::string& variableName)
	{
		handle = glGetUniformLocation(mProgram, variableName.c_str());
		checkGlError("glGetUniformLocation");
	}
public:
	static txRendererColor* mColourRender;
	static txRendererLight* mLightRender;
	static txRendererShadowMap* mShadowMapRender;
	static txRendererShadowMapLight* mShadowMapLightRender;
	static txRendererShadowMapSkinAnim* mShadowMapSkinAnimRender;
	static txRendererSkinAnimation* mSkinAnimationRender;
	static txRendererSkybox* mSkyboxRender;
	static txRendererTexture* mTextureRender;
	static txRendererUI* mUIRender;
	static txRendererWater* mWaterRender;
protected:
	std::string mVertFileName;
	std::string mFragFileName;
	GLuint mProgram;
};

#endif