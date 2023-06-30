#include "DecryptDLL.h"

const string DecryptDLL::mDecryptIndex = "ghasdjhguiweg87879563sdgw";

myVector<DecryptCharFunc> DecryptDLL::mDecryptFuncList
{
	DecryptDLL::decryptChar0,
	DecryptDLL::decryptChar6,
	DecryptDLL::decryptChar13,
	DecryptDLL::decryptChar3,
	DecryptDLL::decryptChar8,
	DecryptDLL::decryptChar18,
	DecryptDLL::decryptChar4,
	DecryptDLL::decryptChar7,
	DecryptDLL::decryptChar10,
	DecryptDLL::decryptChar19,
	DecryptDLL::decryptChar2,
	DecryptDLL::decryptChar14,
	DecryptDLL::decryptChar5,
	DecryptDLL::decryptChar9,
	DecryptDLL::decryptChar12,
	DecryptDLL::decryptChar15,
	DecryptDLL::decryptChar17,
	DecryptDLL::decryptChar1,
	DecryptDLL::decryptChar11,
	DecryptDLL::decryptChar20,
	DecryptDLL::decryptChar16,
};

bool DecryptDLL::decrypt(const string& fileName, char* contents, const int length)
{
	const string key = fileName + fileName.substr(0, fileName.length() >> 1) + fileName;
	FOR_I(length >> 2)
	{
		const int swapIndex = i + (length >> 1);
		const char temp = contents[swapIndex];
		contents[swapIndex] = contents[i];
		contents[i] = temp;
	}
	FOR_I(length)
	{
		contents[i] = mDecryptFuncList[mDecryptIndex[i & 15] & 15](contents[i], i);
		contents[i] ^= i * 3;
		contents[i] -= fileName[i % fileName.length()];
		contents[i] ^= key[i % key.length()];
	}
	return true;
}

char DecryptDLL::encryptChar0(char value, int index)
{
	return value + (index >> 3);
}

char DecryptDLL::encryptChar1(char value, int index)
{
	return value ^ (index + 4);
}

char DecryptDLL::encryptChar2(char value, int index)
{
	return value ^ (index * 4 + 15);
}

char DecryptDLL::encryptChar3(char value, int index)
{
	return value + 23 - index * 5;
}

char DecryptDLL::encryptChar4(char value, int index)
{
	return value + 13 - index * BinaryUtility::crc16_byte(0xFF, 5);
}

char DecryptDLL::encryptChar5(char value, int index)
{
	return value - index * BinaryUtility::crc16_byte(0xFF, index);
}

char DecryptDLL::encryptChar6(char value, int index)
{
	return encryptChar5(value, index) + index;
}

char DecryptDLL::encryptChar7(char value, int index)
{
	return value - BinaryUtility::crc16_byte(0x6F, index) - 13;
}

char DecryptDLL::encryptChar8(char value, int index)
{
	return value ^ BinaryUtility::crc16_byte(0x6F, index * 2);
}

char DecryptDLL::encryptChar9(char value, int index)
{
	return value ^ BinaryUtility::crc16_byte(index - 4, 0x6F);
}

char DecryptDLL::encryptChar10(char value, int index)
{
	return decryptChar9(value, index * 2);
}

char DecryptDLL::encryptChar11(char value, int index)
{
	return decryptChar7(value, index);
}

char DecryptDLL::encryptChar12(char value, int index)
{
	return encryptChar5(value, index + 3);
}

char DecryptDLL::encryptChar13(char value, int index)
{
	return encryptChar3(value, index + 3);
}

char DecryptDLL::encryptChar14(char value, int index)
{
	return encryptChar4(value, index + 3);
}

char DecryptDLL::encryptChar15(char value, int index)
{
	return decryptChar4(value, index ^ 5);
}

char DecryptDLL::encryptChar16(char value, int index)
{
	return decryptChar3(value, index * 2);
}

char DecryptDLL::encryptChar17(char value, int index)
{
	return decryptChar3(value, index ^ 2);
}

char DecryptDLL::encryptChar18(char value, int index)
{
	return value ^ (index ^ (index - 17));
}

char DecryptDLL::encryptChar19(char value, int index)
{
	return decryptChar7(value, index ^ 11);
}

char DecryptDLL::encryptChar20(char value, int index)
{
	return encryptChar7(value, index ^ 11);
}

char DecryptDLL::decryptChar0(char value, int index)
{
	return value - (index >> 3);
}

char DecryptDLL::decryptChar1(char value, int index)
{
	return value ^ (index + 4);
}

char DecryptDLL::decryptChar2(char value, int index)
{
	return value ^ (index * 4 + 15);
}

char DecryptDLL::decryptChar3(char value, int index)
{
	return value - 23 + index * 5;
}

char DecryptDLL::decryptChar4(char value, int index)
{
	return value - 13 + index * BinaryUtility::crc16_byte(0xFF, 5);
}

char DecryptDLL::decryptChar5(char value, int index)
{
	return value + index * BinaryUtility::crc16_byte(0xFF, index);
}

char DecryptDLL::decryptChar6(char value, int index)
{
	return decryptChar5(value, index) - index;
}

char DecryptDLL::decryptChar7(char value, int index)
{
	return value + BinaryUtility::crc16_byte(0x6F, index) + 13;
}

char DecryptDLL::decryptChar8(char value, int index)
{
	return value ^ BinaryUtility::crc16_byte(0x6F, index * 2);
}

char DecryptDLL::decryptChar9(char value, int index)
{
	return value ^ BinaryUtility::crc16_byte(index - 4, 0x6F);
}

char DecryptDLL::decryptChar10(char value, int index)
{
	return encryptChar9(value, index * 2);
}

char DecryptDLL::decryptChar11(char value, int index)
{
	return encryptChar7(value, index);
}

char DecryptDLL::decryptChar12(char value, int index)
{
	return decryptChar5(value, index + 3);
}

char DecryptDLL::decryptChar13(char value, int index)
{
	return decryptChar3(value, index + 3);
}

char DecryptDLL::decryptChar14(char value, int index)
{
	return decryptChar4(value, index + 3);
}

char DecryptDLL::decryptChar15(char value, int index)
{
	return encryptChar4(value, index ^ 5);
}

char DecryptDLL::decryptChar16(char value, int index)
{
	return encryptChar3(value, index * 2);
}

char DecryptDLL::decryptChar17(char value, int index)
{
	return encryptChar3(value, index ^ 2);
}

char DecryptDLL::decryptChar18(char value, int index)
{
	return value ^ (index ^ (index - 17));
}

char DecryptDLL::decryptChar19(char value, int index)
{
	return encryptChar7(value, index ^ 11);
}

char DecryptDLL::decryptChar20(char value, int index)
{
	return decryptChar7(value, index ^ 11);
}