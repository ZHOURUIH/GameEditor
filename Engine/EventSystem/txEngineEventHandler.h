#ifndef _TX_ENGINE_EVENT_HANDLER_H_
#define _TX_ENGINE_EVENT_HANDLER_H_

#include "txEngineDefine.h"

class txEventHandlerBase
{
public:
	virtual void notifyEvent(const int& event, const txVector<std::string>& paramList) = 0;
};

class txEngineEventHandler : public txEventHandlerBase
{
public:
	virtual void registerEngineEventHandler() = 0;
	virtual void unregisterEngineEventHandler() = 0;
	virtual void onEngineEvent(const ENGINE_EVENT& event, const txVector<std::string>& paramList) = 0;
	virtual void notifyEvent(const int& event, const txVector<std::string>& paramList)
	{
		onEngineEvent((ENGINE_EVENT)event, paramList);
	}
protected:
};

#endif