#include "EditorBase.h"
#include "EditorApp.h"
#include "EditorFrame.h"

EditorFrame* EditorBase::mEditorFrame = NULL;
SceneHierarchy* EditorBase::mSceneHierarchy = NULL;
ComponentHierarchy* EditorBase::mComponentHierarchy = NULL;
ComponentResource* EditorBase::mComponentResource = NULL;
PropertyWindow* EditorBase::mPropertyWindow = NULL;
RenderCanvas* EditorBase::mRenderCanvas = NULL;

void EditorBase::notifyConstructDone()
{
	if (mEditorFrame == NULL)
	{
		mEditorFrame = wxGetApp().GetMainFrame();
		mComponentHierarchy = mEditorFrame->getComponentHierarchy();
		mComponentResource = mEditorFrame->getComponentResource();
		mPropertyWindow = mEditorFrame->getPropertyWindow();
		mRenderCanvas = mEditorFrame->getRenderCanvas();
		mSceneHierarchy = mEditorFrame->getSceneHierarchy();
	}
}