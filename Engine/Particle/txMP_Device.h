/*
 * txMP_Device.h
 *
 *  Created on: 2015-6-25
 *      Author: zhourui
 */

#ifndef TXMP_DEVICE_H_
#define TXMP_DEVICE_H_

#include "txMP_wrap.h"
#include "txOpenGLBuffer.h"
#include "txEngineBase.h"

class txCamera;
class txMP_Device : public MP_Device
{
protected:
	#ifdef OPENGL_WIN
	HWND hwnd;
	HDC	hDC;		// eng: Private GDI Device Context
	HGLRC hRC;		// eng: Permanent Rendering Context
	int bits;
	#endif
public:
	MAGIC_MATRIX view, projection;
protected:
	#ifdef OPENGL_SHADER
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	GLuint uniform_view_matrix;
	GLuint uniform_projection_matrix;

	GLuint mPositionHandle;
	GLuint mColourHandle;
	GLuint mTextureCoordHandle;
	#endif

	OpenGLBuffer buffer;

	#ifdef OPENGL_SAVE_ATTRIBUTES
	// attributes
	int prev_texture;
	GLboolean attr_GL_BLEND;
	GLboolean attr_GL_DEPTH_WRITEMASK;
	GLboolean attr_GL_DEPTH_TEST;

	#ifdef OPENGL_SHADER
	GLint prev_program;

	GLint attr_GL_BLEND_SRC_ALPHA;
	GLint attr_GL_BLEND_DST_ALPHA;
	GLint attr_GL_ARRAY_BUFFER_BINDING;
	GLint attr_GL_ELEMENT_ARRAY_BUFFER_BINDING;

	GLint attr_ATTRIB_VERTEX;
	GLint attr_ATTRIB_COLOR;
	GLint attr_ATTRIB_TEXTUREPOSITION;

	BUFFER_ATTRIBUTES attr_vertex_buffer;
	BUFFER_ATTRIBUTES attr_color_buffer;
	BUFFER_ATTRIBUTES attr_textureposition_buffer;
	#else
	GLboolean attr_GL_TEXTURE_2D;
	GLboolean attr_GL_COLOR_MATERIAL;
	GLboolean attr_GL_LIGHTING;
	GLboolean attr_GL_VERTEX_ARRAY;
	GLboolean attr_GL_TEXTURE_COORD_ARRAY;
	GLboolean attr_GL_COLOR_ARRAY;
	#endif
	#endif

public:

	#ifdef OPENGL_SHADER
	const GLuint& GetPositionHandle() {return mPositionHandle;}
	const GLuint& GetColourHandle() {return mColourHandle;}
	const GLuint& GetTextureCoordHandle() {return mTextureCoordHandle;}
	#endif

    #ifndef OPENGL_WIN
	txMP_Device();
    #else
    txMP_Device(HWND hwnd, int bits);
    #endif
	~txMP_Device()
	{
		Destroy();
	}

	// eng: Creating
	virtual bool Create();

	virtual void SetScene3d(){}

	// eng: Destroying
	virtual void Destroy();

	// eng: Beginning of scene drawing
	virtual void BeginScene(txCamera* txcamera);

	// eng: End of scene drawing
	virtual void EndScene();

	// eng: Indicates that device is lost
	virtual bool IsLost();

	virtual void SetWorldMatrix(MAGIC_MATRIX* m);
	virtual void SetViewMatrix(MAGIC_MATRIX* m);
	virtual void SetProjectionMatrix(MAGIC_MATRIX* m);

//	virtual void SetViewport(MP_VIEWPORT* viewport);

	#ifdef OPENIL
	// eng: Loading texture from file
	virtual void LoadTextureFromFile(const char* file_name);
	#else
	unsigned char* LoadTextureFromFile(const char* file_name, int* width, int* height);
	#endif

    OpenGLBuffer& GetBuffer(){return buffer;}

	#ifdef OPENGL_SAVE_ATTRIBUTES
	void SaveAttributes();
	void RestoreAttributes();
	#endif
};

#endif /* TXMP_DEVICE_H_ */
