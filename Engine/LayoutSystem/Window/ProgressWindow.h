#ifndef _PROGRESS_WINDOW_H_
#define _PROGRESS_WINDOW_H_

#include "ComplexWindow.h"

class TextureWindow;
class ProgressWindow : public ComplexWindow
{
public:
	ProgressWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~ProgressWindow(){}
	virtual void init(Layout* layout);
	void setProgress(const float& percent, const bool& refreshNow = false);
	const float& getProgress() { return mProgress; }
	TextureWindow* getBackgroundWindow() { return mBackground; }
	TextureWindow* getSlotWindow() { return mSlot; }
	TextureWindow* getBarWindow() { return mBar; }
	void setOriginBarWindowSize(const txPoint& size);
	void setOriginBarWindowPosition(const txPoint& position);
	const VECTOR2& getOriginBarTextureSize() { return mOriginBarTextureSize; }
	const txPoint& getOriginBarWindowSize() { return mOriginBarWindowSize; }
	const txPoint& getOriginBarWindowPosition() { return mOriginBarWindowPosition; }
	void setLeftToRight(const bool& leftToRight);
	const bool& getLeftToRight(){ return mLeftToRight; }
	virtual void notifyChildRectChanged(Window* child);
	virtual void notifyChildTextureSizeChanged(Window* child);
	virtual void notifyChildTextureChanged(Window* child);
	virtual void saveWindow(std::string& windowStream);
	virtual void saveTemplate(std::string& windowStream);
protected:
	virtual void initProperty();
	virtual void renameInnerChildren(const std::string& namePrefix);
protected:
	VECTOR2 mOriginBarTextureSize;
	txPoint mOriginBarWindowSize;
	txPoint mOriginBarWindowPosition;
	TextureWindow* mBackground; // 进度条的背景
	TextureWindow* mSlot;		// 进度条的槽
	TextureWindow* mBar;		// 进度条的条
	bool mLeftToRight;			// 进度条是从左到右(true)还是从右到左(false)
	float mProgress;
	bool mChangindChildRect;	// 是否正在改变子窗口的窗口矩形
	bool mChangingChildTextureSize;	// 是否正在改变子窗口的图片大小
};

#endif
