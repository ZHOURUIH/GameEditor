#include "FrameHeader.h"

void CmdGlobalDelayCall0::execute()
{
	if (mFunction != nullptr)
	{
		mFunction();
	}
}