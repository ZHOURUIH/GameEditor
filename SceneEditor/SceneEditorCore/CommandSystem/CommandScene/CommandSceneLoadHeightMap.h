#ifndef _COMMAND_SCENE_LOAD_HEIGHT_MAP_H_
#define _COMMAND_SCENE_LOAD_HEIGHT_MAP_H_

#include "EditorCoreCommand.h"

class CommandSceneLoadHeightMap : public EditorCoreCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif