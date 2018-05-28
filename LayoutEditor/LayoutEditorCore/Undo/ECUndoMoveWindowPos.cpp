#include "ECUndoMoveWindowPos.h"
#include "LayoutEditorCore.h"

#include "Layout.h"
#include "WindowHeader.h"

void ECUndoMoveWindowPos::undo()
{
	Layout* curLayout = mLayoutEditorCore->getCurLayout();
	if (NULL == curLayout)
	{
		return;
	}
	Window* window = curLayout->getWindow(mWindowName);
	if (NULL != window)
	{
		mLayoutEditorCore->moveWindowPos(window, mOldPos);
	}
}