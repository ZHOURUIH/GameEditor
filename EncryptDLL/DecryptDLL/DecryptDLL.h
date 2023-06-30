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

#pragma comment(lib, "../x64/Release/Frame.lib")

typedef function<char(char, int)> DecryptCharFunc;

extern "C" DECRYPT_DLL_API void decrypt(const char* pFileName, char* contents, int length);

class DECRYPT_DLL_API DecryptDLL
{
public:
	static const string mDecryptIndex;
	static myVector<DecryptCharFunc> mDecryptFuncList;
};