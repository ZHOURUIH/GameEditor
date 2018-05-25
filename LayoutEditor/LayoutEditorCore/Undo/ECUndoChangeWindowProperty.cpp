#include "Layout.h"
#include "WindowHeader.h"

#include "ECUndoChangeWindowProperty.h"
#include "LayoutEditorCore.h"
#include "CommandHeader.h"

void ECUndoChangeWindowProperty::undo()
{
	Layout* curLayout = mLayoutEditorCore->getCurLayout();
	if (curLayout != NULL)
	{
		Window* window = curLayout->getWindow(mWindowName);
		if (window != NULL)
		{
			CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
			cmdProperty->mPropertyName = mPropertyName;
			cmdProperty->mPropertyValue = mPropertyValue;
			cmdProperty->mOperator = this;
			mCommandSystem->pushCommand(cmdProperty, window);
		}
	}
}