#ifndef _EDITOR_COMMAND_H_
#define _EDITOR_COMMAND_H_

#include "EditorCoreCommand.h"
#include "EditorBase.h"

class EditorCommand : public EditorCoreCommand, public EditorBase
{
public:
	EditorCommand(){}
};

#endif