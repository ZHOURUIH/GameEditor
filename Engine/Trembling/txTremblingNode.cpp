#include "Utility.h"
#include "txTremblingNode.h"
#include "txTremblingKeyFrame.h"

void txTremblingNode::clear()
{
	mLength = 0.0f;
	int size = mKeyFrameList.size();
	FOR_STL(mKeyFrameList, int i = 0; i < size; ++i)
	{
		TRACE_DELETE(mKeyFrameList[i]);
	}
	END_FOR_STL(mKeyFrameList);
	mKeyFrameList.clear();
}

void txTremblingNode::saveTrembingNode(std::string& stream)
{
	stream += "*|" + mName + "|" + mInfo + "\r\n";
	int keyFrameCount = mKeyFrameList.size();
	FOR_STL(mKeyFrameList, int i = 0; i < keyFrameCount; ++i)
	{
		stream += txStringUtility::floatToString(mKeyFrameList[i]->getTime(), 4) + ",\t" + txStringUtility::floatToString(mKeyFrameList[i]->getOffset(), 4) + "\r\n";
	}
	END_FOR_STL(mKeyFrameList);
}

float txTremblingNode::queryKeyFrame(const float& keyTime)
{
	int index = findIndex(keyTime);
	int keyFrameCount = mKeyFrameList.size();
	if (index < 0)
	{
		return mKeyFrameList[0]->getOffset();
	}
	else if (index < keyFrameCount - 1)
	{
		float ratio = (keyTime - mKeyFrameList[index]->getTime()) / (mKeyFrameList[index + 1]->getTime() - mKeyFrameList[index]->getTime());
		float curOffset = mKeyFrameList[index]->getOffset() + (mKeyFrameList[index + 1]->getOffset() - mKeyFrameList[index]->getOffset()) * ratio;
		return curOffset;
	}
	else if (index == keyFrameCount - 1)
	{
		return mKeyFrameList[keyFrameCount - 1]->getOffset();
	}
	return 0.0f;
}

bool txTremblingNode::deleteKeyFrame(const int& index)
{
	if (!txMath::isInRange(index, 0, (int)mKeyFrameList.size() - 1))
	{
		return false;
	}
	// 销毁移除关键帧
	TRACE_DELETE(mKeyFrameList[index]);
	mKeyFrameList.erase(mKeyFrameList.begin() + index);
	// 重置后续关键帧中记录的位置
	int keyFrameCount = mKeyFrameList.size();
	FOR_STL(mKeyFrameList, int i = index; i < keyFrameCount; ++i)
	{
		mKeyFrameList[i]->setIndexInParent(i);
	}
	END_FOR_STL(mKeyFrameList);
	// 如果删除的是最后一个关键帧,则重置节点长度
	if (index == mKeyFrameList.size())
	{
		if (mKeyFrameList.size() > 0)
		{
			mLength = mKeyFrameList[mKeyFrameList.size() - 1]->getTime();
		}
		else
		{
			mLength = 0.0f;
		}
	}
	return true;
}

txTremblingKeyFrame* txTremblingNode::insertKeyFrame(const int& index, const float& time, const float& offset)
{
	if (!txMath::isInRange(index, 0, (int)mKeyFrameList.size() - 1))
	{
		return NULL;
	}
	float prevTime = 0.0f;
	float nextTime = -1.0f;
	if (index > 0 && index - 1 < (int)mKeyFrameList.size())
	{
		prevTime = mKeyFrameList[index - 1]->getTime();
	}
	if (index < (int)mKeyFrameList.size())
	{
		nextTime = mKeyFrameList[index]->getTime();
	}
	float newTime = time;
	// 如果没有指定时间点,则自动计算
	if (newTime < 0.0f)
	{
		// 有后续帧,则取中点时间
		if (nextTime > 0.0f)
		{
			newTime = (prevTime + nextTime) / 2.0f;
		}
		// 没有后续帧,则前一帧加1秒
		else
		{
			newTime = prevTime + 1.0f;
		}
	}
	// 不能小于或者等于上一个关键帧(第0个关键帧除外),不能
	if ((newTime > prevTime || (index == 0 && txMath::isFloatZero(newTime))) && (nextTime < 0.0f || newTime < nextTime))
	{
		txTremblingKeyFrame* keyFrame = TRACE_NEW(txTremblingKeyFrame, keyFrame);
		// 必须先设置父节点和在节点中的位置,再插入列表,然后才能设置关键帧时间和值
		keyFrame->setParent(this);
		keyFrame->setIndexInParent(index);
		mKeyFrameList.insert(mKeyFrameList.begin() + index, keyFrame);
		keyFrame->setKeyFrame(newTime, offset);
		int keyFrameCount = mKeyFrameList.size();
		FOR_STL(mKeyFrameList, int i = index + 1; i < keyFrameCount; ++i)
		{
			mKeyFrameList[i]->setIndexInParent(i);
		}
		END_FOR_STL(mKeyFrameList);
		return keyFrame;
	}
	return NULL;
}

bool txTremblingNode::allowedKeyFrameTime(const int& index, const float& time)
{
	if (!txMath::isInRange(index, 0, (int)mKeyFrameList.size() - 1))
	{
		return false;
	}
	float prevTime = 0.0f;
	float nextTime = -1.0f;
	if (txMath::isInRange(index - 1, 0, (int)mKeyFrameList.size() - 1))
	{
		prevTime = mKeyFrameList[index - 1]->getTime();
	}
	if (txMath::isInRange(index + 1, 0, (int)mKeyFrameList.size() - 1))
	{
		nextTime = mKeyFrameList[index + 1]->getTime();
	}
	if ((time > prevTime || (index == 0 && txMath::isFloatZero(time))) && (nextTime < 0.0f || time < nextTime))
	{
		return true;
	}
	return false;
}

void txTremblingNode::setKeyFrameList(txMap<float, float>& keyFrameList)
{
	clear();
	int size = keyFrameList.size();
	auto iterKeyFrame = keyFrameList.begin();
	auto iterKeyFrameEnd = keyFrameList.end();
	FOR_STL(keyFrameList, ; iterKeyFrame != iterKeyFrameEnd; ++iterKeyFrame)
	{
		txTremblingKeyFrame* keyFrame = TRACE_NEW(txTremblingKeyFrame, keyFrame);
		// 必须先设置父节点和在节点中的位置,再插入列表,然后才能设置关键帧时间和值
		keyFrame->setParent(this);
		keyFrame->setIndexInParent(mKeyFrameList.size());
		mKeyFrameList.push_back(keyFrame);
		keyFrame->setKeyFrame(iterKeyFrame->first, iterKeyFrame->second);
		if (keyFrame->getTime() > mLength)
		{
			mLength = keyFrame->getTime();
		}
	}
	END_FOR_STL(keyFrameList);
}

int txTremblingNode::findIndex(const float& time)
{
	int keyFrameCount = mKeyFrameList.size();
	// -1表示时间比第0个关键帧还小
	if (keyFrameCount > 0 && time <= mKeyFrameList[0]->getTime())
	{
		return -1;
	}
	// 大于最后一个关键帧的时间
	else if (keyFrameCount > 0 && time >= mKeyFrameList[keyFrameCount - 1]->getTime())
	{
		return keyFrameCount - 1;
	}
	else
	{
		int pos = -1;
		FOR_STL(mKeyFrameList, int i = 0; i < keyFrameCount - 1; ++i)
		{
			if (time >= mKeyFrameList[i]->getTime() && time <= mKeyFrameList[i + 1]->getTime())
			{
				pos = i;
				break;
			}
		}
		END_FOR_STL(mKeyFrameList);
		return pos;
	}
	return -1;
}