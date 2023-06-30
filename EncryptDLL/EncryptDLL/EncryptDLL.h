#pragma once

#include "FrameUtility.h"

#ifdef _WINDOWS
#ifdef ENCRYPT_DLL_EXPORTS
#define ENCRYPT_DLL_API __declspec(dllexport)
#else
#define ENCRYPT_DLL_API __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#define ENCRYPT_DLL_API __attribute__((visibility("default")))
#else
#define ENCRYPT_DLL_API
#endif

#pragma comment(lib, "../x64/Release/Frame.lib")

typedef function<char(char, int)> EncryptCharFunc;

class ENCRYPT_DLL_API EncryptDLL
{
public:
	static bool encrypt(const string& fileName, char* contents, int length);
protected:
	static const string mEncryptIndex;
	static myVector<EncryptCharFunc> mEncryptFuncList;
	static char encryptChar0(char value, int index);
	static char encryptChar1(char value, int index);
	static char encryptChar2(char value, int index);
	static char encryptChar3(char value, int index);
	static char encryptChar4(char value, int index);
	static char encryptChar5(char value, int index);
	static char encryptChar6(char value, int index);
	static char encryptChar7(char value, int index);
	static char encryptChar8(char value, int index);
	static char encryptChar9(char value, int index);
	static char encryptChar10(char value, int index);
	static char encryptChar11(char value, int index);
	static char encryptChar12(char value, int index);
	static char encryptChar13(char value, int index);
	static char encryptChar14(char value, int index);
	static char encryptChar15(char value, int index);
	static char encryptChar16(char value, int index);
	static char encryptChar17(char value, int index);
	static char encryptChar18(char value, int index);
	static char encryptChar19(char value, int index);
	static char encryptChar20(char value, int index);
	static char decryptChar0(char value, int index);
	static char decryptChar1(char value, int index);
	static char decryptChar2(char value, int index);
	static char decryptChar3(char value, int index);
	static char decryptChar4(char value, int index);
	static char decryptChar5(char value, int index);
	static char decryptChar6(char value, int index);
	static char decryptChar7(char value, int index);
	static char decryptChar8(char value, int index);
	static char decryptChar9(char value, int index);
	static char decryptChar10(char value, int index);
	static char decryptChar11(char value, int index);
	static char decryptChar12(char value, int index);
	static char decryptChar13(char value, int index);
	static char decryptChar14(char value, int index);
	static char decryptChar15(char value, int index);
	static char decryptChar16(char value, int index);
	static char decryptChar17(char value, int index);
	static char decryptChar18(char value, int index);
	static char decryptChar19(char value, int index);
	static char decryptChar20(char value, int index);
};