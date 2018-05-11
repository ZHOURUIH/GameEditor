#ifndef _SYSTEM_UTILITY_H_
#define _SYSTEM_UTILITY_H_

#include "CommonDefine.h"

class SystemUtility
{
public:
	static void getAllProcess(txSet<std::string>& processList);
	static bool launchExecutable(const std::string& path, const std::string& fullName);
	static void setRegistryValue(const std::string& path, const std::string& key, const std::string& value);
	static std::string getRegistryValue(const std::string& path, const std::string& key);
	static std::string getDateTimeString();
};

#endif