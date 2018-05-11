#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "TextureWindow.h"

class txTexture;
class Button : public TextureWindow
{
public:
	Button(const std::string& name, const WINDOW_TYPE& type);
	virtual ~Button(){}

	void setState(const BUTTON_STATE& state)
	{
		mState = state;
		setTexture(mButtonTextureList[mState]);
	}
	// 设置按钮指定状态的图片
	void setButtonTexture(const BUTTON_STATE& state, txTexture* texture)
	{
		mButtonTextureList[state] = texture;
		// 如果设置的是当前状态的图片,则需要重新设置当前渲染的图片
		if (mState == state)
		{
			setTexture(texture);
		}
	}
	// 获得按钮指定状态的图片
	txTexture* getButtonTexture(const BUTTON_STATE& state) { return mButtonTextureList[state]; }

	// 处理鼠标键盘输入消息
	virtual void onLeftDown(const VECTOR2& position)
	{
		TextureWindow::onLeftDown(position);
		setState(BS_PUSHED); 
	}
	virtual void onLeftUp(const VECTOR2& position)
	{
		TextureWindow::onLeftUp(position);
		setState(BS_HOVER);
		pushWindowEvent(WE_CLICKED);
	}
	virtual void onMouseEnter()
	{
		TextureWindow::onMouseEnter();
		setState(BS_HOVER); 
	}
	virtual void onMouseLeave()
	{
		TextureWindow::onMouseLeave();
		setState(BS_NORMAL); 
	}
	virtual void finishAsycLoad();
	virtual void getUsedResourceList(txVector<std::string>& resourceList);
protected:
	virtual void initProperty();
protected:
	BUTTON_STATE mState;
	txTexture* mButtonTextureList[BS_MAX_STATE];
};

#endif