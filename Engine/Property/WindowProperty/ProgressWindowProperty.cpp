#include "ProgressWindowProperty.h"

ProgressWindowPropertyPercent::ProgressWindowPropertyPercent()
{
	mPropertyName = "Percent";
	mDescription = "设置进度条窗口进度";
}
void ProgressWindowPropertyPercent::setRealValue(const float& value, ProgressWindow* window)
{
	window->setProgress(value);
}
float ProgressWindowPropertyPercent::getRealValue(ProgressWindow* window)
{
	return window->getProgress();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
ProgressWindowPropertyLeftToRight::ProgressWindowPropertyLeftToRight()
{
	mPropertyName = "LeftToRight";
	mDescription = "进度条是从左到右还是从右到左";
}
void ProgressWindowPropertyLeftToRight::setRealValue(const bool& value, ProgressWindow* window)
{
	window->setLeftToRight(value);
}
bool ProgressWindowPropertyLeftToRight::getRealValue(ProgressWindow* window)
{
	return window->getLeftToRight();
}