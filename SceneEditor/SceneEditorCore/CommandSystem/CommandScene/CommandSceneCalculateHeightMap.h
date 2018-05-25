#ifndef _COMMAND_SCENE_CALCULATE_HEIGHT_MAP_H_
#define _COMMAND_SCENE_CALCULATE_HEIGHT_MAP_H_

#include "EditorCoreCommand.h"

class CommandSceneCalculateHeightMap : public EditorCoreCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif