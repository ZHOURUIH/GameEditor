#ifndef _COMMAND_GYM_INFO_NOTIFY_REGISTE_RET_H_
#define _COMMAND_GYM_INFO_NOTIFY_REGISTE_RET_H_

#include "EditorCoreCommand.h"

class CommandGymInfoNotifyRegisteRet : public EditorCoreCommand
{
public:
	COMMAND_EDITOR_CORE_CONSTRUCT(CommandGymInfoNotifyRegisteRet)
	{}
	virtual void execute();
public:
	bool mRegisteRet;
	std::string mErrorInfo;
	std::string mGymName;
	std::string mRegisteCode;
	std::string mRegisteDate;
};

#endif