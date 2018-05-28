#ifndef _CORE_EVENT_HANDLER_H_
#define _CORE_EVENT_HANDLER_H_

#include "txEngineEventHandler.h"

#include "CommonDefine.h"

class CoreEventHandler : public txEventHandlerBase
{
public:
	virtual void registerEditorCoreEvent() = 0;
	virtual void unregisterEditorCoreEvent() = 0;
	virtual void onEditorEvent(const CORE_EVENT& type, const txVector<std::string>& params) = 0;
	virtual void notifyEvent(const int& event, const txVector<std::string>& paramList)
	{
		onEditorEvent((CORE_EVENT)event, paramList);
	}
protected:
};

#endif