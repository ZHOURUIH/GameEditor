#include "EditorUtility.h"

bool EditorUtility::isIgnoreClass(txSet<std::string>& keywordList, std::string name)
{
	txSet<std::string>::iterator iter = keywordList.begin();
	txSet<std::string>::iterator iterEnd = keywordList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (strstr(name.c_str(), iter->c_str()) != NULL)
		{
			return true;
		}
	}
	return false;
}

bool EditorUtility::isIgnoreClass(txVector<std::string>& keywordList, std::string name)
{
	txVector<std::string>::iterator iter = keywordList.begin();
	txVector<std::string>::iterator iterEnd = keywordList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (strstr(name.c_str(), iter->c_str()) != NULL)
		{
			return true;
		}
	}
	return false;
}