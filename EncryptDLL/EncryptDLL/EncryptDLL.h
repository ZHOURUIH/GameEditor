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

extern "C" ENCRYPT_DLL_API void encrypt(const char* pFileName, char* contents, int length);

class ENCRYPT_DLL_API EncryptDLL
{
public:
	static const string mEncryptIndex;
	static myVector<EncryptCharFunc> mEncryptFuncList;
};