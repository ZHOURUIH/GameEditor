#include "ECUndoLayoutVisible.h"
#include "LayoutEditorCore.h"

void ECUndoLayoutVisible::undo()
{
	if (mLayout != NULL)
	{
		mLayoutEditorCore->setLayoutVisible(mLayout, mVisible);
	}
}