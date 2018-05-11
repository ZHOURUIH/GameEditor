#include "txSkinBone.h"

void txSkinBone::CopySkinBone(txSkinBone* pSkinBone)
{
	pSkinBone->setOffsetMat(mOffsetMat);
	auto iter = mWeights.begin();
	auto iterEnd = mWeights.end();
	FOR_STL(mWeights, ; iter != iterEnd; ++iter)
	{
		pSkinBone->setWeight(iter->first, iter->second);
	}
	END_FOR_STL(mWeights);
}
