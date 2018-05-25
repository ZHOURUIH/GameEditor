#ifndef _COMMAND_SCENE_SAVE_HEIGHT_MAP_H_
#define _COMMAND_SCENE_SAVE_HEIGHT_MAP_H_

#include "EditorCoreCommand.h"

class CommandSceneSaveHeightMap : public EditorCoreCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif