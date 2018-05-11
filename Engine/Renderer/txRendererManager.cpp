#include "txRendererManager.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"

void txRendererManager::init()
{
	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	//    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. *//*
	 eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	 ANativeWindow_setBuffersGeometry(window, 0, 0, format);

	 surface = eglCreateWindowSurface(display, config, window, NULL);

	 EGLint contextAtt[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
	 context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAtt);

	 if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
	 return -1;
	 }

	 eglQuerySurface(display, surface, EGL_WIDTH, &width);
	 eglQuerySurface(display, surface, EGL_HEIGHT, &height);*/

	createInstance<txRendererColor>("Color");
	createInstance<txRendererLight>("TextureLight");
	createInstance<txRendererShadowMap>("ShadowMap");
	createInstance<txRendererShadowMapLight>("TextureLight_shadowMap");
	createInstance<txRendererShadowMapSkinAnim>("SkinAnimation_shadowMap");
	createInstance<txRendererSkinAnimation>("SkinAnimation");
	createInstance<txRendererSkybox>("SkyBox");
	createInstance<txRendererTexture>("Texture");
	createInstance<txRendererUI>("Texture");
	createInstance<txRendererWater>("water");

	// 创建完渲染实例后通知实例基类
	txRendererInstance::notifyRendererDone();
}

void txRendererManager::destroy()
{
	auto iter = mRendererList.begin();
	auto iterEnd = mRendererList.end();
	FOR_STL(mRendererList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mRendererList);
	mRendererList.clear();
}

GLuint txRendererManager::loadShader(GLenum shaderType, const char* pSource)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader != 0)
	{
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				char* buf = TRACE_NEW_ARRAY(char, infoLen, buf);
				glGetShaderInfoLog(shader, infoLen, NULL, buf);
				ENGINE_ERROR("error : could not compile shader, type : %d, info : %s", shaderType, buf);
				TRACE_DELETE_ARRAY(buf);
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

GLuint txRendererManager::createProgram(const std::string& pVertexSource, const std::string& pFragmentSource)
{
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource.c_str());
	if (vertexShader == 0)
	{
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource.c_str());
	if (pixelShader == 0)
	{
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program != 0)
	{
		glAttachShader(program, vertexShader);
		txRendererInstance::checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		txRendererInstance::checkGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE)
		{
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength)
			{
				char* buf = TRACE_NEW_ARRAY(char, bufLength, buf);
				glGetProgramInfoLog(program, bufLength, NULL, buf);
				ENGINE_ERROR("error : Could not link program, info : %s", buf);
				TRACE_DELETE_ARRAY(buf);
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}

void txRendererManager::printGLString(const char *name, GLenum s)
{
	const char *v = (const char *)glGetString(s);
	LOGI("GL %s = %s\n", name, v);
}