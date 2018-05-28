#include "txComponentHeader.h"

#include "EditorCoreRoot.h"
#include "ComponentEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "ComponentHierarchy.h"

void CommandEditorFrameNotifyComponentSelectionChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	txVector<txComponent*>& selections = mComponentEditorCore->getComponentSelections();
	txVector<std::string> nameList;
	int selectItemCount = selections.size();
	FOR_STL(selections, int i = 0; i < selectItemCount; ++i)
	{
		nameList.push_back(selections[i]->getName());
	}
	END_FOR_STL(selections);
	mComponentHierarchy->setSelection(nameList);
	if (selections.size() > 0)
	{
		mPropertyWindow->showProperty(selections[0]);
	}
	else
	{
		mPropertyWindow->showProperty(NULL);
	}
}

std::string CommandEditorFrameNotifyComponentSelectionChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}