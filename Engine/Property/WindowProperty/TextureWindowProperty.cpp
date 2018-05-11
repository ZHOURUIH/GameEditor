#include "TextureWindowProperty.h"

TextureWindowPropertyTexture::TextureWindowPropertyTexture()
{
	mPropertyName = "Texture";
	mDescription = "设置纹理窗口的纹理";
}
void TextureWindowPropertyTexture::setRealValue(const PTexture& value, TextureWindow* window)
{
	window->setTexture(const_cast<txTexture*>(value));
}
txTexture* TextureWindowPropertyTexture::getRealValue(TextureWindow* window)
{
	return window->getTexture();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextureWindowPropertyMaskTexture::TextureWindowPropertyMaskTexture()
{
	mPropertyName = "MaskTexture";
	mDescription = "设置纹理窗口的遮挡纹理";
}
void TextureWindowPropertyMaskTexture::setRealValue(const PTexture& value, TextureWindow* window)
{
	window->setMaskTexture(const_cast<txTexture*>(value));
}
txTexture* TextureWindowPropertyMaskTexture::getRealValue(TextureWindow* window)
{
	return window->getMaskTexture();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextureWindowPropertyAutoSize::TextureWindowPropertyAutoSize()
{
	mPropertyName = "AutoSize";
	mDescription = "是否自动获取纹理大小";
}
void TextureWindowPropertyAutoSize::setRealValue(const bool& value, TextureWindow* window)
{
	window->setAutoSize(value);
}
bool TextureWindowPropertyAutoSize::getRealValue(TextureWindow* window)
{
	return window->getAutoSize();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextureWindowPropertyTextureCoord::TextureWindowPropertyTextureCoord()
{
	mPropertyName = "TextureCoord";
	mDescription = "设置纹理窗口的纹理坐标";
}
void TextureWindowPropertyTextureCoord::setRealValue(const VECTOR2& value, TextureWindow* window)
{
	window->setTextureCoord(value, false);
}
VECTOR2 TextureWindowPropertyTextureCoord::getRealValue(TextureWindow* window)
{
	return window->getTextureCoord();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextureWindowPropertyTextureSize::TextureWindowPropertyTextureSize()
{
	mPropertyName = "TextureSize";
	mDescription = "设置纹理窗口的纹理大小";
}
void TextureWindowPropertyTextureSize::setRealValue(const VECTOR2& value, TextureWindow* window)
{
	window->setTextureSize(value, false);
}
VECTOR2 TextureWindowPropertyTextureSize::getRealValue(TextureWindow* window)
{
	return window->getTextureSize();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextureWindowPropertyArcProgress::TextureWindowPropertyArcProgress()
{
	mPropertyName = "ArcProgress";
	mDescription = "扇形剪切百分比,0~1";
}
void TextureWindowPropertyArcProgress::setRealValue(const float& value, TextureWindow* window)
{
	window->setArcProgress(value);
}
float TextureWindowPropertyArcProgress::getRealValue(TextureWindow* window)
{
	return window->getArcProgress();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextureWindowPropertyLinearDodge::TextureWindowPropertyLinearDodge()
{
	mPropertyName = "LinearDodge";
	mDescription = "是否使用线性减淡叠加效果";
}
void TextureWindowPropertyLinearDodge::setRealValue(const bool& value, TextureWindow* window)
{
	window->setLinearDodge(value);
}
bool TextureWindowPropertyLinearDodge::getRealValue(TextureWindow* window)
{
	return window->getLinearDodge();
}