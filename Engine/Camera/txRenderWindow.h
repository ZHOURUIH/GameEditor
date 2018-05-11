#ifndef _TX_RENDER_WINDOW_H_
#define _TX_RENDER_WINDOW_H_

#include "txEngineDefine.h"

class txRenderWindow
{
public:
	txRenderWindow();
	virtual ~txRenderWindow();
#if RUN_PLATFORM == PLATFORM_WINDOWS
	void init(const std::string& name, HWND hwnd, const int& coloutBits, const int& x, const int& y);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	void init(const std::string& name, const int& x, const int& y);
#endif
	const std::string& getName() { return mWindowName; }
	void Clear();
	void SwapBuffer();
	void destroy();
	void snapScreen(const std::string& fileNameNoSuffix);
	void setWindowSize(const int& width, const int& height)
	{
		mScreenInfo.screenWidth = width;
		mScreenInfo.screenHeight = height;
	}
	void setVAsync(const bool& vsync);
	void setScreenDensity(const float& density)		{ mScreenInfo.density = density; }
	void setScreenInfo(const SCREEN_INFO& screenInfo){ mScreenInfo = screenInfo; }
	const int& getWidth()							{ return mScreenInfo.screenWidth; }
	const int& getHeight()							{ return mScreenInfo.screenHeight; }
	const float& getScreenDensity()					{ return mScreenInfo.density; }
	const int& getScreenDensityDPI()				{ return mScreenInfo.densityDPI; }
	const float& getScreenDensityDPIx()				{ return mScreenInfo.xDPI; }
	const float& getScreenDensityDPIy()				{ return mScreenInfo.yDPI; }
	float getAspect() { return (float)mScreenInfo.screenWidth / (float)mScreenInfo.screenHeight; }
	void setBackgroundColour(const VECTOR4& colour) { mBackgroundColour = colour; }
	VECTOR4 getBackgroundColour() { return mBackgroundColour; }
#if RUN_PLATFORM == PLATFORM_WINDOWS
	const HDC& getHDC()								{ return mHDC; }
	const HWND& getHWND()							{ return mHWND; }
#endif
protected:
#if RUN_PLATFORM == PLATFORM_WINDOWS
	HGLRC mHRC; // Rendering Context
	HDC mHDC;   // Device Context
	HWND mHWND;
#endif
	VECTOR4 mBackgroundColour;
	std::string mWindowName; // 渲染窗口名称
	SCREEN_INFO mScreenInfo;
	std::string mInitErrorInfo;
};

#endif