#ifndef _TX_SKIN_BONE_H_
#define _TX_SKIN_BONE_H_

#include "txEngineDefine.h"

class txSkinBone
{
public:
	txSkinBone() {}
	virtual ~txSkinBone() {}
	void setWeight(const int& vertexIndex, const float& weight)
	{
		auto iter = mWeights.find(vertexIndex);
		if (iter != mWeights.end())
		{
			iter->second = weight;
		}
		else
		{
			mWeights.insert(vertexIndex, weight);
		}
	}
	const MATRIX4& getOffsetMat(){ return mOffsetMat; }
	void setOffsetMat(const MATRIX4& offsetMat){ mOffsetMat = offsetMat; }
	// 拷贝所有数据
	void CopySkinBone(txSkinBone* pSkinBone);
	txMap<int, float>& getWeightList() { return mWeights; }
protected:
	MATRIX4 mOffsetMat;
	txMap<int, float> mWeights;
};

#endif