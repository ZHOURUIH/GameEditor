#ifndef _EDITOR_BASE_H_
#define _EDITOR_BASE_H_

#include "EditorHeader.h"

class EditorFrame;
class ComponentHierarchy;
class ComponentResource;
class PropertyWindow;
class RenderCanvas;
class SceneHierarchy;
class EditorBase
{
public:
	static void notifyConstructDone();
protected:
	static EditorFrame* mEditorFrame;
	static ComponentHierarchy* mComponentHierarchy;
	static ComponentResource* mComponentResource;
	static PropertyWindow* mPropertyWindow;
	static RenderCanvas* mRenderCanvas;
	static SceneHierarchy* mSceneHierarchy;
};

#endif