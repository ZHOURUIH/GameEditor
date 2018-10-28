#ifndef _TX_EVENT_HANDLER_H_
#define _TX_EVENT_HANDLER_H_

#include "CommonDefine.h"

class txEventHandlerBase
{
public:
	virtual void notifyEvent(const int& event, txVector<std::string>& paramList) = 0;
};

#endif