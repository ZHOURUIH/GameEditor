#ifndef _EDITOR_UTILITY_H_
#define _EDITOR_UTILITY_H_

#include "EditorHeader.h"
#include "CommonDefine.h"

class EditorUtility
{
public:
	static void logError(std::string info)
	{
		MessageBoxA(NULL, info.c_str(), "¥ÌŒÛ", MB_OK);
	}
	static void logInfo(std::string info)
	{
		MessageBoxA(NULL, info.c_str(), "Ã· æ", MB_OK);
	}
	static bool isIgnoreClass(txSet<std::string>& keywordList, const std::string& name);
	static bool isIgnoreClass(txVector<std::string>& keywordList, const std::string& name);
	static void clearUpList(wxListCtrl* list, const int& shownCount);
	static void resetListItemCount(wxListCtrl* listCtrl, const int& count);
	static void clearEmptyRow(wxListCtrl* listCtrl, const int& shownCount);
	static void updateItem(wxListCtrl* listCtrl, const int& item, const int& column, const std::string& text);
};

#endif