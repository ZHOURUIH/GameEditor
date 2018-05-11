#ifndef _TEXTURE_WINDOW_PROPERTY_H_
#define _TEXTURE_WINDOW_PROPERTY_H_

#include "WindowProperty.h"

WINDOW_PROPERTY(TextureWindow)

//---------------------------------------------------------------------------------------------------------------------------------------------
// 纹理
class TextureWindowPropertyTexture : public TextureWindowProperty<txTextureProperty, PTexture>
{
public:
	TextureWindowPropertyTexture();
	virtual void setRealValue(const PTexture& value, TextureWindow* window);
	virtual PTexture getRealValue(TextureWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 遮挡纹理
class TextureWindowPropertyMaskTexture : public TextureWindowProperty<txTextureProperty, PTexture>
{
public:
	TextureWindowPropertyMaskTexture();
	virtual void setRealValue(const PTexture& value, TextureWindow* window);
	virtual PTexture getRealValue(TextureWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 是否自动获取纹理大小
class TextureWindowPropertyAutoSize : public TextureWindowProperty<txBoolProperty, bool>
{
public:
	TextureWindowPropertyAutoSize();
	virtual void setRealValue(const bool& value, TextureWindow* window);
	virtual bool getRealValue(TextureWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
//纹理坐标
class TextureWindowPropertyTextureCoord : public TextureWindowProperty<txVector2Property, VECTOR2>
{
public:
	TextureWindowPropertyTextureCoord();
	virtual void setRealValue(const VECTOR2& value, TextureWindow* window);
	virtual VECTOR2 getRealValue(TextureWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
//纹理大小
class TextureWindowPropertyTextureSize : public TextureWindowProperty<txVector2Property, VECTOR2>
{
public:
	TextureWindowPropertyTextureSize();
	virtual void setRealValue(const VECTOR2& value, TextureWindow* window);
	virtual VECTOR2 getRealValue(TextureWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 进度百分比
class TextureWindowPropertyArcProgress : public TextureWindowProperty<txFloatProperty, float>
{
public:
	TextureWindowPropertyArcProgress();
	virtual void setRealValue(const float& value, TextureWindow* window);
	virtual float getRealValue(TextureWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 是否使用线性减淡叠加效果
class TextureWindowPropertyLinearDodge : public TextureWindowProperty<txBoolProperty, bool>
{
public:
	TextureWindowPropertyLinearDodge();
	virtual void setRealValue(const bool& value, TextureWindow* window);
	virtual bool getRealValue(TextureWindow* window);
};

#endif