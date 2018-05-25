#ifndef _EC_EVENT_HANDLER_H_
#define _EC_EVENT_HANDLER_H_

#include "txEngineEventHandler.h"

#include "CommonDefine.h"

class CoreEventHandler : public txEventHandlerBase
{
public:
	virtual void registerEditorCoreEventHandler() = 0;
	virtual void unregisterEditorCoreEventHandler() = 0;
	virtual void onEditorCoreEvent(const CORE_EVENT& type, const txVector<std::string>& params) = 0;
	virtual void notifyEvent(const int& event, const txVector<std::string>& paramList)
	{
		onEditorCoreEvent((CORE_EVENT)event, paramList);
	}
protected:
};

#endif