#include "DecryptDLL.h"

const string DecryptDLL::mDecryptIndex = "ghasdjhguiweg87879563sdgw";

myVector<DecryptCharFunc> DecryptDLL::mDecryptFuncList
{
	BinaryUtility::decryptChar0,
	BinaryUtility::decryptChar6,
	BinaryUtility::decryptChar13,
	BinaryUtility::decryptChar3,
	BinaryUtility::decryptChar8,
	BinaryUtility::decryptChar18,
	BinaryUtility::decryptChar4,
	BinaryUtility::decryptChar7,
	BinaryUtility::decryptChar10,
	BinaryUtility::decryptChar19,
	BinaryUtility::decryptChar2,
	BinaryUtility::decryptChar14,
	BinaryUtility::decryptChar5,
	BinaryUtility::decryptChar9,
	BinaryUtility::decryptChar12,
	BinaryUtility::decryptChar15,
	BinaryUtility::decryptChar17,
	BinaryUtility::decryptChar1,
	BinaryUtility::decryptChar11,
	BinaryUtility::decryptChar20,
	BinaryUtility::decryptChar16,
};

void decrypt(const char* pFileName, char* contents, const int length)
{
	string fileName = pFileName;
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
		contents[i] = DecryptDLL::mDecryptFuncList[DecryptDLL::mDecryptIndex[i & 15] & 15](contents[i], i);
		contents[i] ^= i * 3;
		contents[i] -= fileName[i % fileName.length()];
		contents[i] ^= key[i % key.length()];
	}
}