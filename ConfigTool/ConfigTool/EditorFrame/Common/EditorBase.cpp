#include "EditorBase.h"
#include "EditorApp.h"

EditorFrame* EditorBase::mEditorFrame = NULL;

EditorBase::EditorBase()
{
	if (mEditorFrame == NULL)
	{
		mEditorFrame = wxGetApp().GetMainFrame();
	}
}