#include "txAnimationNode.h"
#include "txMathUtility.h"

void txAnimationNode::ClearAll()
{
	mPosKeyFrameList.clear();
	mPosTime.clear();
	mSclKeyFrameList.clear();
	mSclTime.clear();
	mRotKeyFrameList.clear();
	mRotTime.clear();
}

VECTOR3 txAnimationNode::getInterpolatePosition(const float& time)
{
	int index1 = 0, index2 = 0;
	getKeyFrameTimeBetween(mPosTime, time, index1, index2);
	VECTOR3 pos;
	if (index1 < 0)
	{
		if (txMath::isFloatZero(mPosTime[0]))
		{
			pos = mPosKeyFrameList[0];
		}
		else
		{
			float coef = time / mPosTime[0];
			pos = coef * mPosKeyFrameList[0];
		}
	}
	else if (index2 >= (int)mPosTime.size())
	{
		pos = mPosKeyFrameList[index1];
	}
	else
	{
		float coef = (time - mPosTime[index1]) / (mPosTime[index2] - mPosTime[index1]);
		pos = mPosKeyFrameList[index1] + coef * (mPosKeyFrameList[index2] - mPosKeyFrameList[index1]);
	}
	return pos;
}

VECTOR3 txAnimationNode::getInterpolateScale(const float& time)
{
	int index1 = 0, index2 = 0;
	getKeyFrameTimeBetween(mSclTime, time, index1, index2);
	VECTOR3 scale;
	if (index1 < 0)
	{
		if (txMath::isFloatZero(mPosTime[0]))
		{
			scale = mSclKeyFrameList[0];
		}
		else
		{
			float coef = time / mSclTime[0];
			scale = VECTOR3(1.0, 1.0, 1.0) + coef * (mSclKeyFrameList[0] - VECTOR3(1.0, 1.0, 1.0));
		}
	}
	else if (index2 >= (int)mSclTime.size())
	{
		scale = mSclKeyFrameList[index1];
	}
	else
	{
		float coef = (time - mSclTime[index1]) / (mSclTime[index2] - mSclTime[index1]);
		scale = mSclKeyFrameList[index1] + coef * (mSclKeyFrameList[index2] - mSclKeyFrameList[index1]);
	}
	return scale;
}

QUATERNION txAnimationNode::getInterpolateRotation(const float& time)
{
	int index1 = 0, index2 = 0;
	getKeyFrameTimeBetween(mRotTime, time, index1, index2);
	QUATERNION rot;
	if (index1 < 0)
	{
		if (txMath::isFloatZero(mPosTime[0]))
		{
			rot = mRotKeyFrameList[0];
		}
		else
		{
			float coef = time / mRotTime[0];
			rot = txMath::slerp(QUATERNION(1.0f, 0.0f, 0.0f, 0.0f), mRotKeyFrameList[0], coef);
		}
	}
	else if (index2 >= (int)mRotTime.size())
	{
		rot = mRotKeyFrameList[index1];
	}
	else
	{
		float coef = (time - mRotTime[index1]) / (mRotTime[index2] - mRotTime[index1]);
		rot = txMath::slerp(mRotKeyFrameList[index1], mRotKeyFrameList[index2], coef);
	}
	return rot;
}

void txAnimationNode::getKeyFrameTimeBetween(const txVector<float>& timeList, const float& time, int& preKeyFrame, int& nextKeyFrame)
{
	int nSize = timeList.size();
	// 如果小于第0帧,则返回第0帧
	float fTime = timeList[0];
	if (time <= fTime)
	{
		preKeyFrame = -1;
		nextKeyFrame = 0;
		return;
	}

	// 如果大于最后一帧,则返回最后一帧
	fTime = timeList[nSize - 1];
	if (time >= fTime)
	{
		preKeyFrame = nSize - 1;
		nextKeyFrame = nSize;
		return;
	}

	// 使用折半查找,找到时间位于哪两帧之间
	int nLeft = 0;
	int nRight = nSize - 1;
	while (nLeft < nRight)
	{
		int nIndex = (nLeft + nRight) / 2;
		float fTime1 = timeList[nIndex];
		float fTime2 = timeList[nIndex + 1];
		if (time >= fTime1 && time <= fTime2)
		{
			preKeyFrame = nIndex;
			nextKeyFrame = nIndex + 1;
			return;
		}
		else if (fTime1 > time)
		{
			nRight = nIndex;
		}
		else if (fTime2 < time)
		{
			nLeft = nIndex;
		}
	}
}

float txAnimationNode::getMaxKeyFrameTime()
{
	float maxTime = 0.0f;
	int posTimeCount = mPosTime.size();
	FOR_STL(mPosTime, int i = 0; i < posTimeCount; ++i)
	{
		if (mPosTime[i] > maxTime)
		{
			maxTime = mPosTime[i];
		}
	}
	END_FOR_STL(mPosTime);
	int sclTimeCount = mSclTime.size();
	FOR_STL(mSclTime, int i = 0; i < sclTimeCount; ++i)
	{
		if (mSclTime[i] > maxTime)
		{
			maxTime = mSclTime[i];
		}
	}
	END_FOR_STL(mSclTime);
	int rotTimeCount = mRotTime.size();
	FOR_STL(mRotTime, int i = 0; i < rotTimeCount; ++i)
	{
		if (mRotTime[i] > maxTime)
		{
			maxTime = mRotTime[i];
		}
	}
	END_FOR_STL(mRotTime);
	return maxTime;
}

void txAnimationNode::CopyNodeAnim(txAnimationNode* pNodeAnim)
{
	int posKeySize = mPosKeyFrameList.size();
	int posTimeSize = mPosTime.size();
	int sclKeySize = mSclKeyFrameList.size();
	int sclTimeSize = mSclTime.size();
	int rotKeySize = mRotKeyFrameList.size();
	int rotTimeSize = mRotTime.size();

	pNodeAnim->mPosKeyFrameList.resize(posKeySize);
	pNodeAnim->mPosTime.resize(posTimeSize);
	pNodeAnim->mSclKeyFrameList.resize(sclKeySize);
	pNodeAnim->mSclTime.resize(sclTimeSize);
	pNodeAnim->mRotKeyFrameList.resize(rotKeySize);
	pNodeAnim->mRotTime.resize(rotTimeSize);

	int i = 0;
	FOR_STL(mPosKeyFrameList, i = 0; i < posKeySize; ++i)
	{
		pNodeAnim->mPosKeyFrameList[i] = mPosKeyFrameList[i];
	}
	END_FOR_STL(mPosKeyFrameList);
	FOR_STL(mPosTime, i = 0; i < posTimeSize; ++i)
	{
		pNodeAnim->mPosTime[i] = mPosTime[i];
	}
	END_FOR_STL(mPosTime);
	FOR_STL(mSclKeyFrameList, i = 0; i < sclKeySize; ++i)
	{
		pNodeAnim->mSclKeyFrameList[i] = mSclKeyFrameList[i];
	}
	END_FOR_STL(mSclKeyFrameList);
	FOR_STL(mSclTime, i = 0; i < sclTimeSize; ++i)
	{
		pNodeAnim->mSclTime[i] = mSclTime[i];
	}
	END_FOR_STL(mSclTime);
	FOR_STL(mRotKeyFrameList, i = 0; i < rotKeySize; ++i)
	{
		pNodeAnim->mRotKeyFrameList[i] = mRotKeyFrameList[i];
	}
	END_FOR_STL(mRotKeyFrameList);
	FOR_STL(mRotTime, i = 0; i < rotTimeSize; ++i)
	{
		pNodeAnim->mRotTime[i] = mRotTime[i];
	}
	END_FOR_STL(mRotTime);
}