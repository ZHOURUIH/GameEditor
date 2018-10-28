#include "SocketPacketFriction.h"
#include "txSerializer.h"
#include "Utility.h"

void SocketPacketFriction::setFrictionParam(char friction)
{
	// FE 03 1个字阻力值 机器号 校验码 FF
	txSerializer serializer;
	serializer.write((unsigned char)0xFE);
	serializer.write((unsigned char)0x03);
	serializer.write(friction);
	serializer.write((char)0);
	const char* buffer = serializer.getBuffer();
	int count1 = 0;
	for (int i = 1; i < 4; ++i)
	{
		count1 += BinaryUtility::crc_check(buffer[i]);
	}
	serializer.write((char)count1);
	serializer.write((unsigned char)0xFF);
	read((char*)serializer.getBuffer(), serializer.getDataSize());
}