#include "FrameHeader.h"

void CmdGlobalDelayLambda::execute()
{
	if (mFunction != nullptr)
	{
		mFunction();
	}
}