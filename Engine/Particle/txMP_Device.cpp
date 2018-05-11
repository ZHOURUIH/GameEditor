#include "txMP_wrap.h"
#include "txMP_Device.h"
#include "txTexture.h"
#include "txCamera.h"

#ifdef OPENIL
#include "IL/ilu.h"
#endif

#ifdef COCOS_2DX
	using namespace cocos2d;
#else
	#ifdef _WINDOWS
	#include <windows.h>
	#pragma comment(lib, "opengl32.lib")
	#endif
#endif


#ifdef OPENIL
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")
#else
#endif

#pragma warning ( disable : 4996)

// eng: Class controlling drawing
#ifndef OPENGL_WIN
txMP_Device::txMP_Device() : MP_Device()
#else
txMP_Device::txMP_Device(HWND hwnd, int bits) : MP_Device()
#endif
{
	#ifdef OPENIL
	ilInit();
	#endif

    #ifdef OPENGL_WIN
	this->hwnd=hwnd;
	hDC=NULL;
	hRC=NULL;
	this->bits=bits;
    #endif

	#ifdef OPENGL_SHADER
	vertex_shader=fragment_shader=0;
	program=0;

    uniform_view_matrix=uniform_projection_matrix=0xffffffff;
	#endif

	#ifdef OPENGL_SAVE_ATTRIBUTES
	prev_texture=-1;
	#endif
}

bool txMP_Device::Create()
{
	Destroy();

	#ifdef OPENGL_WIN
	GLuint		PixelFormat;

	static	PIXELFORMATDESCRIPTOR pfd=				// eng: pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// eng: Size Of This Pixel Format Descriptor
		1,											// eng: Version Number
		PFD_DRAW_TO_WINDOW |						// eng: Format Must Support Window
		PFD_SUPPORT_OPENGL |						// eng: Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// eng: Must Support Double Buffering
		PFD_TYPE_RGBA,								// eng: Request An RGBA Format
		bits,										// eng: Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// eng: Color Bits Ignored
		0,											// eng: No Alpha Buffer
		0,											// eng: Shift Bit Ignored
		0,											// eng: No Accumulation Buffer
		0, 0, 0, 0,									// eng: Accumulation Bits Ignored
		0,											// eng: No Z-Buffer (Depth Buffer)
		0,											// eng: No Stencil Buffer
		0,											// eng: No Auxiliary Buffer
		PFD_MAIN_PLANE,								// eng: Main Drawing Layer
		0,											// eng: Reserved
		0, 0, 0										// eng: Layer Masks Ignored
	};

	if (!(hDC=GetDC(hwnd)))							// eng: Did We Get A Device Context?
	{
		Destroy();
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// eng: Did Windows Find A Matching Pixel Format?
	{
		Destroy();
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: eturn FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// eng: Are We Able To Set The Pixel Format?
	{
		Destroy();
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// eng: Are We Able To Get A Rendering Context?
	{
		Destroy();
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// eng: Try To Activate The Rendering Context
	{
		Destroy();
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// eng: Return FALSE
	}

	RECT client_rect;
	GetClientRect(hwnd,&client_rect);
	window_width=client_rect.right;
	window_height=client_rect.bottom;
	#endif


	#ifdef OPENGL_SHADER
	#ifndef COCOS_2DX
	// vertex shader
	vertex_shader=glCreateShader(GL_VERTEX_SHADER);
	std::string vertex_shader_code;
	vertex_shader_code+="attribute vec4 position;";
	vertex_shader_code+="attribute vec4 color;";
	vertex_shader_code+="attribute vec2 inputTextureCoordinate;";
	vertex_shader_code+="uniform mat4 modelViewMatrix;";
	vertex_shader_code+="uniform mat4 projectionMatrix;";
	vertex_shader_code+="varying vec4 colorVarying;";
	vertex_shader_code+="varying vec2 textureCoordinate;";
	vertex_shader_code+="void main()";
	vertex_shader_code+="{";
	//#ifdef MAGIC_3D
	vertex_shader_code+="gl_Position = projectionMatrix * modelViewMatrix * position;";
	//#else
	//vertex_shader_code+="gl_Position = position;";
	//#endif
	vertex_shader_code+="colorVarying = color;";
	vertex_shader_code+="textureCoordinate = inputTextureCoordinate.xy;";
	vertex_shader_code+="}";

    const char* code=vertex_shader_code.c_str();
	const GLchar** code_ptr=&code;

	glShaderSource(vertex_shader, 1, code_ptr, 0);
	glCompileShader(vertex_shader);
	char log[1000];
	int len=0;
	glGetShaderInfoLog(vertex_shader, 1000, &len, log);

	// fragment shader
	fragment_shader=glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragment_shader_code;
	fragment_shader_code+="#ifdef GL_ES\r\n";
	fragment_shader_code+="// define default precision for float, vec, mat.\r\n";
	fragment_shader_code+="precision lowp float;\r\n";
	fragment_shader_code+="#endif\r\n";
	fragment_shader_code+="varying vec4 colorVarying;\r\n";
	fragment_shader_code+="#ifdef GL_ES\r\n";
	fragment_shader_code+="varying lowp vec2 textureCoordinate;\r\n";
	fragment_shader_code+="#else\r\n";
	fragment_shader_code+="varying vec2 textureCoordinate;\r\n";
	fragment_shader_code+="#endif\r\n";
	fragment_shader_code+="uniform sampler2D texture;";
	fragment_shader_code+="void main()";
	fragment_shader_code+="{";
	fragment_shader_code+="vec4 a = texture2D(texture, textureCoordinate);";
	fragment_shader_code+="gl_FragColor = a * colorVarying;";
	fragment_shader_code+="}";

    code=fragment_shader_code.c_str();
	code_ptr=&code;

	glShaderSource(fragment_shader, 1, code_ptr, 0);
	glCompileShader(fragment_shader);

	glGetShaderInfoLog(fragment_shader, 1000, &len, log);

	// program
	program=glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	// 如果是绑定索引,需要在glLinkProgram之前绑定,但是绑定会引起其他的问题,所以不使用绑定
	//glBindAttribLocation(program, ATTRIB_VERTEX, "position");
	//glBindAttribLocation(program, ATTRIB_COLOR, "color");
	//glBindAttribLocation(program, ATTRIB_TEXTUREPOSITION, "inputTextureCoordinate");

    glLinkProgram(program);

	// 只有在glLinkProgram之后才能获取
	mPositionHandle = glGetAttribLocation(program, "position");
	mColourHandle = glGetAttribLocation(program, "color");
	mTextureCoordHandle = glGetAttribLocation(program, "inputTextureCoordinate");

	uniform_view_matrix=glGetUniformLocation(program, "modelViewMatrix");
	uniform_projection_matrix=glGetUniformLocation(program, "projectionMatrix");

	#endif
	#endif

	return true;
}

// eng: Destroying
void txMP_Device::Destroy()
{
	MP_Device::Destroy();

	#ifdef OPENGL_WIN
	if (hRC)											// eng: Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// eng: Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// eng: Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// eng: Set RC To NULL
	}

	if (hDC && !ReleaseDC(hwnd,hDC))					// eng: Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// eng: Set DC To NULL
	}
	#endif

	#ifdef OPENGL_SHADER
	#ifndef COCOS_2DX
	if (vertex_shader)
	{
		glDeleteShader(vertex_shader);
		vertex_shader=0;
	}

	if (fragment_shader)
	{
		glDeleteShader(fragment_shader);
		fragment_shader=0;
	}

	if (program)
	{
		glDeleteProgram(program);
		program=0;
	}
	#endif
	#endif
}

// eng: Beginning of scene drawing
void txMP_Device::BeginScene(txCamera* txcamera)
{
	MP_Device::BeginScene(txcamera);

	#ifdef OPENGL_SAVE_ATTRIBUTES
	SaveAttributes();
	#endif

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	MAGIC_MATRIX world;
	MatrixIdentity(&world);
	#ifdef OPENGL_SHADER

	glUseProgram(program);
	//glEnableVertexAttribArray(ATTRIB_VERTEX);
	//glEnableVertexAttribArray(ATTRIB_COLOR);
	//glEnableVertexAttribArray(ATTRIB_TEXTUREPOSITION);
	glEnableVertexAttribArray(mPositionHandle);
	glEnableVertexAttribArray(mColourHandle);
	glEnableVertexAttribArray(mTextureCoordHandle);

    #ifndef MAGIC_3D
    world._41=(float)(-window_width/2);
    world._42=(float)(window_height/2);
	if (Magic_GetAxis()==MAGIC_pXnY)
		world._42=-world._42;
    #endif

	MATRIX4 worldMatrix(1.0f);
	worldMatrix[0].x = 1.0f;
	worldMatrix[1].y = 1.0f;
	worldMatrix[2].z = 1.0f;
	worldMatrix[3].x = 0.0f;
	worldMatrix[3].y = 0.0f;
	worldMatrix[3].z = 0.0f;
	const MATRIX4& viewMatrix = txcamera->getViewMatrix();
	const MATRIX4& projectionMatrix = txcamera->getPerspectiveMatrix();
	MATRIX4 view_world = viewMatrix * worldMatrix;
	glUniformMatrix4fv(uniform_view_matrix, 1, false, txMath::toPtr(view_world));
	glUniformMatrix4fv(uniform_projection_matrix, 1, false, txMath::toPtr(projectionMatrix));

	VECTOR3 cameraPos;
	cameraPos.x = viewMatrix[3].x;
	cameraPos.y = viewMatrix[3].y;
	cameraPos.z = viewMatrix[3].z;
	MATRIX3 matRot(viewMatrix);
	VECTOR3 cameraDir = txMath::AXIS_Z * matRot;
	#ifdef MAGIC_3D
	MAGIC_CAMERA camera;
	camera.pos.x = cameraPos.x;
	camera.pos.y = cameraPos.y;
	camera.pos.z = cameraPos.z;
	camera.dir.x = cameraDir.x;
	camera.dir.y = cameraDir.y;
	camera.dir.z = cameraDir.z;
	Magic_SetCamera(&camera);
	#endif

	#else
	SetWorldMatrix(&world);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//glEnableClientState(GL_INDEX_ARRAY);
	#endif

	#ifdef OPENGL_WIN
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// eng: Black Background
	//glClear(GL_COLOR_BUFFER_BIT);			// eng: Clear Screen
	#endif
}

// eng: End of scene drawing
void txMP_Device::EndScene()
{
	glDisableVertexAttribArray(mPositionHandle);
	glDisableVertexAttribArray(mColourHandle);
	glDisableVertexAttribArray(mTextureCoordHandle);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	#ifdef OPENGL_SAVE_ATTRIBUTES
	RestoreAttributes();
	#endif

	#ifdef OPENGL_WIN
	SwapBuffers(hDC);						// eng: Swap Buffers (Double Buffering)
	#endif
}

// eng: Indicates that device is lost
bool txMP_Device::IsLost()
{
	return false;
}

void txMP_Device::SetWorldMatrix(MAGIC_MATRIX* m)
{
#ifndef OPENGL_SHADER
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((const float*)m);
#ifndef MAGIC_3D
	glTranslatef(-window_width/2.f,-window_height/2.f,0);
#endif
#endif
}

void txMP_Device::SetViewMatrix(MAGIC_MATRIX* m)
{
	view=*m;

#ifndef OPENGL_SHADER
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const float*)&projection);
	glMultMatrixf((const float*)&view);
#endif
}

void txMP_Device::SetProjectionMatrix(MAGIC_MATRIX* m)
{
	projection=*m;

#ifdef OPENGL_WIN
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const float*)&projection);
	glMultMatrixf((const float*)&view);
#endif
}

#ifdef OPENGL_SAVE_ATTRIBUTES
void txMP_Device::SaveAttributes()
{
	glGetBooleanv(GL_BLEND, &attr_GL_BLEND);

	glGetIntegerv(GL_BLEND_SRC_ALPHA, &attr_GL_BLEND_SRC_ALPHA);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &attr_GL_BLEND_DST_ALPHA);

	glGetBooleanv(GL_DEPTH_WRITEMASK, &attr_GL_DEPTH_WRITEMASK);
	glGetBooleanv(GL_DEPTH_TEST, &attr_GL_DEPTH_TEST);

	#ifdef OPENGL_SHADER
	glGetIntegerv(GL_CURRENT_PROGRAM, &prev_program);

    glGetVertexAttribiv(ATTRIB_VERTEX, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attr_ATTRIB_VERTEX);
	glGetVertexAttribiv(ATTRIB_COLOR, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attr_ATTRIB_COLOR);
	glGetVertexAttribiv(ATTRIB_TEXTUREPOSITION, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attr_ATTRIB_TEXTUREPOSITION);

	//GetVertexBufferAttributes(ATTRIB_VERTEX, &attr_vertex_buffer);
	//GetVertexBufferAttributes(ATTRIB_COLOR, &attr_color_buffer);
	//GetVertexBufferAttributes(ATTRIB_TEXTUREPOSITION, &attr_textureposition_buffer);

	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &attr_GL_ARRAY_BUFFER_BINDING);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &attr_GL_ELEMENT_ARRAY_BUFFER_BINDING);

	#else
	glGetBooleanv(GL_TEXTURE_2D, &attr_GL_TEXTURE_2D);
	glGetBooleanv(GL_COLOR_MATERIAL, &attr_GL_COLOR_MATERIAL);
	glGetBooleanv(GL_LIGHTING, &attr_GL_LIGHTING);
	glGetBooleanv(GL_VERTEX_ARRAY, &attr_GL_VERTEX_ARRAY);
	glGetBooleanv(GL_TEXTURE_COORD_ARRAY, &attr_GL_TEXTURE_COORD_ARRAY);
	glGetBooleanv(GL_COLOR_ARRAY, &attr_GL_COLOR_ARRAY);
	#endif

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);
}

void txMP_Device::RestoreAttributes()
{
	if (attr_GL_BLEND)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glBlendFunc(attr_GL_BLEND_SRC_ALPHA, attr_GL_BLEND_DST_ALPHA);

	if (attr_GL_DEPTH_WRITEMASK)
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);

	if (attr_GL_DEPTH_TEST)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	#ifdef OPENGL_SHADER
	glUseProgram(prev_program);

	if (attr_ATTRIB_VERTEX)
		glEnableVertexAttribArray(ATTRIB_VERTEX);
	else
		glDisableVertexAttribArray(ATTRIB_VERTEX);

	if (attr_ATTRIB_COLOR)
		glEnableVertexAttribArray(ATTRIB_COLOR);
	else
		glDisableVertexAttribArray(ATTRIB_COLOR);

	if (attr_ATTRIB_TEXTUREPOSITION)
		glEnableVertexAttribArray(ATTRIB_TEXTUREPOSITION);
	else
		glDisableVertexAttribArray(ATTRIB_TEXTUREPOSITION);

	//BUFFER_ATTRIBUTES* s=&attr_vertex_buffer;
	//glVertexAttribPointer(ATTRIB_VERTEX, s->size, s->type, s->normalized, s->stride,  s->pointer);
	//s=&attr_color_buffer;
	//glVertexAttribPointer(ATTRIB_COLOR, s->size, s->type, s->normalized, s->stride,  s->pointer);
	//s=&attr_textureposition_buffer;
	//glVertexAttribPointer(ATTRIB_TEXTUREPOSITION, s->size, s->type, s->normalized, s->stride,  s->pointer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attr_GL_ELEMENT_ARRAY_BUFFER_BINDING);
	glBindBuffer(GL_ARRAY_BUFFER, attr_GL_ARRAY_BUFFER_BINDING);

	#else
	if (attr_GL_TEXTURE_2D)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	if (attr_GL_COLOR_MATERIAL)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);

	if (attr_GL_LIGHTING)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if (attr_GL_VERTEX_ARRAY)
		glEnableClientState(GL_VERTEX_ARRAY);
	else
		glDisableClientState(GL_VERTEX_ARRAY);

	if (attr_GL_TEXTURE_COORD_ARRAY)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (attr_GL_COLOR_ARRAY)
		glEnableClientState(GL_COLOR_ARRAY);
	else
		glDisableClientState(GL_COLOR_ARRAY);

	//glEnableClientState(GL_INDEX_ARRAY);
	#endif

	BindTexture(prev_texture);
}
#endif
