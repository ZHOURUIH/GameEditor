#ifndef _MAIN_VIEWER_WINDOW_H_
#define _MAIN_VIEWER_WINDOW_H_

#include "EditorHeader.h"
#include "DataDefine.h"

class Data;
class MainViewerWindow : public wxPanel
{
public:
	MainViewerWindow(wxWindow* parent, long style);
	virtual ~MainViewerWindow(){}
	void setup();
	void showData(const std::string& type);
	void deleteItem(int index);
	void insertItem(int index);
	DECLARE_EVENT_TABLE()
	void OnDataItemEditDone(wxDataViewEvent& event);
	void OnDataItemSelectionChanged(wxDataViewEvent& event);
	void OnDataColumnSelected(wxDataViewEvent& event);
	void OnToolDeleteItem(wxCommandEvent& event);
	void OnToolInsertItemFront(wxCommandEvent& event);
	void OnToolInsertItemBelow(wxCommandEvent& event);
	void OnInsertCountText(wxCommandEvent& event);
	void OnFromItemText(wxCommandEvent& event);
	void OnToItemText(wxCommandEvent& event);
	void OnSetItemButton(wxCommandEvent& event);
	void OnIntExpressionCheck(wxCommandEvent& event);
	void OnFloatExpressionCheck(wxCommandEvent& event);
	void OnNumberExpressionCheck(wxCommandEvent& event);
	void OnStringExpressionCheck(wxCommandEvent& event);
protected:
	void setIntOrFloat(bool intOrFloat);
	void setNumberOrString(bool numberOrString);
protected:
	wxAuiToolBar* mAuiToolBar;
	wxDataViewListCtrl* mDataInfoListCtrl;
	wxTextCtrl* mInsertCountText;
	wxTextCtrl* mFromItemText;
	wxTextCtrl* mToItemText;
	wxTextCtrl* mItemValueText;
	wxTextCtrl* mSelectColumnNameText;
	std::string mShowDataType;
	EditorFrame* mEditorFrame;
	int mSelectColumn;
	bool mIntOrFloat;
	bool mNumberOrString;
	std::vector<int> mSelectItemList;
};

#endif