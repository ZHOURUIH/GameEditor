#include <iostream>
#include "EncryptDLL.h"
#include "DecryptDLL.h"

#pragma comment(lib, "../x64/Release/EncryptDLL.lib")
#pragma comment(lib, "../x64/Release/DecryptDLL.lib")

int main()
{
	StringUtility::initStringUtility();
	MathUtility::initMathUtility();

	string filePath = "./HotFix.dll";
	FileContent content;
	FileUtility::openBinaryFile(filePath, content, false);
	encrypt(filePath.c_str(), content.mBuffer, content.mFileSize);
	FileUtility::writeFileSimple(filePath + ".encrypt", content.mBuffer, content.mFileSize);
	decrypt(filePath.c_str(), content.mBuffer, content.mFileSize);
	FileUtility::writeFileSimple(filePath + ".decrypt", content.mBuffer, content.mFileSize);
	return 0;
}