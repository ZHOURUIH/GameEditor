#include "ActionWindow.h"
#include "WindowActionProperty.h"

void ActionWindow::initProperty()
{
	txAction::initProperty();
	addProperty<ActionWindowPropertyActionWindow>();
	addProperty<ActionWindowPropertyExecuteTime>();
}
void ActionWindow::setProperty(const std::string& propertyName, const std::string& propertyValue)
{
	txProperty* property = getProperty(propertyName);
	if (property == NULL)
	{
		return;
	}
	property->set(propertyValue, this);
}