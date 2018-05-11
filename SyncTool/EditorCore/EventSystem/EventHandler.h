#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include "txEventHandler.h"

#include "CommonDefine.h"

class EventHandler : public txEventHandlerBase
{
public:
	EventHandler(){}
	virtual ~EventHandler(){}
	virtual void registerEditorCoreEventHandler() = 0;
	virtual void unregisterEditorCoreEventHandler() = 0;
	virtual void onEditorCoreEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params) = 0;
	virtual void notifyEvent(const int& event, const txVector<std::string>& paramList)
	{
		onEditorCoreEvent((CORE_EVENT_TYPE)event, paramList);
	}
protected:
};

#endif