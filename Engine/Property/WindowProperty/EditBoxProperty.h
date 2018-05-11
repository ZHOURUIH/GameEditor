#ifndef _EDITBOX_PROPERTY_H_
#define _EDITBOX_PROPERTY_H_

#include "WindowProperty.h"

WINDOW_PROPERTY(EditBox);

//---------------------------------------------------------------------------------------------------------------------------------------------
// 是否显示光标
class EditBoxPropertyShowCaret : public EditBoxProperty<txBoolProperty, bool>
{
public:
	EditBoxPropertyShowCaret();
	virtual void setRealValue(const bool& value, EditBox* window);
	virtual bool getRealValue(EditBox* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 文本框最大字符长度
class EditBoxPropertyMaxTextLength : public EditBoxProperty<txIntProperty, int>
{
public:
	EditBoxPropertyMaxTextLength();
	virtual void setRealValue(const int& value, EditBox* window);
	virtual int getRealValue(EditBox* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 文本框的文字
class EditBoxPropertyEditText : public EditBoxProperty<txStringProperty, std::string>
{
public:
	EditBoxPropertyEditText();
	virtual void setRealValue(const std::string& value, EditBox* window);
	virtual std::string getRealValue(EditBox* window);
};

#endif