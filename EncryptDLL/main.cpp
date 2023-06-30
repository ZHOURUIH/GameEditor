#include <iostream>
#include "EncryptDLL.h"
#include "DecryptDLL.h"

#ifdef _X86
#ifdef _DEBUG
#pragma comment(lib, "../Debug/EncryptDLL.lib")
#pragma comment(lib, "../Debug/DecryptDLL.lib")
#else
#pragma comment(lib, "../Release/EncryptDLL.lib")
#pragma comment(lib, "../Release/DecryptDLL.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/EncryptDLL.lib")
#pragma comment(lib, "../x64/Debug/DecryptDLL.lib")
#else
#pragma comment(lib, "../x64/Release/EncryptDLL.lib")
#pragma comment(lib, "../x64/Release/DecryptDLL.lib")
#endif
#endif

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