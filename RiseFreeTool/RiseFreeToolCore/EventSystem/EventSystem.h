#ifndef _EVENT_SYSTEM_H_
#define _EVENT_SYSTEM_H_

#include "txEventSystemBase.h"

#include "EventHandler.h"

class EventHandler;
class EventSystem : public txEventSystemBase<CORE_EVENT_TYPE, EventHandler>
{
public:
	EventSystem();
protected:
};

#endif