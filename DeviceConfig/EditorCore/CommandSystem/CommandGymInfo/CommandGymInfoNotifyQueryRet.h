#ifndef _COMMAND_GYM_INFO_NOTIFY_QUERY_RET_H_
#define _COMMAND_GYM_INFO_NOTIFY_QUERY_RET_H_

#include "EditorCoreCommand.h"

class CommandGymInfoNotifyQueryRet : public EditorCoreCommand
{
public:
	COMMAND_EDITOR_CORE_CONSTRUCT(CommandGymInfoNotifyQueryRet)
	{}
	virtual void execute();
public:
	int mResult;
	bool mRegisted;
	std::string mGymName;
	std::string mRegisteDate;
};

#endif