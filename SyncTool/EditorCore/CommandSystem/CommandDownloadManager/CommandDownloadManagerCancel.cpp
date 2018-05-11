#include "CommandHeader.h"
#include "DownloadManager.h"
#include "EditorCore.h"

void CommandDownloadManagerCancel::execute()
{
	DownloadManager* downloadManager = static_cast<DownloadManager*>(mReceiver);
	mEditorCore->sendEvent(CET_CANCEL_UPDATE);
}

std::string CommandDownloadManagerCancel::showDebugInfo()
{
	return TOSTRING(CommandDownloadManagerCancel);
}