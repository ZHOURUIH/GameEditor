#ifndef _TX_EVENT_HANDLER_H_
#define _TX_EVENT_HANDLER_H_

#include <string>
#include <vector>

class txEventHandlerBase
{
public:
	virtual void notifyEvent(const int& event, std::vector<std::string>& paramList) = 0;
};

#endif