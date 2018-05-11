#ifndef _EC_EVENT_SYSTEM_H_
#define _EC_EVENT_SYSTEM_H_

#include "txEventSystemBase.h"

#include "CTEventHandler.h"

class CTEventHandler;
class CTEventSystem : public txEventSystemBase<CORE_EVENT_TYPE, CTEventHandler>
{
public:
	CTEventSystem();
protected:
};

#endif