#include "WindowActionProperty.h"
#include "ActionWindow.h"
#include "txAction.h"
#include "txActionTree.h"
#include "Layout.h"

ActionWindowPropertyActionWindow::ActionWindowPropertyActionWindow()
{
	mPropertyName = "ActionWindow";
	mDescription = "行为所挂接的窗口";
}
void ActionWindowPropertyActionWindow::setRealValue(const std::string& value, ActionWindow* action)
{
	Layout* layout = (Layout*)action->getActionTree()->getUserData();
	if (layout != NULL)
	{
		action->setWindow(layout->getWindow(value));
	}
}
std::string ActionWindowPropertyActionWindow::getRealValue(ActionWindow* action)
{
	if (action->getWindow() != NULL)
	{
		return action->getWindow()->getName();
	}
	return EMPTY_STRING;
}
//---------------------------------------------------------------------------------------------------------
ActionWindowPropertyExecuteTime::ActionWindowPropertyExecuteTime()
{
	mPropertyName = "ExecuteTime";
	mDescription = "行为执行时间";
}
void ActionWindowPropertyExecuteTime::setRealValue(const float& value, ActionWindow* action)
{
	action->setExecuteTime(value);
}
float ActionWindowPropertyExecuteTime::getRealValue(ActionWindow* action)
{
	return action->getExecuteTime();
}