#include "EditBoxProperty.h"

EditBoxPropertyShowCaret::EditBoxPropertyShowCaret()
{
	mPropertyName = "ShowCaret";
	mDescription = "设置编辑窗是否显示光标";
}
void EditBoxPropertyShowCaret::setRealValue(const bool& value, EditBox* window)
{
	window->setShowCaret(value);
}
bool EditBoxPropertyShowCaret::getRealValue(EditBox* window)
{
	return window->getShowCaret();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
EditBoxPropertyMaxTextLength::EditBoxPropertyMaxTextLength()
{
	mPropertyName = "MaxTextLength";
	mDescription = "设置编辑窗口文本框最大字符长度";
}
void EditBoxPropertyMaxTextLength::setRealValue(const int& value, EditBox* window)
{
	window->setMaxTextLength(value);
}
int EditBoxPropertyMaxTextLength::getRealValue(EditBox* window)
{
	return window->getMaxTextLength();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
EditBoxPropertyEditText::EditBoxPropertyEditText()
{
	mPropertyName = "EditText";
	mDescription = "文本框的文字";
}
void EditBoxPropertyEditText::setRealValue(const std::string& value, EditBox* window)
{
	window->setText(value, mAsync);
}
std::string EditBoxPropertyEditText::getRealValue(EditBox* window)
{
	return window->getText();
}