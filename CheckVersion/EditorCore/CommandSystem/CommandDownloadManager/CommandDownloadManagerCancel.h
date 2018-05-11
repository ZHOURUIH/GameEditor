#ifndef _COMMAND_DOWNLOAD_MANAGER_CANCEL_H_
#define _COMMAND_DOWNLOAD_MANAGER_CANCEL_H_

#include "ToolCoreCommand.h"

class CommandDownloadManagerCancel : public ToolCoreCommand
{
public:
	COMMAND_TOOL_CORE_CONSTRUCT(CommandDownloadManagerCancel){}
	COMMAND_DECLARE_FUNCTION;
};

#endif