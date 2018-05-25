#ifndef _COMMAND_SCENE_SHOW_HEIGHT_MAP_H_
#define _COMMAND_SCENE_SHOW_HEIGHT_MAP_H_

#include "EditorCoreCommand.h"

class CommandSceneShowHeightMap : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mShow = true;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mShow;
};

#endif