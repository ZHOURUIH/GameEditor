#include "EditorCoreUtility.h"
#include "EditorCore.h"

void EditorCoreUtility::logError(const std::string& info)
{
	EditorCore::getSingletonPtr()->logError(info);
}

void EditorCoreUtility::logInfo(const std::string& info)
{
	EditorCore::getSingletonPtr()->logInfo(info);
}

std::string EditorCoreUtility::charToHex(unsigned char byte)
{
	char byteHex[3] = {0};
	char charPool[] = { 'A', 'B', 'C', 'D', 'E', 'F' };
	unsigned char highBit = byte >> 4;
	// 高字节的十六进制
	if (highBit < (unsigned char)10)
	{
		byteHex[0] = '0' + highBit;
	}
	else
	{
		byteHex[0] = charPool[highBit - 10];
	}
	// 低字节的十六进制
	unsigned char lowBit = byte & 0x0F;
	if (lowBit < (unsigned char)10)
	{
		byteHex[1] = '0' + lowBit;
	}
	else
	{
		byteHex[1] = charPool[lowBit - 10];
	}
	return byteHex;
}

std::string EditorCoreUtility::charArrayToHexString(unsigned char* data, const int& dataCount)
{
	int showCount = dataCount * 3 + 1;
	char* byteData = TRACE_NEW_ARRAY(char, showCount, byteData);
	memset(byteData, 0, showCount);
	for (int j = 0; j < dataCount; ++j)
	{
		unsigned char curByte = data[j];
		std::string byteStr = charToHex(curByte);
		byteData[j * 3 + 0] = byteStr[0];
		byteData[j * 3 + 1] = byteStr[1];
		byteData[j * 3 + 2] = ' ';
	}
	std::string str(byteData);
	TRACE_DELETE_ARRAY(byteData);
	return str;
}