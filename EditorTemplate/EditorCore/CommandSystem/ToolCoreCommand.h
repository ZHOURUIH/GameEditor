#ifndef _GAME_COMMAND_H_
#define _GAME_COMMAND_H_

#include "txCommand.h"

#include "EditorCoreBase.h"

class ToolCoreCommand : public EditorCoreBase, public txCommand
{
public:
	ToolCoreCommand(const char* file, int line, bool showInfo = true)
		:
		txCommand(file, line, showInfo)
	{}
};

#endif