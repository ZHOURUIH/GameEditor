#include "EncryptDLL.h"

const string EncryptDLL::mEncryptIndex = "ghasdjhguiweg87879563sdgw";
myVector<EncryptCharFunc> EncryptDLL::mEncryptFuncList
{
	EncryptDLL::encryptChar0,
	EncryptDLL::encryptChar6,
	EncryptDLL::encryptChar13,
	EncryptDLL::encryptChar3,
	EncryptDLL::encryptChar8,
	EncryptDLL::encryptChar18,
	EncryptDLL::encryptChar4,
	EncryptDLL::encryptChar7,
	EncryptDLL::encryptChar10,
	EncryptDLL::encryptChar19,
	EncryptDLL::encryptChar2,
	EncryptDLL::encryptChar14,
	EncryptDLL::encryptChar5,
	EncryptDLL::encryptChar9,
	EncryptDLL::encryptChar12,
	EncryptDLL::encryptChar15,
	EncryptDLL::encryptChar17,
	EncryptDLL::encryptChar1,
	EncryptDLL::encryptChar11,
	EncryptDLL::encryptChar20,
	EncryptDLL::encryptChar16,
};

bool EncryptDLL::encrypt(const string& fileName, char* contents, const int length)
{
	const string key = fileName + fileName.substr(0, fileName.length() >> 1) + fileName;
	FOR_I(length)
	{
		contents[i] ^= key[i % key.length()];
		contents[i] += fileName[i % fileName.length()];
		contents[i] ^= i * 3;
		contents[i] = mEncryptFuncList[mEncryptIndex[i & 15] & 15](contents[i], i);
	}
	FOR_I((length >> 2))
	{
		const int swapIndex = i + (length >> 1);
		const char temp = contents[swapIndex];
		contents[swapIndex] = contents[i];
		contents[i] = temp;
	}
	return true;
}

char EncryptDLL::encryptChar0(char value, int index)
{
	return value + (index >> 3);
}

char EncryptDLL::encryptChar1(char value, int index)
{
	return value ^ (index + 4);
}

char EncryptDLL::encryptChar2(char value, int index)
{
	return value ^ (index * 4 + 15);
}

char EncryptDLL::encryptChar3(char value, int index)
{
	return value + 23 - index * 5;
}

char EncryptDLL::encryptChar4(char value, int index)
{
	return value + 13 - index * BinaryUtility::crc16_byte(0xFF, 5);
}

char EncryptDLL::encryptChar5(char value, int index)
{
	return value - index * BinaryUtility::crc16_byte(0xFF, index);
}

char EncryptDLL::encryptChar6(char value, int index)
{
	return encryptChar5(value, index) + index;
}

char EncryptDLL::encryptChar7(char value, int index)
{
	return value - BinaryUtility::crc16_byte(0x6F, index) - 13;
}

char EncryptDLL::encryptChar8(char value, int index)
{
	return value ^ BinaryUtility::crc16_byte(0x6F, index * 2);
}

char EncryptDLL::encryptChar9(char value, int index)
{
	return value ^ BinaryUtility::crc16_byte(index - 4, 0x6F);
}

char EncryptDLL::encryptChar10(char value, int index)
{
	return decryptChar9(value, index * 2);
}

char EncryptDLL::encryptChar11(char value, int index)
{
	return decryptChar7(value, index);
}

char EncryptDLL::encryptChar12(char value, int index)
{
	return encryptChar5(value, index + 3);
}

char EncryptDLL::encryptChar13(char value, int index)
{
	return encryptChar3(value, index + 3);
}

char EncryptDLL::encryptChar14(char value, int index)
{
	return encryptChar4(value, index + 3);
}

char EncryptDLL::encryptChar15(char value, int index)
{
	return decryptChar4(value, index ^ 5);
}

char EncryptDLL::encryptChar16(char value, int index)
{
	return decryptChar3(value, index * 2);
}

char EncryptDLL::encryptChar17(char value, int index)
{
	return decryptChar3(value, index ^ 2);
}

char EncryptDLL::encryptChar18(char value, int index)
{
	return value ^ (index ^ (index - 17));
}

char EncryptDLL::encryptChar19(char value, int index)
{
	return decryptChar7(value, index ^ 11);
}

char EncryptDLL::encryptChar20(char value, int index)
{
	return encryptChar7(value, index ^ 11);
}

char EncryptDLL::decryptChar0(char value, int index)
{
	return value - (index >> 3);
}

char EncryptDLL::decryptChar1(char value, int index)
{
	return value ^ (index + 4);
}

char EncryptDLL::decryptChar2(char value, int index)
{
	return value ^ (index * 4 + 15);
}

char EncryptDLL::decryptChar3(char value, int index)
{
	return value - 23 + index * 5;
}

char EncryptDLL::decryptChar4(char value, int index)
{
	return value - 13 + index * BinaryUtility::crc16_byte(0xFF, 5);
}

char EncryptDLL::decryptChar5(char value, int index)
{
	return value + index * BinaryUtility::crc16_byte(0xFF, index);
}

char EncryptDLL::decryptChar6(char value, int index)
{
	return decryptChar5(value, index) - index;
}

char EncryptDLL::decryptChar7(char value, int index)
{
	return value + BinaryUtility::crc16_byte(0x6F, index) + 13;
}

char EncryptDLL::decryptChar8(char value, int index)
{
	return value ^ BinaryUtility::crc16_byte(0x6F, index * 2);
}

char EncryptDLL::decryptChar9(char value, int index)
{
	return value ^ BinaryUtility::crc16_byte(index - 4, 0x6F);
}

char EncryptDLL::decryptChar10(char value, int index)
{
	return encryptChar9(value, index * 2);
}

char EncryptDLL::decryptChar11(char value, int index)
{
	return encryptChar7(value, index);
}

char EncryptDLL::decryptChar12(char value, int index)
{
	return decryptChar5(value, index + 3);
}

char EncryptDLL::decryptChar13(char value, int index)
{
	return decryptChar3(value, index + 3);
}

char EncryptDLL::decryptChar14(char value, int index)
{
	return decryptChar4(value, index + 3);
}

char EncryptDLL::decryptChar15(char value, int index)
{
	return encryptChar4(value, index ^ 5);
}

char EncryptDLL::decryptChar16(char value, int index)
{
	return encryptChar3(value, index * 2);
}

char EncryptDLL::decryptChar17(char value, int index)
{
	return encryptChar3(value, index ^ 2);
}

char EncryptDLL::decryptChar18(char value, int index)
{
	return value ^ (index ^ (index - 17));
}

char EncryptDLL::decryptChar19(char value, int index)
{
	return encryptChar7(value, index ^ 11);
}

char EncryptDLL::decryptChar20(char value, int index)
{
	return decryptChar7(value, index ^ 11);
}