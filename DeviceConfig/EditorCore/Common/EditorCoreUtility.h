#ifndef _EDITOR_CORE_UTILITY_H_
#define _EDITOR_CORE_UTILITY_H_

#include <string>

#include "txUtility.h"

class EditorCoreUtility
{
public:
	static void logError(const std::string& info);
	static void logInfo(const std::string& info);
	static std::string charToHex(unsigned char byte);
	static std::string charArrayToHexString(unsigned char* data, const int& dataCount);
	static void httpWebRequestGet();
	static std::string readRegistryValue(const std::string& companyName, const std::string& gameName, const std::string& keyName);
	static bool writeRegistryValue(const std::string& companyName, const std::string& gameName, const std::string& value, const std::string& keyName);
};

#endif