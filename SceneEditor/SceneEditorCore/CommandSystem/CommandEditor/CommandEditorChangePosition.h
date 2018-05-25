#ifndef _COMMAND_EDITOR_CHANGE_POSITION_H_
#define _COMMAND_EDITOR_CHANGE_POSITION_H_

#include "EditorCoreCommand.h"

class CommandEditorChangePosition : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mPosition = txMath::VEC3_ZERO;
		mRefreshNow = false;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	VECTOR3 mPosition;
	bool mRefreshNow;
};

#endif