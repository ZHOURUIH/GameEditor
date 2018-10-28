#include "EditorUtility.h"

bool EditorUtility::isIgnoreClass(txSet<std::string>& keywordList, const std::string& name)
{
	auto iter = keywordList.begin();
	auto iterEnd = keywordList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (strstr(name.c_str(), iter->c_str()) != NULL)
		{
			return true;
		}
	}
	return false;
}

bool EditorUtility::isIgnoreClass(txVector<std::string>& keywordList, const std::string& name)
{
	auto iter = keywordList.begin();
	auto iterEnd = keywordList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (strstr(name.c_str(), iter->c_str()) != NULL)
		{
			return true;
		}
	}
	return false;
}

void EditorUtility::clearUpList(wxListCtrl* list, const int& shownCount)
{
	int itemCount = list->GetItemCount();
	if (itemCount < shownCount)
	{
		resetListItemCount(list, shownCount);
	}
	clearEmptyRow(list, shownCount);
}

void EditorUtility::resetListItemCount(wxListCtrl* listCtrl, const int& count)
{
	int curItemCount = listCtrl->GetItemCount();
	int deltaCount = curItemCount - count;
	// 行数多了,则删除多余的
	if (deltaCount > 0)
	{
		for (int i = 0; i < deltaCount; ++i)
		{
			listCtrl->DeleteItem(curItemCount - i - 1);
		}
	}
	// 行数少了,则添加
	else if (deltaCount < 0)
	{
		deltaCount = -deltaCount;
		for (int i = 0; i < deltaCount; ++i)
		{
			listCtrl->InsertItem(curItemCount + i, wxT(""), -1);
		}
	}
	else
	{
		return;
	}
}

void EditorUtility::clearEmptyRow(wxListCtrl* listCtrl, const int& shownCount)
{
	int itemCount = listCtrl->GetItemCount();
	for (int i = shownCount; i < itemCount; ++i)
	{
		if (listCtrl->GetItemText(i, 0) != "")
		{
			int columnCount = listCtrl->GetColumnCount();
			for (int j = 0; j < columnCount; ++j)
			{
				listCtrl->SetItem(i, j, wxT(""));
			}
		}
	}
}

void EditorUtility::updateItem(wxListCtrl* listCtrl, const int& item, const int& column, const std::string& text)
{
	if (listCtrl->GetItemText(item, column) != text)
	{
		listCtrl->SetItem(item, column, text);
	}
}