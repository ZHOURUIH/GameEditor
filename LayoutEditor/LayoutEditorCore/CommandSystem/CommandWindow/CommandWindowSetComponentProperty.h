#ifndef _COMMAND_WINDOW_SET_COMPONENT_PROPERTY_H_
#define _COMMAND_WINDOW_SET_COMPONENT_PROPERTY_H_

#include "EditorCoreCommand.h"

class CommandWindowSetComponentProperty : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mComponentName = EMPTY_STRING;
		mPropertyName = EMPTY_STRING;
		mPropertyValue = EMPTY_STRING;
		mOperator = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mComponentName;
	std::string mPropertyName;
	std::string mPropertyValue;
	void* mOperator;
};

#endif