#include "TextureAnimWindowProperty.h"

txVector<std::string> TextureAnimWindowPropertyTextureAnimSet::mChoices;
TextureAnimWindowPropertyTextureAnimSet::TextureAnimWindowPropertyTextureAnimSet()
{
	mPropertyName = "TextureAnimSet";
	mDescription = "设置序列帧窗口的序列帧";
}
void TextureAnimWindowPropertyTextureAnimSet::setRealValue(const std::string& value, TextureAnimWindow* window)
{
	window->setTextureSet(value, mAsync);
}
std::string TextureAnimWindowPropertyTextureAnimSet::getRealValue(TextureAnimWindow* window)
{
	return window->getTextureSet();
}

//---------------------------------------------------------------------------------------------------------------------
TextureAnimWindowPropertyFrameCount::TextureAnimWindowPropertyFrameCount()
{
	mPropertyName = "FrameCount";
	mDescription = "序列帧帧数";
	mReadOnly = true;
}
void TextureAnimWindowPropertyFrameCount::setRealValue(const int& value, TextureAnimWindow* window)
{
	// 只读属性,不作处理
}
int TextureAnimWindowPropertyFrameCount::getRealValue(TextureAnimWindow* window)
{
	return window->getTextureFrameCount();
}

//---------------------------------------------------------------------------------------------------------------------
TextureAnimWindowPropertyInterval::TextureAnimWindowPropertyInterval()
{
	mPropertyName = "Interval";
	mDescription = "序列帧切换间隔";
}
void TextureAnimWindowPropertyInterval::setRealValue(const float& value, TextureAnimWindow* window)
{
	window->setInterval(value);
}
float TextureAnimWindowPropertyInterval::getRealValue(TextureAnimWindow* window)
{
	return window->getInterval();
}

//---------------------------------------------------------------------------------------------------------------------
TextureAnimWindowPropertyLoopMode::TextureAnimWindowPropertyLoopMode()
{
	mPropertyName = "LoopMode";
	mDescription = "序列帧循环播放";
	ADD_ENUM(LM_NONE);
	ADD_ENUM(LM_NORMAL);
	ADD_ENUM(LM_PINGPONG);
}
void TextureAnimWindowPropertyLoopMode::setRealValue(const LOOP_MODE& value, TextureAnimWindow* window)
{
	window->setLoop(value);
}
LOOP_MODE TextureAnimWindowPropertyLoopMode::getRealValue(TextureAnimWindow* window)
{
	return window->getLoop();
}

//---------------------------------------------------------------------------------------------------------------------
TextureAnimWindowPropertyPlayState::TextureAnimWindowPropertyPlayState()
{
	mPropertyName = "AnimPlayState";
	mDescription = "序列帧播放状态";
	ADD_ENUM(PS_PLAY);
	ADD_ENUM(PS_PAUSE);
	ADD_ENUM(PS_STOP);
}
void TextureAnimWindowPropertyPlayState::setRealValue(const PLAY_STATE& value, TextureAnimWindow* window)
{
	window->setPlayState(value);
}
PLAY_STATE TextureAnimWindowPropertyPlayState::getRealValue(TextureAnimWindow* window)
{
	return window->getPlayState();
}

//---------------------------------------------------------------------------------------------------------------------
TextureAnimWindowPropertyStartIndex::TextureAnimWindowPropertyStartIndex()
{
	mPropertyName = "StartIndex";
	mDescription = "序列帧起始下标";
}
void TextureAnimWindowPropertyStartIndex::setRealValue(const int& value, TextureAnimWindow* window)
{
	window->setStartIndex(value);
}
int TextureAnimWindowPropertyStartIndex::getRealValue(TextureAnimWindow* window)
{
	return window->getStartIndex();
}

//---------------------------------------------------------------------------------------------------------------------
TextureAnimWindowPropertyEndIndex::TextureAnimWindowPropertyEndIndex()
{
	mPropertyName = "EndIndex";
	mDescription = "序列帧终止下标";
}
void TextureAnimWindowPropertyEndIndex::setRealValue(const int& value, TextureAnimWindow* window)
{
	window->setEndIndex(value);
}
int TextureAnimWindowPropertyEndIndex::getRealValue(TextureAnimWindow* window)
{
	return window->getEndIndex();
}