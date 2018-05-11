#ifndef _DOWNLOAD_PANEL_H_
#define _DOWNLOAD_PANEL_H_

#include "EditorPanel.h"

class DownloadPanel : public EditorPanel
{
public:
	DownloadPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~DownloadPanel();
	void setup();
	void startDownload();
	void finishDownload();
	void notifyNothingUpdate();
	void notifyStartGenerateLocalFile();
	void notifyFinishGenerateLocalFile();
	void update(float elapsedTime);
	DECLARE_EVENT_TABLE()
	void onCancelButton(wxCommandEvent& event);
protected:
	void updateState();
protected:
	wxStaticText* mTotalProgressLabel;
	wxStaticText* mTotalTimeLabel;
	wxStaticText* mCurDownloadingLabel;
	wxStaticText* mCurSizeLabel;
	wxStaticText* mCurSpeedLabel;
	wxStaticText* mTotalSizeLabel;
	wxGauge* mCurFileProgress;
	wxGauge* mTotalProgress;
	wxButton* mCancelButton;
};

#endif