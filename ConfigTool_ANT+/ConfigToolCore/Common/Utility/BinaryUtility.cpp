#include "BinaryUtility.h"
#include "MathUtility.h"

/** CRC table for the CRC-16. The poly is 0x8005 (x^16 + x^15 + x^2 + 1) */
uint16_t const BinaryUtility::crc16_table[256] =
{
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

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

bool BinaryUtility::isMemoryEqual(char* buffer0, char* buffer1, int length)
{
	for (int i = 0; i < length; ++i)
	{
		if (buffer0[i] != buffer1[i])
		{
			return false;
		}
	}
	return true;
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

/**
* crc16 - compute the CRC-16 for the data buffer
* @crc: previous CRC value
* @buffer: data pointer
* @len: number of bytes in the buffer
*
* Returns the updated CRC value.
*/
unsigned short BinaryUtility::crc16(unsigned short crc, unsigned char* buffer, unsigned short len)
{
	while (len--)
		crc = crc16_byte(crc, *buffer++);
	return crc;
}

unsigned short BinaryUtility::CRC_Calc16(const volatile void *pvDataPtr_, unsigned int ulSize_)
{
	return CRC_UpdateCRC16(0, pvDataPtr_, ulSize_);
}

// Function to calculate a crc in blocks.  Feed each consecutive data block into
// this function, along with the current value of usCRC as returned by the previous
// call of this function.  The first call of this function should pass the initial
// value (usually 0) of the crc in usCRC.
unsigned short BinaryUtility::CRC_UpdateCRC16(unsigned short usCRC_, const volatile void *pvDataPtr_, unsigned int ulSize_)
{
	unsigned char *pucDataPtr = (unsigned char *)pvDataPtr_;

	while (ulSize_)
	{
		usCRC_ = CRC_Get16(usCRC_, *pucDataPtr);
		pucDataPtr++;
		ulSize_--;
	}

	return usCRC_;
}

///////////////////////////////////////////////////////////////////////
unsigned short BinaryUtility::CRC_Get16(unsigned short usCRC_, unsigned char ucByte_)
{
	static const unsigned short ausCRC16Table[16] =
	{
		0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
		0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
	};
	unsigned short usTemp;

	// compute checksum of lower four bits of byte 
	usTemp = ausCRC16Table[usCRC_ & 0xF];
	usCRC_ = (usCRC_ >> 4) & 0x0FFF;
	usCRC_ = usCRC_ ^ usTemp ^ ausCRC16Table[ucByte_ & 0xF];

	// now compute checksum of upper four bits of byte 
	usTemp = ausCRC16Table[usCRC_ & 0xF];
	usCRC_ = (usCRC_ >> 4) & 0x0FFF;
	usCRC_ = usCRC_ ^ usTemp ^ ausCRC16Table[(ucByte_ >> 4) & 0xF];

	return usCRC_;
}