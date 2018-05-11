#ifndef _TX_RENDERER_MANAGER_H_
#define _TX_RENDERER_MANAGER_H_

#include "txEngineDefine.h"
#include "txRenderer_Color.h"
#include "txRenderer_Light.h"
#include "txRenderer_ShadowMap.h"
#include "txRenderer_ShadowMapLight.h"
#include "txRenderer_ShadowMapSkinAnim.h"
#include "txRenderer_SkinAnimation.h"
#include "txRenderer_Skybox.h"
#include "txRenderer_Texture.h"
#include "txRenderer_UI.h"
#include "txRenderer_Water.h"
#include "Utility.h"

class txRendererInstance;
class txRendererManager
{
public:
	txRendererManager(){}
	virtual ~txRendererManager(){ destroy(); }
	virtual void init();
	void destroy();
	int Clear(const float& r, const float& g, const float& b, const float& a);
	GLuint createProgram(const std::string& pVertexSource, const std::string& pFragmentSource); //失败返回0
	template<typename T>
	T* getInstance()
	{
		auto iter = mRendererList.find(typeid(T).name());
		if (iter != mRendererList.end())
		{
			return static_cast<T*>(iter->second);
		}
		return NULL;
	}
protected:
	template<typename T>
	void createInstance(const std::string& shaderName)
	{
		std::string vertexName = P_SHADER_PATH + shaderName + ".vert";
		std::string fragName = P_SHADER_PATH + shaderName + ".frag";
		T* instance = TRACE_NEW(T, instance, vertexName, fragName);
		instance->init();
		mRendererList.insert(typeid(T).name(), instance);
	}
	GLuint loadShader(GLenum shaderType, const char* pSource); //失败返回0
	void printGLString(const char *name, GLenum s);
protected:
	txMap<std::string, txRendererInstance*> mRendererList;
};

#endif