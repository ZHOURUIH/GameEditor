#include "NumberWindowProperty.h"

NumberWindowPropertyNumber::NumberWindowPropertyNumber()
{
	mPropertyName = "Number";
	mDescription = "设置数字窗口的数字";
}
void NumberWindowPropertyNumber::setRealValue(const std::string& value, NumberWindow* window)
{
	window->setNumber(value);
}
std::string NumberWindowPropertyNumber::getRealValue(NumberWindow* window)
{
	return window->getNumber();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
txVector<std::string> NumberWindowPropertyNumberStyle::mChoices;
NumberWindowPropertyNumberStyle::NumberWindowPropertyNumberStyle()
{
	mPropertyName = "NumberStyle";
	mDescription = "设置数字窗口数字风格";
}
void NumberWindowPropertyNumberStyle::setRealValue(const std::string& value, NumberWindow* window)
{
	window->setNumberStyle(value, mAsync);
}
std::string NumberWindowPropertyNumberStyle::getRealValue(NumberWindow* window)
{
	return window->getNumberStyle();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
NumberWindowPropertyNumberInterval::NumberWindowPropertyNumberInterval()
{
	mPropertyName = "NumberInterval";
	mDescription = "设置数字窗口数字间距, 其中的相对值是相对于窗口最终高度";
}
void NumberWindowPropertyNumberInterval::setRealValue(const txDim& value, NumberWindow* window)
{
	window->setNumberInterval(value);
}
txDim NumberWindowPropertyNumberInterval::getRealValue(NumberWindow* window)
{
	return window->getNumberInterval();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
NumberWindowPropertyMaxNumberCount::NumberWindowPropertyMaxNumberCount()
{
	mPropertyName = "MaxNumberCount";
	mDescription = "设置数字窗口数字最大个数";
}
void NumberWindowPropertyMaxNumberCount::setRealValue(const int& value, NumberWindow* window)
{
	window->setMaxNumberCount(value);
}
int NumberWindowPropertyMaxNumberCount::getRealValue(NumberWindow* window)
{
	return window->getMaxNumberCount();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
NumberWindowPropertyNumberDockingPosition::NumberWindowPropertyNumberDockingPosition()
{
	mPropertyName = "NumberDockingPosition";
	mDescription = "设置数字窗口数字停靠位置";
	ADD_ENUM(WDP_LEFT);
	ADD_ENUM(WDP_RIGHT);
	ADD_ENUM(WDP_CENTER);
}
void NumberWindowPropertyNumberDockingPosition::setRealValue(const WINDOW_DOCKING_POSITION& value, NumberWindow* window)
{
	window->setDockingPosition(value);
}
WINDOW_DOCKING_POSITION NumberWindowPropertyNumberDockingPosition::getRealValue(NumberWindow* window)
{
	return window->getDockingPosition();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
NumberWindowPropertyRotateWithWindow::NumberWindowPropertyRotateWithWindow()
{
	mPropertyName = "RotateWithWindow";
	mDescription = "数字是否随窗口一起旋转";
}
void NumberWindowPropertyRotateWithWindow::setRealValue(const bool& value, NumberWindow* window)
{
	window->setRotateWithWindow(value);
}
bool NumberWindowPropertyRotateWithWindow::getRealValue(NumberWindow* window)
{
	return window->getRotateWithWindow();
}