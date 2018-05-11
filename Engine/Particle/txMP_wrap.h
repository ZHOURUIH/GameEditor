//-----------------------------------------------------------------------------
// File: mp_wrap.h
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#ifndef _TX_MP_WRAP_H
#define _TX_MP_WRAP_H

#include <stdlib.h>
#include "txEngineDefine.h"
#include "txEngineBase.h"
#ifdef _ANDROID
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(_WINDOWS)
#include <glew.h>
#endif

#include "txMP.h"

#ifdef COCOS_2DX
	#include "cocos2d.h"
	#include "platform/CCPlatformConfig.h"
	#include <CCGL.h>

	#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
		#define internalformat GL_RGBA
	#else
		#define internalformat GL_RGBA8
	#endif

	#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
		#include <android/asset_manager.h>
	#endif
#else
	#ifdef _WINDOWS
		#include <windows.h>
		#include <gl/gl.h>
		#define internalformat GL_RGBA8
		#define DIST_CAMERA_3D  -840
	#endif

	#ifdef __APPLE__
		#include "TargetConditionals.h"
		#if (TARGET_OS_IPHONE==1) || (TARGET_IPHONE_SIMULATOR==1)
			#include <stdlib.h>
			#ifdef OPENGL_SHADER
				#import <OpenGLES/ES2/gl.h>
				#import <OpenGLES/ES2/glext.h>
			#else
				#import <OpenGLES/ES1/gl.h>
				#import <OpenGLES/ES1/glext.h>
			#endif
			#define internalformat GL_RGBA
			#define DIST_CAMERA_3D  -2520
		#else
			#if (TARGET_OS_MAC==1)
				#include <GLUT/glut.h>
				#define internalformat GL_RGBA8
				#define DIST_CAMERA_3D  -840
			#endif
		#endif
	#endif

	#if defined(__ANDROID__)
		#include <jni.h>
		#include <android/asset_manager.h>
		#include <android/log.h>

//		#define  LOG_TAG    "libwpapper_android"
//		#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//		#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>

		#define internalformat GL_RGBA
		#define DIST_CAMERA_3D  -2520
	#endif
#endif

#ifdef OPENGL_SHADER
// attribute index
enum ATTRIBUTES_INDEX {
	//ATTRIB_VERTEX,
	//ATTRIB_COLOR,
	//ATTRIB_TEXTUREPOSITION,
	NUM_ATTRIBUTES
};
#endif

struct BUFFER_ATTRIBUTES
{
	void* pointer;
	GLint enabled;
	GLint size;
	GLint type;
	GLint normalized;
	GLint stride;
	GLint buffer_binding;
};

void MattrixToQuaternion( float m[4][4], float quat[4] );
void MagicMatrixMul3(MAGIC_MATRIX* c, const MAGIC_MATRIX* a, const MAGIC_MATRIX* b);
void MagicMatrixMul(MAGIC_MATRIX* c, const MAGIC_MATRIX* a, const MAGIC_MATRIX* b);
void MagicMatrixTransposing(MAGIC_MATRIX* m);
void GetVertexBufferAttributes(const GLuint& index, BUFFER_ATTRIBUTES* bc);
void BindTexture(const GLuint& textureId);

#endif
