#ifndef _TX_ANIMATION_H_
#define _TX_ANIMATION_H_

#include "txEngineDefine.h"

class txAnimationNode;

// 整个骨骼的动作
class txAnimation
{
public:
	txAnimation()
		:
		mLength(0.0f)
	{}
	virtual ~txAnimation();
	txAnimationNode* GetNodeAnim(const char* name);
	// 在动作关键帧都加载完以后,计算动作长度
	void FinishCreateNodeAnim();
	const float& getLength() { return mLength; }
	void SetAnimationName(const std::string& animationName) {mAnimationName =  animationName;}
	const std::string& GetAnimationName() { return mAnimationName;}
	void perform(txMap<std::string, AnimInfo>& animInfo, const float& nTime);
	// 拷贝动作的所有数据
	void CopyAnimation(txAnimation* pAnimation);
protected:
	txMap<std::string, txAnimationNode*> mNodeAnimList;
	std::string mAnimationName; // 此处的名字没有取动作文件中的名字,而是使用文件名作为动作名
	float mLength;
};

#endif