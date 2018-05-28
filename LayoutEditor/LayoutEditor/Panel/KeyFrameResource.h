#ifndef _KEY_FRAME_RESOURCE_H_
#define _KEY_FRAME_RESOURCE_H_

#include "EditorPanel.h"

class KeyFrameResource : public EditorPanel
{
public:
	KeyFrameResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~KeyFrameResource(){}
	void setup();
	void refreshList();
	void refreshDimensionList();
	void notifyTremblingActived();
	void notifyDimensionSelected();
	void notifyKeyFrameSelected();
	void notifyNewTrembling(const std::string& name);
	void notifyAddDimension(const std::string& name);
	void notifyDeleteDimension(const std::string& name);
	void notifyAddKeyFrame(const int& index);
	void notifyDeleteKeyFrame(const int& index);
	void notifyTremblingModified(bool modified);
	DECLARE_EVENT_TABLE()
	void OnResourceActivated(wxListEvent& event);
	void OnResourceRightUp(wxContextMenuEvent& event);
	void OnDimensionSelected(wxListEvent& event);
	void OnKeyFrameSelected(wxListEvent& event);
	void OnRefreshResource(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnNewResource(wxCommandEvent& event);
	void OnTremblingInfo(wxCommandEvent& event);
	void OnTremblingFormat(wxCommandEvent& event);
	void OnAddDimension(wxCommandEvent& event);
	void OnDeleteDimension(wxCommandEvent& event);
	void OnInsertKeyFrame(wxCommandEvent& event);
	void OnAddKeyFrame(wxCommandEvent& event);
	void OnDeleteKeyFrame(wxCommandEvent& event);
	void OnDimensionInfo(wxCommandEvent& event);
	void OnKeyFrameTime(wxCommandEvent& event);
	void OnKeyFrameValue(wxCommandEvent& event);
	void OnOpenFile(wxCommandEvent& event);
	void OnOpenDir(wxCommandEvent& event);
protected:
	void refreshKeyFrameList();
	std::string makeKeyFrameName(txTremblingKeyFrame* keyFrame);
	int getKeyFrameIndexFromName(const std::string& name);
protected:
	wxAuiToolBar* mTremblingToolBar;
	wxTextCtrl* mTremblingInfoText;
	wxTextCtrl* mTremblingFormatText;
	wxListCtrl* mResourceList;
	wxAuiToolBar* mDimensionToolBar;
	wxTextCtrl* mNodeInfoText;
	wxListCtrl* mDimensionList;
	wxTextCtrl* mKeyFrameNameText;
	wxTextCtrl* mKeyFrameTimeText;
	wxTextCtrl* mKeyFrameValueText;
	wxListCtrl* mKeyFrameList;
};

#endif