#pragma once

#include "CommonMacro.h"

enum class PACKET_TYPE : byte
{
	NONE,
	CS_INIT,
	CS_MOVE,
	SC_OTHER_MOVE,
};

// »Øµ÷
class CustomThread;
typedef void (*CustomThreadCallback)(CustomThread* thread);
typedef void (*DelayCallback0)();

typedef function<void()> DelayLambda;