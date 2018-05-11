#include "EditorBase.h"
#include "EditorApp.h"

EditorFrame* EditorBase::mEditorFrame = NULL;

void EditorBase::notifyConstructDone()
{
	if (mEditorFrame == NULL)
	{
		mEditorFrame = wxGetApp().GetMainFrame();
	}
}