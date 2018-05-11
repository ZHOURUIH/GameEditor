#ifndef _COMMAND_DOWNLOAD_MANAGER_CANCEL_H_
#define _COMMAND_DOWNLOAD_MANAGER_CANCEL_H_

#include "EditorCoreCommand.h"

class CommandDownloadManagerCancel : public EditorCoreCommand
{
public:
	COMMAND_EDITOR_CORE_CONSTRUCT(CommandDownloadManagerCancel){}
	COMMAND_DECLARE_FUNCTION;
};

#endif