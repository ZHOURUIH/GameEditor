#ifndef _TX_COMPONENT_MOVE_H_
#define _TX_COMPONENT_MOVE_H_

#include "txComponentKeyFrame.h"

// 从某处移动到某处的简单移动组件
class txComponentMove : public txComponentKeyFrame
{
public:
	txComponentMove(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name)
	{}
	virtual ~txComponentMove(){}
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentMove); }
protected:
	virtual void applyKeyFrame(const txVector<float>& value);
protected:
	VECTOR3 mStartPosition;
	VECTOR3 mTargetPosition;
};

#endif