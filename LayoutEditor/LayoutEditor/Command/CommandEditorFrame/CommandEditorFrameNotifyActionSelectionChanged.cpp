#include "txComponent.h"
#include "txAction.h"

#include "WindowHeader.h"

#include "ActionTreeEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "EditorCommandHeader.h"
#include "ComponentViewer.h"
#include "ActionPropertyPanel.h"
#include "ActionTreeResource.h"

void CommandEditorFrameNotifyActionSelectionChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// 选择项改变时,需要显示选择项属性,并且要确定在树形列表窗口中选中该项
	txAction* action = mActionTreeEditorCore->getCurAction();
	mActionPropertyPanel->showReceiverProperty(action);
	mActionTreeResource->notifyActionSelectionChanged();
}

std::string CommandEditorFrameNotifyActionSelectionChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}