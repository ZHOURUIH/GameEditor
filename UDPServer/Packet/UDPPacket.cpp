#include "UDPPacket.h"

void UDPPacket::read(char* buffer, int bufferSize, int& index)
{
	mType = Utility::read<PACKET_TYPE>(buffer, bufferSize, index);
}

void UDPPacket::write(char* buffer, int bufferSize, int& index)
{
	Utility::write<PACKET_TYPE>(buffer, bufferSize, index, mType);
}