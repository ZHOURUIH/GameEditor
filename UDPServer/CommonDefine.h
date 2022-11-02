#pragma once

#include "CommonMacro.h"

enum class PACKET_TYPE : byte
{
	NONE,
	CS_INIT,
	CS_MOVE,
	SC_OTHER_MOVE,
};