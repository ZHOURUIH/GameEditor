#ifndef _EDITOR_BASE_H_
#define _EDITOR_BASE_H_

#include "wxEditorHeader.h"

class EditorFrame;
class PrefabResource;
class LayoutResource;
class LoadedLayout;
class LayoutHierarchy;
class InnerChildHierarchy;
class PropertyWindow;
class WindowResource;
class RenderCanvas;
class WindowTemplate;
class KeyFrameGraph;
class KeyFrameResource;
class ComponentViewer;
class ComponentPropertyPanel;
class ActionTreeResource;
class ActionTreeGraph;
class ActionPropertyPanel;
class EditorBase
{
public:
	static void notifyConstructDone();
public:
	static EditorFrame* mEditorFrame;
	static PrefabResource* mPrefabResource;
	static LayoutResource* mLayoutResource;
	static LoadedLayout* mLoadedLayout;
	static LayoutHierarchy* mLayoutHierarchy;
	static InnerChildHierarchy* mInnerChildHierarchy;
	static PropertyWindow* mPropertyWindow;
	static WindowResource* mWindowResource;
	static RenderCanvas* mRenderCanvas;
	static WindowTemplate* mWindowTemplate;
	static KeyFrameGraph* mKeyFrameGraph;
	static KeyFrameResource* mKeyFrameResource;
	static ComponentViewer* mComponentViewer;
	static ComponentPropertyPanel* mComponentPropertyPanel;
	static ActionTreeResource* mActionTreeResource;
	static ActionTreeGraph* mActionTreeGraph;
	static ActionPropertyPanel* mActionPropertyPanel;
};

#endif