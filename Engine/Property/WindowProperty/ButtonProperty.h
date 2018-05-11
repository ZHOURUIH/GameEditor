#ifndef _BUTTON_PROPERTY_H_
#define _BUTTON_PROPERTY_H_

#include "WindowProperty.h"

WINDOW_PROPERTY(Button);

//---------------------------------------------------------------------------------------------------------------------------------------------
// 普通状态图片
class ButtonPropertyNormalTexture : public ButtonProperty<txTextureProperty, PTexture>
{
public:
	ButtonPropertyNormalTexture();
	virtual void setRealValue(const PTexture& value, Button* window);
	virtual PTexture getRealValue(Button* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 鼠标悬停状态图片
class ButtonPropertyHoverTexture : public ButtonProperty<txTextureProperty, PTexture>
{
public:
	ButtonPropertyHoverTexture();
	virtual void setRealValue(const PTexture& value, Button* window);
	virtual PTexture getRealValue(Button* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 点击状态图片
class ButtonPropertyPushedTexture : public ButtonProperty<txTextureProperty, PTexture>
{
public:
	ButtonPropertyPushedTexture();
	virtual void setRealValue(const PTexture& value, Button* window);
	virtual PTexture getRealValue(Button* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 禁用状态图片
class ButtonPropertyDisabledTexture : public ButtonProperty<txTextureProperty, PTexture>
{
public:
	ButtonPropertyDisabledTexture();
	virtual void setRealValue(const PTexture& value, Button* window);
	virtual PTexture getRealValue(Button* window);
};

#endif