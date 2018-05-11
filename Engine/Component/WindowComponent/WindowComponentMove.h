#ifndef _WINDOW_COMPONENT_MOVE_H_
#define _WINDOW_COMPONENT_MOVE_H_

#include "txPoint.h"
#include "txComponentKeyFrame.h"

// 简单移动组件
class WindowComponentMove : public txComponentKeyFrame
{
public:
	WindowComponentMove(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name)
	{}
	virtual ~WindowComponentMove(){}
	virtual bool isType(const std::string& type)
	{
		return txComponentKeyFrame::isType(type) || type == TOSTRING(WindowComponentMove);
	}
	void setStartPosition(const txPoint& start) { mStartPosition = start; }
	void setTargetPosition(const txPoint& target) { mTargetPosition = target; }
	const txPoint& getStartPosition() { return mStartPosition; }
	const txPoint& getTargetPosition() { return mTargetPosition; }
protected:
	virtual void applyKeyFrame(const txVector<float>& value);
protected:
	txPoint mStartPosition;
	txPoint mTargetPosition;
};

#endif