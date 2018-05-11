#ifndef _EC_EVENT_HANDLER_H_
#define _EC_EVENT_HANDLER_H_

#include <vector>
#include <string>

#include "txEventHandler.h"

#include "CommonDefine.h"

class CTEventHandler : public txEventHandlerBase
{
public:
	CTEventHandler(){}
	virtual ~CTEventHandler(){}
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