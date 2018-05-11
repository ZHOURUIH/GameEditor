#ifndef _EDITBOX_H_
#define _EDITBOX_H_

#include "ComplexWindow.h"

class txTexture;
class TextureWindow;
class TextWindow;
class EditBox : public ComplexWindow
{
public:
	EditBox(const std::string& name, const WINDOW_TYPE& type);
	virtual ~EditBox(){}
	virtual void init(Layout* layout);
	virtual void update(const float& elapsedTime);
	virtual void setFocus(const bool& focus);
	void clearText(const bool& async = false);
	void appendText(const std::string& text, const bool& async = false);
	void eraseLastText(const bool& async = false);
	void setShowCaret(const bool& show){ mShowCaret = show; }
	const bool& getShowCaret() { return mShowCaret; }
	void setMaxTextLength(const int& length){ mMaxTextLength = length; }
	const int& getMaxTextLength() { return mMaxTextLength; }

	TextureWindow* getBackgroundWindow() { return mBackground; }
	TextureWindow* getCaretWindow() { return mCaret; }
	TextWindow* getTextWindow() { return mTextWindow; }
	TextureWindow* getTextTipWindow() { return mTextWindowTip; }

	// 处理鼠标键盘输入消息
	virtual void processKey(const float& elapsedTime);
	virtual void onLeftDown(const VECTOR2& position);
	virtual void onRightDown(const VECTOR2& position);
	virtual void onMouseLeave();
	virtual void onMouseMove(const VECTOR2& position);
	virtual void setText(const std::string& text, const bool& async);
	virtual const std::string& getText();
	virtual void notifyPixelSizeChanged() { mNeedUpdateCaret = true; }
protected:
	virtual void initProperty();
	virtual void renameInnerChildren(const std::string& namePrefix);
	void updateCaretPos();
protected:
	TextureWindow* mBackground;
	TextureWindow* mCaret;
	TextWindow* mTextWindow;	// 用于显示编辑框内的文字
	TextureWindow* mTextWindowTip;
	bool mShowCaret;
	float mTimeCount;	// 用于更新光标闪烁的计时
	int mMaxTextLength;
	bool mNeedUpdateCaret;
};

#endif
