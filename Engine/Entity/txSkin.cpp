#include "txSkin.h"
#include "txSkinBone.h"
#include "txSceneManager.h"
#include "Utility.h"

txSkin::~txSkin()
{
	auto ite = mSBList.begin();
	auto iteEnd = mSBList.end();
	FOR_STL(mSBList, ; ite != iteEnd; ++ite)
	{
		txSkinBone* pBone = ite->second;
		TRACE_DELETE(pBone);
	}
	END_FOR_STL(mSBList);
	mSBList.clear();
}

void txSkin::setWeight(const char* boneName, const int& vertexIndex, const float& weight)
{
	auto iter = mSBList.find(boneName);
	if (iter == mSBList.end())
	{
		txSkinBone* sb = TRACE_NEW(txSkinBone, sb);
		if (sb == NULL)
		{
			return;
		}
		mSBList.insert(boneName, sb);
		sb->setWeight(vertexIndex, weight);
	}
	else
	{
		iter->second->setWeight(vertexIndex, weight);
	}
}

void txSkin::setOffsetMatrix(const char* boneName, const MATRIX4& offsetMat)
{
	auto iter = mSBList.find(boneName);
	if (iter == mSBList.end())
	{
		txSkinBone* sb = TRACE_NEW(txSkinBone, sb);
		if (sb == NULL)
		{
			return;
		}
		mSBList.insert(boneName, sb);
		sb->setOffsetMat(offsetMat);
	}
	else
	{
		iter->second->setOffsetMat(offsetMat);
	}
}

txSkinBone* txSkin::getSkinBone(const char* boneName)
{
	auto ite = mSBList.find(boneName);
	if (ite == mSBList.end())
	{
		return NULL;
	}
	return ite->second;
}

void txSkin::CopySkin(txSkin* pSkin)
{
	auto iter = mSBList.begin();
	auto iterEnd = mSBList.end();
	FOR_STL(mSBList, ; iter != iterEnd; ++iter)
	{
		txSkinBone* thisSkinBone = iter->second;
		txSkinBone* pSkinBone = TRACE_NEW(txSkinBone, pSkinBone);
		thisSkinBone->CopySkinBone(pSkinBone);
		pSkin->mSBList.insert(iter->first, pSkinBone);
	}
	END_FOR_STL(mSBList);
}