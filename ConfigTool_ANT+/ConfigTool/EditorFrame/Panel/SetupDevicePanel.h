#ifndef _SETUP_DEVICE_PANEL_H_
#define _SETUP_DEVICE_PANEL_H_

#include "EditorPanel.h"
#include "CommonDefine.h"

class SetupDevicePanel : public EditorPanel
{
public:
	SetupDevicePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	~SetupDevicePanel(){}
	void init();
	void update(float elapsedTime);
	// 通知设备绑定列表是否被修改或者是否需要刷新
	void notifyRegisteListModified(const bool& modified, const bool& forceRefresh = false);
	// 通知绑定信息上传的状态
	void notifyUploadState(const UPLOAD_STATE& state);
	DECLARE_EVENT_TABLE()
	void OnSetDataPathButton(wxCommandEvent& event);
	void OnRegisteButton(wxCommandEvent& event);
	void OnUnregisteButton(wxCommandEvent& event);
	void OnReceiveItemSelect(wxListEvent& event);
	void OnReceiveItemActive(wxListEvent& event);
	void OnRegistedItemSelect(wxListEvent& event);
	void OnRegistedItemActive(wxListEvent& event);
	void OnSavedDeviceListButton(wxCommandEvent& event);
	void OnUploadDeviceListButton(wxCommandEvent& event);
protected:
	void setup();
	void setDataPath();
	void refreshSelection();
	void refreshReceiveList();
	void refreshRegisteList();
	void setSelection(const int& receiveListSelect, const int& registeListSelect, const bool& forceRefresh = false);
protected:
	wxTextCtrl* mDataPathText;
	wxButton* mSetDataPathButton;
	wxStaticText* mDeviceIDLabel;
	wxStaticText* mDeviceRegistedLabel;
	wxButton* mRegisteButton;
	wxButton* mUnregisteButton;
	wxTextCtrl* mMachineIndexText;
	wxListCtrl* mReceivedDeviceList;
	wxListCtrl* mRegistedDeviceList;
	wxStaticText* mRegisteDeviceTitle;
	wxButton* mSaveRegisteDeviceButton;
	wxButton* mUploadButton;
	wxStaticText* mUploadStateLabel;
	int mReceiveDeviceSelectItem;
	int mRegisteDeviceSelectItem;
};

#endif