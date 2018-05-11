#ifndef _TEXTURE_ANIM_WINDOW_PROPERTY_H_
#define _TEXTURE_ANIM_WINDOW_PROPERTY_H_

#include "WindowProperty.h"

WINDOW_PROPERTY(TextureAnimWindow)

//---------------------------------------------------------------------------------------------------------------------------------------------
//序列帧
class TextureAnimWindowPropertyTextureAnimSet : public TextureAnimWindowProperty<txStringProperty, std::string>
{
public:
	TextureAnimWindowPropertyTextureAnimSet();
	virtual void setRealValue(const std::string& value, TextureAnimWindow* window);
	virtual std::string getRealValue(TextureAnimWindow* window);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(const txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 序列帧的帧数
class TextureAnimWindowPropertyFrameCount : public TextureAnimWindowProperty<txIntProperty, int>
{
public:
	TextureAnimWindowPropertyFrameCount();
	virtual void setRealValue(const int& value, TextureAnimWindow* window);
	virtual int getRealValue(TextureAnimWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 序列帧切换间隔
class TextureAnimWindowPropertyInterval : public TextureAnimWindowProperty<txFloatProperty, float>
{
public:
	TextureAnimWindowPropertyInterval();
	virtual void setRealValue(const float& value, TextureAnimWindow* window);
	virtual float getRealValue(TextureAnimWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 序列帧循环播放
class TextureAnimWindowPropertyLoopMode : public TextureAnimWindowProperty<txEnumProperty<LOOP_MODE>, LOOP_MODE>
{
public:
	TextureAnimWindowPropertyLoopMode();
	virtual void setRealValue(const LOOP_MODE& value, TextureAnimWindow* window);
	virtual LOOP_MODE getRealValue(TextureAnimWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 序列帧播放状态
class TextureAnimWindowPropertyPlayState : public TextureAnimWindowProperty<txEnumProperty<PLAY_STATE>, PLAY_STATE>
{
public:
	TextureAnimWindowPropertyPlayState();
	virtual void setRealValue(const PLAY_STATE& value, TextureAnimWindow* window);
	virtual PLAY_STATE getRealValue(TextureAnimWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 序列帧起始下标
class TextureAnimWindowPropertyStartIndex : public TextureAnimWindowProperty<txIntProperty, int>
{
public:
	TextureAnimWindowPropertyStartIndex();
	virtual void setRealValue(const int& value, TextureAnimWindow* window);
	virtual int getRealValue(TextureAnimWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 序列帧终止下标
class TextureAnimWindowPropertyEndIndex : public TextureAnimWindowProperty<txIntProperty, int>
{
public:
	TextureAnimWindowPropertyEndIndex();
	virtual void setRealValue(const int& value, TextureAnimWindow* window);
	virtual int getRealValue(TextureAnimWindow* window);
};

#endif