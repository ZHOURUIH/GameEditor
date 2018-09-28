#ifndef _SERVER_BASE_H_
#define _SERVER_BASE_H_

#include "ServerDefine.h"

class ServerBase
{
public:
	static void notifyConstructDone();
	static void notifyComponentDeconstruct();
public:
};

#endif
