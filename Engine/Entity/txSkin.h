#ifndef _TX_SKIN_H_
#define _TX_SKIN_H_

#include "txEngineDefine.h"

class txSkinBone;
class txSkin
{
public:
	txSkin() {}
	virtual ~txSkin();
	void setWeight(const char* BoneName, const int& vertexIndex, const float& weight);
	void setOffsetMatrix(const char* boneName, const MATRIX4& offsetMat);
	txSkinBone* getSkinBone(const char* boneName);
	// 拷贝蒙皮的所有数据
	void CopySkin(txSkin* pSkin);
	txMap<std::string, txSkinBone*>& getSkinBoneList(){ return mSBList; }
private:
	txMap<std::string, txSkinBone*> mSBList;
};

#endif