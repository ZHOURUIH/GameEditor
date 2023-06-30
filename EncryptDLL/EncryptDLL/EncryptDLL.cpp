#include "EncryptDLL.h"

const string EncryptDLL::mEncryptIndex = "ghasdjhguiweg87879563sdgw";
myVector<EncryptCharFunc> EncryptDLL::mEncryptFuncList
{
	BinaryUtility::encryptChar0,
	BinaryUtility::encryptChar6,
	BinaryUtility::encryptChar13,
	BinaryUtility::encryptChar3,
	BinaryUtility::encryptChar8,
	BinaryUtility::encryptChar18,
	BinaryUtility::encryptChar4,
	BinaryUtility::encryptChar7,
	BinaryUtility::encryptChar10,
	BinaryUtility::encryptChar19,
	BinaryUtility::encryptChar2,
	BinaryUtility::encryptChar14,
	BinaryUtility::encryptChar5,
	BinaryUtility::encryptChar9,
	BinaryUtility::encryptChar12,
	BinaryUtility::encryptChar15,
	BinaryUtility::encryptChar17,
	BinaryUtility::encryptChar1,
	BinaryUtility::encryptChar11,
	BinaryUtility::encryptChar20,
	BinaryUtility::encryptChar16,
};

void encrypt(const char* pFileName, char* contents, const int length)
{
	string fileName = pFileName;
	const string key = fileName + fileName.substr(0, fileName.length() >> 1) + fileName;
	FOR_I(length)
	{
		contents[i] ^= key[i % key.length()];
		contents[i] += fileName[i % fileName.length()];
		contents[i] ^= i * 3;
		contents[i] = EncryptDLL::mEncryptFuncList[EncryptDLL::mEncryptIndex[i & 15] & 15](contents[i], i);
	}
	FOR_I((length >> 2))
	{
		const int swapIndex = i + (length >> 1);
		const char temp = contents[swapIndex];
		contents[swapIndex] = contents[i];
		contents[i] = temp;
	}
}