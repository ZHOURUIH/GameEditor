#ifndef _EVENT_SYSTEM_H_
#define _EVENT_SYSTEM_H_

#include "txEventSystemBase.h"

#include "CTEventHandler.h"

class CTEventHandler;
class EventSystem : public txEventSystemBase<CORE_EVENT_TYPE, CTEventHandler>
{
public:
	EventSystem();
protected:
};

#endif