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

#ifdef _X86
#ifdef _DEBUG
#pragma comment(lib, "../Debug/Frame.lib")
#else
#pragma comment(lib, "../Release/Frame.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/Frame.lib")
#else
#pragma comment(lib, "../x64/Release/Frame.lib")
#endif
#endif

typedef function<char(char, int)> EncryptCharFunc;

extern "C" ENCRYPT_DLL_API void encrypt(const char* pFileName, char* contents, int length);

class ENCRYPT_DLL_API EncryptDLL
{
public:
	static const string mEncryptIndex;
	static myVector<EncryptCharFunc> mEncryptFuncList;
};