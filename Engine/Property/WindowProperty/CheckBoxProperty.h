#ifndef _CHECKBOX_PROPERTY_H_
#define _CHECKBOX_PROPERTY_H_

#include "WindowProperty.h"

WINDOW_PROPERTY(CheckBox);

//---------------------------------------------------------------------------------------------------------------------------------------------
// 选中状态
class CheckBoxPropertyCheck : public CheckBoxProperty<txBoolProperty, bool>
{
public:
	CheckBoxPropertyCheck();
	virtual void setRealValue(const bool& value, CheckBox* window);
	virtual bool getRealValue(CheckBox* window);
};

#endif