#ifndef _EDITOR_CORE_UTILITY_H_
#define _EDITOR_CORE_UTILITY_H_

#include "Header.h"

class EditorCoreUtility
{
public:
	static std::string readRegistryValue(const std::string& companyName, const std::string& gameName, const std::string& keyName);
	static bool writeRegistryValue(const std::string& companyName, const std::string& gameName, const std::string& value, const std::string& keyName);
};

#endif