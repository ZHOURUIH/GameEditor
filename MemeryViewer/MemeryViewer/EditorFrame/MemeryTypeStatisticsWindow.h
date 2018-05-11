#ifndef _MEMERY_TYPE_STATISTICS_WINDOW_H_
#define _MEMERY_TYPE_STATISTICS_WINDOW_H_

#include <vector>

#include "EditorHeader.h"
#include "MemeryViewer.h"

class MemeryViewer;
class KeywordDialog;
class MemeryTypeStatisticsWindow : public wxPanel
{
public:
	MemeryTypeStatisticsWindow(wxWindow* parent, long style, wxSize size);
	~MemeryTypeStatisticsWindow();
	void init(MemeryViewer* viewer);
	void update(float elapsedTime);
	void destroy();
	void setMaxCount(int count);
	DECLARE_EVENT_TABLE()
	void OnFilterTextChanged(wxCommandEvent& event);
	void OnSearchHideItem(wxCommandEvent& event);
	void OnOpenKeywordList(wxCommandEvent& event);
protected:
	wxListCtrl* mTypeList;				// 内存类型列表
	wxListCtrl* mSearchList;			// 内存搜索结果列表
	wxTextCtrl* mFilterText;			// 关键字搜索框
	wxCheckBox* mSearchHideItem;		// 是否在隐藏项中搜索
	wxButton* mOpenIgnoreKeywordList;	// 打开关键字过滤列表
	wxStaticText* mMemeryCountLabel;	// 内存总数量
	wxStaticText* mMemerySizeLabel;		// 内存总大小
	wxStaticText* mMemeryTypeCountLabel;// 内存类型个数
	MemeryViewer* mMemeryViewer;
	std::vector<MemeryType> mIgnoreMemeryType;
	KeywordDialog* mKeywordListDialog;
};

#endif