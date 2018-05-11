#include "DownloadPanel.h"
#include "File.h"
#include "EditorCore.h"
#include "DownloadManager.h"
#include "StringUtility.h"
#include "MathUtility.h"

enum
{
	ID_CANCEL_BUTTON,
};

BEGIN_EVENT_TABLE(DownloadPanel, EditorPanel)
EVT_BUTTON(ID_CANCEL_BUTTON, onCancelButton)
END_EVENT_TABLE()

float mRefreshTime = 0.5f;

DownloadPanel::DownloadPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
	:
	EditorPanel(parent, position, size, style, name)
{
	setup();
}

DownloadPanel::~DownloadPanel()
{
	;
}

void DownloadPanel::setup()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* curDownloadingSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* curDownloadingProgressSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("当前正在下载 : "), wxDefaultPosition, wxDefaultSize, 0);
	curDownloadingProgressSizer->Add(m_staticText3, 0, wxALL, 5);
	mCurDownloadingLabel = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	curDownloadingProgressSizer->Add(mCurDownloadingLabel, 0, wxALL, 5);
	curDownloadingSizer->Add(curDownloadingProgressSizer, 1, wxEXPAND, 5);

	wxBoxSizer* curDownloadedSizeSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText4 = new wxStaticText(this, wxID_ANY, wxT("当前文件体积 : "), wxDefaultPosition, wxDefaultSize, 0);
	curDownloadedSizeSizer->Add(m_staticText4, 0, wxALL, 5);
	mCurSizeLabel = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	curDownloadedSizeSizer->Add(mCurSizeLabel, 0, wxALL, 5);
	curDownloadingSizer->Add(curDownloadedSizeSizer, 1, wxEXPAND, 5);

	wxBoxSizer* curSpeedSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText5 = new wxStaticText(this, wxID_ANY, wxT("速度 : "), wxDefaultPosition, wxDefaultSize, 0);
	curSpeedSizer->Add(m_staticText5, 0, wxALL, 5);
	mCurSpeedLabel = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	curSpeedSizer->Add(mCurSpeedLabel, 0, wxALL, 5);
	curDownloadingSizer->Add(curSpeedSizer, 1, wxEXPAND, 5);

	topSizer->Add(curDownloadingSizer, 1, wxEXPAND, 5);

	wxBoxSizer* curProgressSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText7 = new wxStaticText(this, wxID_ANY, wxT("当前进度 : "), wxDefaultPosition, wxDefaultSize, 0);
	curProgressSizer->Add(m_staticText7, 0, wxALL, 5);
	mCurFileProgress = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	mCurFileProgress->SetValue(0);
	curProgressSizer->Add(mCurFileProgress, 1, wxALL, 5);
	topSizer->Add(curProgressSizer, 1, wxEXPAND, 5);

	wxBoxSizer* totalDownloadingSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* totalCountSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("进度 : "), wxDefaultPosition, wxDefaultSize, 0);
	totalCountSizer->Add(m_staticText1, 0, wxALL, 5);
	mTotalProgressLabel = new wxStaticText(this, wxID_ANY, wxT("0/0"), wxDefaultPosition, wxDefaultSize, 0);
	totalCountSizer->Add(mTotalProgressLabel, 0, wxALL, 5);
	totalDownloadingSizer->Add(totalCountSizer, 1, wxEXPAND, 5);

	wxBoxSizer* totalTimeSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("预计剩余时间 : "), wxDefaultPosition, wxDefaultSize, 0);
	totalTimeSizer->Add(m_staticText2, 0, wxALL, 5);
	mTotalTimeLabel = new wxStaticText(this, wxID_ANY, wxT("00:00:00"), wxDefaultPosition, wxDefaultSize, 0);
	totalTimeSizer->Add(mTotalTimeLabel, 0, wxALL, 5);
	totalDownloadingSizer->Add(totalTimeSizer, 1, wxEXPAND, 5);

	wxBoxSizer* curDownloadedBulkVolume = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText8 = new wxStaticText(this, wxID_ANY, wxT("文件总大小 : "), wxDefaultPosition, wxDefaultSize, 0);
	curDownloadedBulkVolume->Add(m_staticText8, 0, wxALL, 5);
	mTotalSizeLabel = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	curDownloadedBulkVolume->Add(mTotalSizeLabel, 0, wxALL, 5);
	totalDownloadingSizer->Add(curDownloadedBulkVolume, 1, wxEXPAND, 5);

	wxBoxSizer* totalProgressSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText6 = new wxStaticText(this, wxID_ANY, wxT("总进度 : "), wxDefaultPosition, wxDefaultSize, 0);
	totalProgressSizer->Add(m_staticText6, 0, wxALL, 5);
	mTotalProgress = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	mTotalProgress->SetValue(0);
	totalProgressSizer->Add(mTotalProgress, 1, wxALL, 5);
	totalDownloadingSizer->Add(totalProgressSizer, 1, wxEXPAND, 5);

	topSizer->Add(totalDownloadingSizer, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer7 = new wxBoxSizer(wxVERTICAL);
	mCancelButton = new wxButton(this, ID_CANCEL_BUTTON, wxT("取消"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer7->Add(mCancelButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	topSizer->Add(bSizer7, 1, wxEXPAND, 5);

	this->SetSizer(topSizer);
	this->Layout();
}

void DownloadPanel::startDownload()
{
	DownloadManager* manager = mEditorCore->getDownloadManager();
	File* file = manager->getCurDownloadingFile();
	mCurDownloadingLabel->SetLabel(file->getFileName());
	mCurFileProgress->SetValue(0);
}

void DownloadPanel::finishDownload()
{
	updateState();
	mCancelButton->SetLabel(wxT("完成"));
}

void DownloadPanel::notifyNothingUpdate()
{
	mCancelButton->SetLabel(wxT("退出"));
}

void DownloadPanel::notifyStartGenerateLocalFile()
{
	mCancelButton->Enable(false);
}

void DownloadPanel::notifyFinishGenerateLocalFile()
{
	mCancelButton->Enable(true);
}

void DownloadPanel::update(float elapsedTime)
{
	DownloadManager* manager = mEditorCore->getDownloadManager();
	File* file = manager->getCurDownloadingFile();
	if (manager->getDownloading() && file != NULL && file->getTotalSize() > 0 && manager->getTotalCount() > 0)
	{
		updateState();
	}
}

void DownloadPanel::onCancelButton(wxCommandEvent& event)
{
	mEditorCore->getDownloadManager()->setCancel(true);
}

void DownloadPanel::updateState()
{
	// 增加界面刷新时间 防止界面闪烁
	mRefreshTime -= 0.01f;
	if (mRefreshTime <= 0.0f)
	{
		DownloadManager* manager = mEditorCore->getDownloadManager();
		File* file = manager->getCurDownloadingFile();
		if (file != NULL && file->getFileName() != VERSION && file->getFileName() != FILELIST)
		{
			// 当前进度
			float percent = file->getTotalSize() > 0 ? (float)file->getCurSize() / (float)file->getTotalSize() : 0.0f;
			mCurFileProgress->SetValue((int)(percent * 100.0f));
			mCurSizeLabel->SetLabel(StringUtility::floatToString(file->getCurSize() / 1024.0f, 3) + "KB/" + StringUtility::floatToString(file->getTotalSize() / 1024.0f, 3) + "KB");
			// 总文件体积
			mTotalSizeLabel->SetLabel(StringUtility::floatToString(manager->getTotalSize() / 1024.0f / 1024.0f, 3) + "MB");
			// 当前下载速度
			mCurSpeedLabel->SetLabel(StringUtility::floatToString(manager->getCurSpeed() / 1024.0f, 3) + "KB/S");
			// 总进度
			mTotalProgress->SetValue((int)(((float)manager->getDownloadedCount() / manager->getTotalCount()) * 100.0f));
			mTotalProgressLabel->SetLabel(StringUtility::intToString(manager->getDownloadedCount()) + "/" + StringUtility::intToString(manager->getTotalCount()));
			// 预计剩余时间
			int remainSeconds = (int)manager->getRemainTime();
			int hours, minutes, seconds;
			MathUtility::secondsToHoursMinutesSeconds(remainSeconds, hours, minutes, seconds);
			std::string timeString = StringUtility::intToString(hours, 2) + "小时" + StringUtility::intToString(minutes, 2) + "分" + StringUtility::intToString(seconds, 2) + "秒";
			mTotalTimeLabel->SetLabel(timeString);
			mRefreshTime = 0.5f;
		}
	}
}