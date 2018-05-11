#include "txKeyFrameProperty.h"

txVector<std::string> txKeyFramePropertyTrembling::mChoices;
txKeyFramePropertyTrembling::txKeyFramePropertyTrembling()
{
	mPropertyName = "KeyFrameName";
	mDescription = "关键帧文件";
}
void txKeyFramePropertyTrembling::setRealValue(const std::string& value, txComponentKeyFrame* component)
{
	component->setTrembling(value);
}
std::string txKeyFramePropertyTrembling::getRealValue(txComponentKeyFrame* component)
{
	return component->getTremblingName();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyOnceLength::txKeyFramePropertyOnceLength()
{
	mPropertyName = "KeyFrameSpeed";
	mDescription = "执行一次关键帧的时间";
}
void txKeyFramePropertyOnceLength::setRealValue(const float& value, txComponentKeyFrame* component)
{
	component->setOnceLength(value);
}
float txKeyFramePropertyOnceLength::getRealValue(txComponentKeyFrame* component)
{
	return component->getOnceLength();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyAmplitude::txKeyFramePropertyAmplitude()
{
	mPropertyName = "KeyFrameAmplitude";
	mDescription = "关键帧幅度";
}
void txKeyFramePropertyAmplitude::setRealValue(const float& value, txComponentKeyFrame* component)
{
	component->setAmplitude(value);
}
float txKeyFramePropertyAmplitude::getRealValue(txComponentKeyFrame* component)
{
	return component->getAmplitude();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyCurrentTime::txKeyFramePropertyCurrentTime()
{
	mPropertyName = "KeyFrameCurrentTime";
	mDescription = "当前关键帧时间";
}
void txKeyFramePropertyCurrentTime::setRealValue(const float& value, txComponentKeyFrame* component)
{
	component->setCurrentTime(value);
}
float txKeyFramePropertyCurrentTime::getRealValue(txComponentKeyFrame* component)
{
	return component->getCurrentTime();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyOffset::txKeyFramePropertyOffset()
{
	mPropertyName = "KeyFrameOffset";
	mDescription = "关键帧偏移";
}
void txKeyFramePropertyOffset::setRealValue(const float& value, txComponentKeyFrame* component)
{
	component->setOffset(value);
}
float txKeyFramePropertyOffset::getRealValue(txComponentKeyFrame* component)
{
	return component->getOffset();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyCurState::txKeyFramePropertyCurState()
{
	mPropertyName = "KeyFrameState";
	mDescription = "关键帧播放状态";
	ADD_ENUM(PS_PLAY);
	ADD_ENUM(PS_PAUSE);
	ADD_ENUM(PS_STOP);
}
void txKeyFramePropertyCurState::setRealValue(const PLAY_STATE& value, txComponentKeyFrame* component)
{
	component->setState(value);
}
PLAY_STATE txKeyFramePropertyCurState::getRealValue(txComponentKeyFrame* component)
{
	return component->getState();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyLoop::txKeyFramePropertyLoop()
{
	mPropertyName = "KeyFrameLoop";
	mDescription = "关键帧循环";
}
void txKeyFramePropertyLoop::setRealValue(const bool& value, txComponentKeyFrame* component)
{
	component->setLoop(value);
}
bool txKeyFramePropertyLoop::getRealValue(txComponentKeyFrame* component)
{
	return component->getLoop();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyFullOnce::txKeyFramePropertyFullOnce()
{
	mPropertyName = "KeyFrameFullOnce";
	mDescription = "关键帧是否完整的一次";
}
void txKeyFramePropertyFullOnce::setRealValue(const bool& value, txComponentKeyFrame* component)
{
	component->setFullOnce(value);
}
bool txKeyFramePropertyFullOnce::getRealValue(txComponentKeyFrame* component)
{
	return component->getFullOnce();
}