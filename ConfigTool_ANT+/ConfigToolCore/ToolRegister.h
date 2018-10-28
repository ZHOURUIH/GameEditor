#include "StringUtility.h"
class ToolRegister
{
public:
	ToolRegister();
	~ToolRegister();
	static std::string readRegistryValue(const std::string& companyName, const std::string& gameName, const std::string& keyName);
	static bool writeRegistryValue(const std::string& companyName, const std::string& gameName, const std::string& value, const std::string& keyName);
};

