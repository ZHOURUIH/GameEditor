#ifndef _TX_BINARY_UTILITY_H_
#define _TX_BINARY_UTILITY_H_

#include "txEngineDefine.h"

class txBinaryUtility
{
public:
	template<typename T>
	static void inverseByte(T& value)
	{
		int typeSize = sizeof(T);
		for (int i = 0; i < typeSize / 2; ++i)
		{
			swapByte(value, i, typeSize - i - 1);
		}
	}
	template<typename T>
	static void swapByte(T& value, const int& pos0, const int& pos1)
	{
		char byte0 = (value & (0xff << (8 * pos0))) >> (8 * pos0);
		char byte1 = (value & (0xff << (8 * pos1))) >> (8 * pos1);
		SET_BYTE(value, byte0, pos1);
		SET_BYTE(value, byte1, pos0);
	}

	// 首尾交换一段内存,width是内存方阵的宽度,每排的内存块数量,height是字节高度,blockByte是每块内存的字节数
	static void swapPixelBuffer(char* buffer, int width, int height, int blockByte);
	static void swapColourByte(char* buffer, int width, int height, int blockByte, int exchangeIndex0, int exchangeIndex1);
	static void upsideDownBuffer(char* buffer, int bufferWidth, int startX, int startY, int width, int height, int blockByte);
	// YUV转RGBA,范围都是0-255
	static void YUVToRGBA(char* YUVData, int width, int height, char* RGBAData);
	// RGNA转YUV,范围都是0-255
	static void RGBAToYUV(char* RGBAData, int width, int height, char* YUVData);
	// YUV420P转换为RGB24
	static void YUV420PToRGB32(unsigned char* yuv420[3], unsigned char* rgb32, int width, int height);
	// 计算16进制的c中1的个数
	static int checkCRC(char c);
	// 比较内存数据是否相同,length为比较的长度
	static bool memoryCompare(char* buffer0, char* buffer1, const int& length);
};

#endif