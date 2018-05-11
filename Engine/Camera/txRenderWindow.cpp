#include "txEngineDefine.h"
#include "txRenderWindow.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"
#include "Utility.h"
#include "freeimage/FreeImage.h"

txRenderWindow::txRenderWindow()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	mHRC = NULL;
	mHDC = NULL;
#endif
	mInitErrorInfo = EMPTY_STRING;
	mWindowName = EMPTY_STRING;
	mBackgroundColour = VECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	mScreenInfo.density = 1.0f;
	mScreenInfo.densityDPI = 160;
	mScreenInfo.xDPI = 160.0f;
	mScreenInfo.yDPI = 160.0f;
	mScreenInfo.screenWidth = 0;
	mScreenInfo.screenHeight = 0;
}
txRenderWindow::~txRenderWindow()
{
	destroy();
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
void txRenderWindow::init(const std::string& name, HWND hwnd, const int& coloutBits, const int& x, const int& y)
#elif RUN_PLATFORM == PLATFORM_ANDROID
void txRenderWindow::init(const std::string& name, const int& x, const int& y)
#endif
{
	mWindowName = name;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	mHWND = hwnd;
	mHDC = GetDC(mHWND);
	// 设置像素格式
	glewExperimental = GL_TRUE;
	PIXELFORMATDESCRIPTOR pfd;   // pfd Tells Windows How We Want Things To Be
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = coloutBits;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cDepthBits = 16;
	int visual = ChoosePixelFormat(mHDC, &pfd);
	SetPixelFormat(mHDC, visual, &pfd);
	
	// 初始化glew
	mHRC = wglCreateContext(mHDC);
	if (mHRC == NULL)
	{
		ENGINE_ERROR("error : wglCreateContext error!");
		return;
	}
	BOOL ret = wglMakeCurrent(mHDC, mHRC);
	if (ret != TRUE)
	{
		ENGINE_ERROR("error : wglMakeCurrent error!");
		return;
	}
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		ENGINE_ERROR("error : glewInit error!");
		return;
	}
	glShadeModel(GL_SMOOTH);// Enables Smooth Shading
#endif
	setWindowSize(x, y);
}

void txRenderWindow::Clear()
{
	glClearColor(mBackgroundColour.r, mBackgroundColour.g, mBackgroundColour.b, mBackgroundColour.a);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void txRenderWindow::SwapBuffer()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	SwapBuffers(mHDC);
#endif
}

void txRenderWindow::destroy()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	if (mHRC != NULL)
	{
		if (!wglMakeCurrent(NULL, NULL))                 // Are We Able To Release The DC And RC Contexts?
		{}
		if (!wglDeleteContext(mHRC))                 // Are We Able To Delete The RC?
		{}
		mHRC = NULL;
	}
	mHDC = NULL;
#endif
}

void txRenderWindow::snapScreen(const std::string& fileName)
{
	int bufferSize = mScreenInfo.screenWidth * mScreenInfo.screenHeight * 3;
	GLubyte* buffer = TRACE_NEW_ARRAY(GLubyte, bufferSize, buffer);
	glReadPixels(0, 0, mScreenInfo.screenWidth, mScreenInfo.screenHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);

	// 分配图片,设置颜色数据
	FIBITMAP* bitmapRet = FreeImage_Allocate(mScreenInfo.screenWidth, mScreenInfo.screenHeight, 32);
	for (int y = 0; y < mScreenInfo.screenHeight; ++y)
	{
		BYTE* bits = FreeImage_GetScanLine(bitmapRet, y);
		for (int x = 0; x < mScreenInfo.screenWidth; ++x)
		{
			bits[x * 4 + 0] = buffer[y * mScreenInfo.screenWidth * 3 + 3 * x + 0];
			bits[x * 4 + 1] = buffer[y * mScreenInfo.screenWidth * 3 + 3 * x + 1];
			bits[x * 4 + 2] = buffer[y * mScreenInfo.screenWidth * 3 + 3 * x + 2];
			bits[x * 4 + 3] = 255;
		}
	}
	TRACE_DELETE_ARRAY(buffer);

	// 保存为png
	if (!txFileUtility::isFileExist(P_SNAP_SCREEN_PATH))
	{
		txFileUtility::createFolder(P_SNAP_SCREEN_PATH);
	}
	FreeImage_Save(FIF_PNG, bitmapRet, (P_SNAP_SCREEN_PATH + fileName + ".png").c_str());
	FreeImage_Unload(bitmapRet);
}

void txRenderWindow::setVAsync(const bool& vsync)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	// 设置垂直同步
	wglSwapIntervalEXT(vsync ? 1 : 0);
#endif
}