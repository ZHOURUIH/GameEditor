#include "txAnimation.h"
#include "txAnimationNode.h"
#include "Utility.h"

txAnimation::~txAnimation()
{
	auto ite = mNodeAnimList.begin();
	auto iteEnd = mNodeAnimList.end();
	FOR_STL(mNodeAnimList, ; ite != iteEnd; ++ite)
	{
		TRACE_DELETE(ite->second);
	}
	END_FOR_STL(mNodeAnimList);
	mNodeAnimList.clear();
}

txAnimationNode* txAnimation::GetNodeAnim(const char* name)
{
	auto iter = mNodeAnimList.find(name);
	if (iter != mNodeAnimList.end())
	{
		return iter->second;
	}
	else
	{
		txAnimationNode* nodeAnim = TRACE_NEW(txAnimationNode, nodeAnim);
		mNodeAnimList.insert(name, nodeAnim);
		return nodeAnim;
	}
	return NULL;
}

void txAnimation::FinishCreateNodeAnim()
{ 
	auto iter = mNodeAnimList.begin();
	auto iterEnd = mNodeAnimList.end();
	FOR_STL(mNodeAnimList, ; iter != iterEnd; ++iter)
	{
		float maxKeyFrameTime = iter->second->getMaxKeyFrameTime();
		mLength = txMath::getMax(maxKeyFrameTime, mLength);
	}
	END_FOR_STL(mNodeAnimList);
}

void txAnimation::perform(txMap<std::string, AnimInfo>& animInfo, const float& nTime)
{
	auto ite = mNodeAnimList.begin();
	auto iteEnd = mNodeAnimList.end();
	FOR_STL(mNodeAnimList, ; ite != iteEnd; ++ite)
	{
		txAnimationNode* nodeAnim = ite->second;
		
		AnimInfo info;
		info.rot = nodeAnim->getInterpolateRotation((float)nTime);
		info.scale = nodeAnim->getInterpolateScale(nTime);
		info.pos = nodeAnim->getInterpolatePosition(nTime);

		auto iter = animInfo.find(ite->first);
		if (iter != animInfo.end())
		{
			iter->second = info;
		}
		else
		{
			animInfo.insert(ite->first, info);
		}
	}
	END_FOR_STL(mNodeAnimList);
}

void txAnimation::CopyAnimation(txAnimation* pAnimation)
{
	auto iter = mNodeAnimList.begin();
	auto iterEnd = mNodeAnimList.end();
	FOR_STL(mNodeAnimList, ; iter != iterEnd; ++iter)
	{
		txAnimationNode* pNodeAnim = TRACE_NEW(txAnimationNode, pNodeAnim);
		txAnimationNode* thisNodeAnim = iter->second;
		thisNodeAnim->CopyNodeAnim(pNodeAnim);
		pAnimation->mNodeAnimList.insert(iter->first, pNodeAnim);
	}
	END_FOR_STL(mNodeAnimList);
	pAnimation->mAnimationName = mAnimationName;
}