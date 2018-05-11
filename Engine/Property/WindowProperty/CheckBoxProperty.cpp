#include "CheckBoxProperty.h"

CheckBoxPropertyCheck::CheckBoxPropertyCheck()
{
	mPropertyName = "Check";
	mDescription = "设置勾选窗选中状态";
}
void CheckBoxPropertyCheck::setRealValue(const bool& value, CheckBox* window)
{
	window->setCkeck(value);
}
bool CheckBoxPropertyCheck::getRealValue(CheckBox* window)
{
	return window->isChecked();
}