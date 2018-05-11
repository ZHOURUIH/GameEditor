#include "txComponentKeyFrame.h"
#include "txTremblingManager.h"
#include "txTrembling.h"
#include "txEngineLog.h"
#include "PropertyHeader.h"

void txComponentKeyFrame::initProperty()
{
	txComponent::initProperty();
	addProperty<txKeyFramePropertyTrembling>();
	addProperty<txKeyFramePropertyOnceLength>();
	addProperty<txKeyFramePropertyAmplitude>();
	addProperty<txKeyFramePropertyCurrentTime>();
	addProperty<txKeyFramePropertyOffset>();
	addProperty<txKeyFramePropertyCurState>();
	addProperty<txKeyFramePropertyLoop>();
	addProperty<txKeyFramePropertyFullOnce>();

	// 为下拉列表添加数据
	txKeyFramePropertyTrembling* tremblingProperty = getProperty<txKeyFramePropertyTrembling>();
	if (tremblingProperty->getChoices() != NULL && tremblingProperty->getChoices()->size() == 0)
	{
		txVector<std::string> fileList;
		auto& temblingList = mTremblingManager->getTremblingList();
		auto iter = temblingList.begin();
		auto iterEnd = temblingList.end();
		FOR_STL(temblingList, ; iter != iterEnd; ++iter)
		{
			fileList.push_back(iter->first);
		}
		END_FOR_STL(temblingList);
		tremblingProperty->setChoices(fileList);
	}
}

void txComponentKeyFrame::update(const float& elapsedTime)
{
	txComponent::update(elapsedTime);
	if (mKeyFrame != NULL && mPlayState == PS_PLAY)
	{
		mCurrentTime += elapsedTime;
		mPlayedTime += elapsedTime;

		bool done = false;
		// 无限播放当前震动
		if (mPlayLength < 0.0f)
		{
			if (mCurrentTime > mOnceLength)
			{
				mCurrentTime = 0.0f;
			}
		}
		// 播放固定长度的震动
		else
		{
			// 超过时间则停止,暂时不播放最后一帧
			if (mPlayedTime > mPlayLength)
			{
				done = true;
				mCurrentTime = mOffset + mPlayLength;
			}
			else if (mCurrentTime > mOnceLength)
			{
				mCurrentTime = 0.0f;
			}
		}

		if (mKeyFrame != NULL)
		{
			mKeyFrame->queryValue(mCurrentTime / mOnceLength, mCurValueList, mAmplitude);
			if (mCurValueList.size() > 0)
			{
				applyKeyFrame(mCurValueList);
			}
			afterApllyTrembling(done);
		}
	}
}

void txComponentKeyFrame::play(const std::string& name, const bool& loop, const float& onceLength, const float& offset, const bool& fullOnce, const float& amplitude)
{
	setTrembling(name);
	mKeyFrame = mTremblingManager->getTrembling(mTremblingName);
	if (mKeyFrame == NULL || txMath::isFloatZero(onceLength))
	{
		mStopValue.clear();
		// 停止并禁用组件
		afterApllyTrembling(true);
		return;
	}
	else
	{
		mStopValue = mKeyFrame->queryValue(mKeyFrame->getLength(), mAmplitude);
	}
	if (offset > onceLength)
	{
		ENGINE_ERROR("offset must be less than onceLength!");
		return;
	}
	mOnceLength = onceLength;
	mPlayState = PS_PLAY;
	mLoop = loop;
	mOffset = offset;
	mCurrentTime = mOffset;
	mAmplitude = amplitude;
	mPlayedTime = 0.0f;
	if (mLoop)
	{
		mPlayLength = -1.0f;
	}
	else
	{
		if (fullOnce)
		{
			mPlayLength = mOnceLength;
		}
		else
		{
			mPlayLength = mOnceLength - offset;
		}
	}
}

void txComponentKeyFrame::stop(const bool& force)
{
	// 如果已经是停止的状态,并且不是要强制停止,则不再执行
	if (mPlayState == PS_STOP && !force)
	{
		return;
	}
	// 构建值全部为默认值的关键帧
	if (mComponentOwner != NULL)
	{
		applyKeyFrame(mStopValue);
	}
	mPlayState = PS_STOP;
	mKeyFrame = NULL;
	mCurrentTime = 0.0f;
	mPlayedTime = 0.0f;
}

void txComponentKeyFrame::setState(const PLAY_STATE& state)
{
	if (mPlayState == state)
	{
		return;
	}
	if (state == PS_PLAY)
	{
		play(mTremblingName, mLoop, mOnceLength, mOffset, mFullOnce, mAmplitude);
	}
	else if (state == PS_STOP)
	{
		stop();
	}
	else if (state == PS_PAUSE)
	{
		pause();
	}
}

void txComponentKeyFrame::setCallback(KeyFrameCallback callback, void* userData, KeyFrameCallback& curCallback, void*& curUserData, txComponentKeyFrame* component)
{
	KeyFrameCallback tempCallback = curCallback;
	void* tempUserData = curUserData;
	curCallback = NULL;
	curUserData = NULL;
	// 如果回调函数当前不为空,则是中断了正在进行的变化
	if (tempCallback != NULL)
	{
		tempCallback(component, tempUserData, true, false);
	}
	curCallback = callback;
	curUserData = userData;
}