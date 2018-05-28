#ifndef _COMMAND_ACTION_SET_PROPERTY_H_
#define _COMMAND_ACTION_SET_PROPERTY_H_

#include "EditorCoreCommand.h"

class CommandActionSetProperty : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mPropertyName = EMPTY_STRING;
		mPropertyValue = EMPTY_STRING;
		mOperator = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mPropertyName;
	std::string mPropertyValue;
	void* mOperator;
};

#endif