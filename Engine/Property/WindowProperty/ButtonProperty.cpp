#include "ButtonProperty.h"

ButtonPropertyNormalTexture::ButtonPropertyNormalTexture()
{
	mPropertyName = "NormalTexture";
	mDescription = "设置按钮普通状态图片";
}
void ButtonPropertyNormalTexture::setRealValue(const PTexture& value, Button* window)
{
	window->setButtonTexture(BS_NORMAL, const_cast<txTexture*>(value));
}
txTexture* ButtonPropertyNormalTexture::getRealValue(Button* window)
{
	return window->getButtonTexture(BS_NORMAL);
}

//------------------------------------------------------------------------------------------
ButtonPropertyHoverTexture::ButtonPropertyHoverTexture()
{
	mPropertyName = "HoverTexture";
	mDescription = "设置按钮鼠标悬停状态图片";
}
void ButtonPropertyHoverTexture::setRealValue(const PTexture& value, Button* window)
{
	window->setButtonTexture(BS_HOVER, const_cast<txTexture*>(value));
}
txTexture* ButtonPropertyHoverTexture::getRealValue(Button* window)
{
	return window->getButtonTexture(BS_HOVER);
}

//------------------------------------------------------------------------------------------
ButtonPropertyPushedTexture::ButtonPropertyPushedTexture()
{
	mPropertyName = "PushedTexture";
	mDescription = "设置按钮禁点击状态图片";
}
void ButtonPropertyPushedTexture::setRealValue(const PTexture& value, Button* window)
{
	window->setButtonTexture(BS_PUSHED, const_cast<txTexture*>(value));
}
txTexture* ButtonPropertyPushedTexture::getRealValue(Button* window)
{
	return window->getButtonTexture(BS_PUSHED);
}

//------------------------------------------------------------------------------------------
ButtonPropertyDisabledTexture::ButtonPropertyDisabledTexture()
{
	mPropertyName = "DisabledTexture";
	mDescription = "设置按钮禁用状态图片";
}
void ButtonPropertyDisabledTexture::setRealValue(const PTexture& value, Button* window)
{
	window->setButtonTexture(BS_DISABLED, const_cast<txTexture*>(value));
}
txTexture* ButtonPropertyDisabledTexture::getRealValue(Button* window)
{
	return window->getButtonTexture(BS_DISABLED);
}