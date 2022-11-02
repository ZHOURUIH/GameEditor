#include "SCPacketOtherMove.h"

void SCPacketOtherMove::write(char* buffer, int bufferSize, int& index)
{
	base::write(buffer, bufferSize, index);
	Utility::write(buffer, bufferSize, index, mPlayerID);
	Utility::write(buffer, bufferSize, index, mPosX);
	Utility::write(buffer, bufferSize, index, mPosY);
}