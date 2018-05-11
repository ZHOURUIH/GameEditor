#include "BinaryUtility.h"
#include "MathUtility.h"

void BinaryUtility::swapPixelBuffer(char* buffer, int width, int height, int blockByte)
{
	for (int i = 0; i < height / 2; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			for (int k = 0; k < blockByte; ++k)
			{
				char temp = buffer[i * width * blockByte + j * blockByte + k];
				buffer[i * width * blockByte + j * blockByte + k] = buffer[(height - 1 - i) * width * blockByte + (width - 1 - j) * blockByte + k];
				buffer[(height - 1 - i) * width * blockByte + (width - 1 - j) * blockByte + k] = temp;
			}
		}
	}

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width / 2; ++j)
		{
			for (int k = 0; k < blockByte; ++k)
			{
				char temp = buffer[i * width * blockByte + j * blockByte + k];
				buffer[i * width * blockByte + j * blockByte + k] = buffer[i * width * blockByte + (width - 1 - j) * blockByte + k];
				buffer[i * width * blockByte + (width - 1 - j) * blockByte + k] = temp;
			}
		}
	}
}

void BinaryUtility::upsideDownBuffer(char* buffer, int bufferWidth, int startX, int startY, int width, int height, int blockByte)
{
	for (int i = 0; i < height / 2; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			for (int k = 0; k < blockByte; ++k)
			{
				char temp = buffer[(i + startY) * bufferWidth * blockByte + (j + startX) * blockByte + k];
				buffer[(i + startY) * bufferWidth * blockByte + (j + startX) * blockByte + k] = buffer[(height + startY - 1 - i) * bufferWidth * blockByte + (j + startX) * blockByte + k];
				buffer[(height + startY - 1 - i) * bufferWidth * blockByte + (j + startX) * blockByte + k] = temp;
			}
		}
	}
}

void BinaryUtility::YUVToRGBA(char* YUVData, int width, int height, char* RGBAData)
{
	char Y, U, V;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Y = YUVData[(i * width + j) * 3 + 0];
			U = YUVData[(i * width + j) * 3 + 1];
			V = YUVData[(i * width + j) * 3 + 2];
			RGBAData[(i * width + j) * 4 + 0] = (char)(Y + 1.14f * V);
			RGBAData[(i * width + j) * 4 + 1] = (char)(Y - 0.394f * U - 0.581f * V);
			RGBAData[(i * width + j) * 4 + 2] = (char)(Y + 2.032f * U);
			RGBAData[(i * width + j) * 4 + 3] = (char)255;
		}
	}
}

void BinaryUtility::RGBAToYUV(char* RGBAData, int width, int height, char* YUVData)
{
	char R, G, B;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			R = RGBAData[(i * width + j) * 4 + 0];
			G = RGBAData[(i * width + j) * 4 + 1];
			B = RGBAData[(i * width + j) * 4 + 2];
			YUVData[(i * width + j) * 3 + 0] = (char)(0.299f * R + 0.587f * G + 0.114f * B);
			YUVData[(i * width + j) * 3 + 1] = (char)(-0.147f * R - 0.289f * G + 0.436f * B);
			YUVData[(i * width + j) * 3 + 2] = (char)(0.615f * R - 0.515f * G - 0.100f * B);
		}
	}
}

void BinaryUtility::YUV420PToRGB32(unsigned char* yuv420[3], unsigned char* rgb32, int width, int height)
{
	int R, G, B, Y, U, V;
	int nWidth = width >> 1; //色度信号宽度
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Y = (int)*(yuv420[0] + y * width + x);
			U = (int)*(yuv420[1] + ((y >> 1) * nWidth) + (x >> 1));
			V = (int)*(yuv420[2] + ((y >> 1) * nWidth) + (x >> 1));
			R = (int)(Y + 1.402f * (V - 128.0f));
			G = (int)(Y - 0.34414f * (U - 128.0f) - 0.71414f * (V - 128.0f));
			B = (int)(Y + 1.772f * (U - 128.0f));

			//防止越界
			MathUtility::limitWithin0To255(R);
			MathUtility::limitWithin0To255(G);
			MathUtility::limitWithin0To255(B);

			*(rgb32 + ((height - y - 1) * width + x) * 4 + 0) = (char)R;
			*(rgb32 + ((height - y - 1) * width + x) * 4 + 1) = (char)G;
			*(rgb32 + ((height - y - 1) * width + x) * 4 + 2) = (char)B;
			*(rgb32 + ((height - y - 1) * width + x) * 4 + 3) = (char)255;
		}
	}
}

int BinaryUtility::crc_check(char c)
{
	int count = 0;
	int bitCount = sizeof(char)* 8;
	for (int i = 0; i < bitCount; ++i)
	{
		if ((c & (0x01 << i)) > 0)
		{
			++count;
		}
	}
	return count;
}