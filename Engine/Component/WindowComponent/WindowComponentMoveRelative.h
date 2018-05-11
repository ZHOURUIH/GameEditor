#ifndef _WINDOW_COMPONENT_MOVE_RELATIVE_H_
#define _WINDOW_COMPONENT_MOVE_RELATIVE_H_

#include "txComponentKeyFrame.h"
#include "txPoint.h"

// 相对于一个点进行运动
class WindowComponentMoveRelative : public txComponentKeyFrame
{
public:
	WindowComponentMoveRelative(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name)
	{}
	virtual ~WindowComponentMoveRelative(){ destroy(); }
	void destroy(){}
	void setStartPos(const txPoint& startPos) { mStartPos = startPos; }
	virtual void applyKeyFrame(const txVector<float>& value);
	virtual bool isType(const std::string& type)
	{
		return txComponentKeyFrame::isType(type) || type == TOSTRING(WindowComponentMoveRelative);
	}
protected:
	txPoint mStartPos;	// 移动开始时的位置
};

#endif