#pragma once

#include "FrameUtility.h"

#ifdef _WINDOWS
#ifdef DECRYPT_DLL_EXPORTS
#define DECRYPT_DLL_API __declspec(dllexport)
#else
#define DECRYPT_DLL_API __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#define DECRYPT_DLL_API __attribute__((visibility("default")))
#else
#define DECRYPT_DLL_API
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

typedef function<char(char, int)> DecryptCharFunc;

extern "C" DECRYPT_DLL_API void decrypt(const char* pFileName, char* contents, int length);

class DECRYPT_DLL_API DecryptDLL
{
public:
	static const string mDecryptIndex;
	static myVector<DecryptCharFunc> mDecryptFuncList;
};