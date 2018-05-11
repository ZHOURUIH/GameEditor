#ifndef _GAME_COMMAND_H_
#define _GAME_COMMAND_H_

#include "txCommand.h"

#include "ToolCoreBase.h"

class ToolCoreCommand : public ToolCoreBase, public txCommand
{
public:
	ToolCoreCommand(const char* file, int line, bool showInfo = true)
		:
		txCommand(file, line, showInfo)
	{}
};

#endif