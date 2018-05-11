#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include "ComplexWindow.h"

class txTexture;
class TextureWindow;
class CheckBox : public ComplexWindow
{
public:
	CheckBox(const std::string& name, const WINDOW_TYPE& type);
	virtual ~CheckBox(){}
	virtual void init(Layout* layout);
	void setCkeck(const bool& check);
	bool isChecked();
	TextureWindow* getMaskWindow() { return mMask; }
	TextureWindow* getBackgroundWindow() { return mBackground; }
	virtual void onLeftDown(const VECTOR2& position);

	// 处理鼠标键盘输入消息
protected:
	virtual void initProperty();
	virtual void renameInnerChildren(const std::string& namePrefix);
protected:
	TextureWindow* mBackground;
	TextureWindow* mMask;
};

#endif