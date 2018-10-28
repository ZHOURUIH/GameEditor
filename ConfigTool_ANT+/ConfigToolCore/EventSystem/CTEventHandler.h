#ifndef _EC_EVENT_HANDLER_H_
#define _EC_EVENT_HANDLER_H_

#include "txEventHandler.h"

#include "CommonDefine.h"

class CTEventHandler : public txEventHandlerBase
{
public:
	CTEventHandler(){}
	virtual ~CTEventHandler(){}
	virtual void registerEditorCoreEventHandler() = 0;
	virtual void unregisterEditorCoreEventHandler() = 0;
	virtual void onEditorCoreEvent(const CORE_EVENT& type, txVector<std::string>& params) = 0;
	virtual void notifyEvent(const int& event, txVector<std::string>& paramList)
	{
		onEditorCoreEvent((CORE_EVENT)event, paramList);
	}
protected:
};

#endif