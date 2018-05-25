#include "Layout.h"
#include "WindowHeader.h"

#include "ECUndoWindowRename.h"
#include "LayoutEditorCore.h"
#include "CommandHeader.h"

void ECUndoWindowRename::undo()
{
	Layout* curLayout = mLayoutEditorCore->getCurLayout();
	if (curLayout != NULL)
	{
		Window* window = curLayout->getWindow(mWindowName);
		if (window != NULL)
		{
			CommandWindowRename* cmdRename = NEW_CMD(cmdRename);
			cmdRename->mNewName = mName;
			mCommandSystem->pushCommand(cmdRename, window);
		}
	}
}