#include "Layout.h"
#include "WindowHeader.h"

#include "ECUndoChangeWindowComponentProperty.h"
#include "LayoutEditorCore.h"
#include "CommandHeader.h"

void ECUndoChangeWindowComponentProperty::undo()
{
	Layout* curLayout = mLayoutEditorCore->getCurLayout();
	if (curLayout != NULL)
	{
		Window* window = curLayout->getWindow(mWindowName);
		if (window != NULL)
		{
			CommandWindowSetComponentProperty* cmdProperty = NEW_CMD(cmdProperty);
			cmdProperty->mComponentName = mComponentName;
			cmdProperty->mPropertyName = mPropertyName;
			cmdProperty->mPropertyValue = mPropertyValue;
			cmdProperty->mOperator = this;
			mCommandSystem->pushCommand(cmdProperty, window);
		}
	}
}