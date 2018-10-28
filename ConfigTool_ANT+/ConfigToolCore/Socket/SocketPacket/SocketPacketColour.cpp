#include "SocketPacketColour.h"
#include "txSerializer.h"
#include "Utility.h"

void SocketPacketColour::setColour(char r, char g, char b)
{
	// FE 06 RGB »úÆ÷ºÅ Ð£ÑéÂë FF
	txSerializer serializer;
	serializer.write((unsigned char)0xFE);
	serializer.write((unsigned char)0x06);
	serializer.write(r);
	serializer.write(g);
	serializer.write(b);
	serializer.write((char)0);
	const char* buffer = serializer.getBuffer();
	int count1 = 0;
	for (int i = 1; i < 6; ++i)
	{
		count1 += BinaryUtility::crc_check(buffer[i]);
	}
	serializer.write((char)count1);
	serializer.write((unsigned char)0xFF);
	read((char*)serializer.getBuffer(), serializer.getDataSize());
}