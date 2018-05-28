#ifndef _COMMAND_EDITOR_SET_PROPERTY_H_
#define _COMMAND_EDITOR_SET_PROPERTY_H_

#include "EditorCoreCommand.h"

class CommandEditorSetProperty : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mComponentName = EMPTY_STRING;
		mPropertyName = EMPTY_STRING;
		mPropertyValue = EMPTY_STRING;
		mChangeOperator = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mComponentName;
	std::string mPropertyName;
	std::string mPropertyValue;
	void* mChangeOperator;
};

#endif