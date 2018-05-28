#include "EditorCoreRoot.h"
#include "SceneEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "SceneHierarchy.h"
#include "ComponentHierarchy.h"
#include "ECEditor.h"

void CommandEditorFrameNotifyEditorSelectionChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	txVector<ECEditor*>& selections = mSceneEditorCore->getEditorSelections();
	txVector<std::string> nameList;
	int selectItemCount = selections.size();
	FOR_STL(selections, int i = 0; i < selectItemCount; ++i)
	{
		nameList.push_back(selections[i]->getName());
	}
	END_FOR_STL(selections);
	mSceneHierarchy->setSelections(nameList);
	if (selections.size() > 0)
	{
		mComponentHierarchy->showComponent(selections[0]);
	}
	else
	{
		mComponentHierarchy->showComponent(NULL);
	}
}

std::string CommandEditorFrameNotifyEditorSelectionChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}