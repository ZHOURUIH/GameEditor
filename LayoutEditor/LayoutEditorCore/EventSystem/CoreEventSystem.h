#ifndef _CORE_EVENT_SYSTEM_H_
#define _CORE_EVENT_SYSTEM_H_

#include "txEventSystemBase.h"

#include "CoreEventHandler.h"

class CoreEventHandler;
class CoreEventSystem : public txEventSystemBase<CORE_EVENT, CoreEventHandler>
{
public:
	CoreEventSystem();
protected:
};

#endif