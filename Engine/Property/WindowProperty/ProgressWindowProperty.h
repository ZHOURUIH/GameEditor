#ifndef _PROGRESS_WINDOW_PROPERTY_H_
#define _PROGRESS_WINDOW_PROPERTY_H_

#include "WindowProperty.h"

WINDOW_PROPERTY(ProgressWindow)

//---------------------------------------------------------------------------------------------------------------------------------------------
// 进度
class ProgressWindowPropertyPercent : public ProgressWindowProperty<txFloatProperty, float>
{
public:
	ProgressWindowPropertyPercent();
	virtual void setRealValue(const float& value, ProgressWindow* window);
	virtual float getRealValue(ProgressWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 进度条是从左到右还是从右到左
class ProgressWindowPropertyLeftToRight : public ProgressWindowProperty<txBoolProperty, bool>
{
public:
	ProgressWindowPropertyLeftToRight();
	virtual void setRealValue(const bool& value, ProgressWindow* window);
	virtual bool getRealValue(ProgressWindow* window);
};

#endif