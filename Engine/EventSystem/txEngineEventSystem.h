#ifndef _TX_ENGINE_EVENT_SYSTEM_H_
#define _TX_ENGINE_EVENT_SYSTEM_H_

#include "txEventSystemBase.h"

// 尽量不要直接使用txEventSystem的pushEvent,应该使用txEngineRoot的sendEvent
class txEventHandler;
class txEngineEventSystem : public txEventSystemBase<ENGINE_EVENT, txEngineEventHandler>
{
public:
	txEngineEventSystem();
protected:
};

#endif
