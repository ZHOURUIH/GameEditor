#include "EditorCoreUtility.h"

std::string EditorCoreUtility::readRegistryValue(const std::string& companyName, const std::string& gameName, const std::string& keyName)
{
	//打开注册表  
	HKEY hKey;
	LONG nRet = RegOpenKeyExA(HKEY_CURRENT_USER, (std::string("SOFTWARE\\") + companyName + "\\" + gameName).c_str(), 0, KEY_READ, &hKey);
	if (nRet != ERROR_SUCCESS)
	{
		return "";
	}
 
	DWORD dwSize = 1024;
	char value[1024] = { 0 };
	DWORD type;
	nRet = RegQueryValueExA(hKey, keyName.c_str(), NULL, &type, (BYTE*)value, &dwSize);
	RegCloseKey(hKey);
	if (nRet != ERROR_SUCCESS)
	{
		return "";
	}
	return value;
}

bool EditorCoreUtility::writeRegistryValue(const std::string& companyName, const std::string& gameName, const std::string& value, const std::string& keyName)
{
	//打开注册表  
	HKEY hKey;
	LONG nRet = RegOpenKeyExA(HKEY_CURRENT_USER, (std::string("SOFTWARE\\") + companyName + "\\" + gameName).c_str(), 0, KEY_WRITE, &hKey);
	if (nRet != ERROR_SUCCESS)
	{
		return false;
	}
	nRet = RegSetValueExA(hKey, keyName.c_str(), 0, REG_SZ, (BYTE*)value.c_str(), value.length() + 1);
	RegCloseKey(hKey);
	if (nRet != ERROR_SUCCESS)
	{
		return false;
	}
	return true;
}