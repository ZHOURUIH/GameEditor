#include "txComponent.h"

#include "WindowHeader.h"

#include "ComponentEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "EditorCommandHeader.h"
#include "ComponentViewer.h"
#include "ComponentPropertyPanel.h"

void CommandEditorFrameNotifyComponentSelectionChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// 选择项改变时,需要显示选择项属性,并且要确定在树形列表窗口中选中该项
	txVector<txComponent*>& selections = mComponentEditorCore->getComponentSelections();
	if (selections.size() > 0)
	{
		mComponentPropertyPanel->showPropertyT(selections);
		txVector<std::string> selectionNameList;
		int selectionCount = selections.size();
		FOR_STL(selections, int i = 0; i < selectionCount; ++i)
		{
			selectionNameList.push_back(selections[i]->getName());
		}
		END_FOR_STL(selections);
		mComponentViewer->unselectAll();
		mComponentViewer->setSelection(selectionNameList);
	}
	else
	{
		mComponentPropertyPanel->showReceiverProperty(NULL);
		mComponentViewer->unselectAll();
	}
}

std::string CommandEditorFrameNotifyComponentSelectionChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}