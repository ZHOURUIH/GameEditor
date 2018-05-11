#include "ConfigToolCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "DeviceListPanel.h"

void CommandEditorFrameNotifyDeviceChanged::execute()
{
	EditorFrame* editorFrame = static_cast<EditorFrame*>(mReceiver);
	editorFrame->getDeviceListPanel()->refreshDeviceList(mConfigToolCore->getDeviceList());
}

std::string CommandEditorFrameNotifyDeviceChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}