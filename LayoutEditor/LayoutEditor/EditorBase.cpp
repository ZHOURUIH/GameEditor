#include "EditorBase.h"
#include "EditorApp.h"
#include "EditorFrame.h"

EditorFrame* EditorBase::mEditorFrame = NULL;
PrefabResource* EditorBase::mPrefabResource = NULL;
LayoutResource* EditorBase::mLayoutResource = NULL;
LoadedLayout* EditorBase::mLoadedLayout = NULL;
LayoutHierarchy* EditorBase::mLayoutHierarchy = NULL;
InnerChildHierarchy* EditorBase::mInnerChildHierarchy = NULL;
PropertyWindow* EditorBase::mPropertyWindow = NULL;
WindowResource* EditorBase::mWindowResource = NULL;
RenderCanvas* EditorBase::mRenderCanvas = NULL;
WindowTemplate* EditorBase::mWindowTemplate = NULL;
KeyFrameGraph* EditorBase::mKeyFrameGraph = NULL;
KeyFrameResource* EditorBase::mKeyFrameResource = NULL;
ComponentViewer* EditorBase::mComponentViewer = NULL;
ComponentPropertyPanel* EditorBase::mComponentPropertyPanel = NULL;
ActionTreeResource* EditorBase::mActionTreeResource = NULL;
ActionTreeGraph* EditorBase::mActionTreeGraph = NULL;
ActionPropertyPanel* EditorBase::mActionPropertyPanel = NULL;

void EditorBase::notifyConstructDone()
{
	if (mEditorFrame == NULL)
	{
		mEditorFrame = wxGetApp().GetMainFrame();
		mPrefabResource = mEditorFrame->getPrefabResource();
		mLayoutResource = mEditorFrame->getLayoutResource();
		mLoadedLayout = mEditorFrame->getLoadedLayout();
		mLayoutHierarchy = mEditorFrame->getLayoutHierarchy();
		mInnerChildHierarchy = mEditorFrame->getInnerChildHierarchy();
		mPropertyWindow = mEditorFrame->getPropertyWindow();
		mWindowResource = mEditorFrame->getWindowResource();
		mRenderCanvas = mEditorFrame->getRenderCanvas();
		mWindowTemplate = mEditorFrame->getWindowTemplate();
		mKeyFrameGraph = mEditorFrame->getKeyFrameGraph();
		mKeyFrameResource = mEditorFrame->getKeyFrameResource();
		mComponentViewer = mEditorFrame->getComponentViewer();
		mComponentPropertyPanel = mEditorFrame->getComponentPropertyPanel();
		mActionTreeResource = mEditorFrame->getActionTreeResource();
		mActionTreeGraph = mEditorFrame->getActionTreeGraph();
		mActionPropertyPanel = mEditorFrame->getActionPropertyPanel();
	}
}