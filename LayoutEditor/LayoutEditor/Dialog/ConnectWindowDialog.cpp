#include "Layout.h"
#include "WindowProperty.h"
#include "LayoutUtility.h"

#include "LayoutEditorCore.h"
#include "CommandHeader.h"
#include "WindowHeader.h"

#include "ConnectWindowDialog.h"
#include "EditorApp.h"
#include "EditorFrame.h"

enum
{
	ID_OK_BUTTON,
	ID_CANCEL_BUTTON,
};

BEGIN_EVENT_TABLE(ConnectWindowDialog, EditorDialog)
EVT_BUTTON(ID_OK_BUTTON, ConnectWindowDialog::OnOKButton)
EVT_BUTTON(ID_CANCEL_BUTTON, ConnectWindowDialog::OnCancelButton)
END_EVENT_TABLE()

ConnectWindowDialog::ConnectWindowDialog(wxWindow* parent, const wxString& title, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorDialog(parent, title, wxDefaultPosition, size, style, name)
{
	setup();
}

void ConnectWindowDialog::setup()
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bSizer8 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* operateWindowLabel = new wxStaticText(this, wxID_ANY, wxT("窗口:"));
	bSizer8->Add(operateWindowLabel, 0, wxALL, 5);
	mOperateWindowNameText = new wxTextCtrl(this, wxID_ANY);
	bSizer8->Add(mOperateWindowNameText, 1, wxALL, 5);
	bSizer2->Add(bSizer8, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer9 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* startWindowLabel = new wxStaticText(this, wxID_ANY, wxT("起始连接窗口:"));
	bSizer9->Add(startWindowLabel, 0, wxALL, 5);
	mStartWindowNameText = new wxTextCtrl(this, wxID_ANY);
	bSizer9->Add(mStartWindowNameText, 1, wxALL, 5);
	wxStaticText* endWindowLabel = new wxStaticText(this, wxID_ANY, wxT("终止连接窗口:"));
	bSizer9->Add(endWindowLabel, 0, wxALL, 5);
	mEndWindowNameText = new wxTextCtrl(this, wxID_ANY);
	bSizer9->Add(mEndWindowNameText, 1, wxALL, 5);
	bSizer2->Add(bSizer9, 1, wxEXPAND, 5);
	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* windowNumberLabel = new wxStaticText(this, wxID_ANY, wxT("窗口数量:"));
	bSizer3->Add(windowNumberLabel, 0, wxALL, 5);
	mWindowCountText = new wxTextCtrl(this, wxID_ANY, wxT("0"));
	bSizer3->Add(mWindowCountText, 1, wxALL, 5);
	mAdjustPosition = new wxCheckBox(this, wxID_ANY, wxT("调整位置"));
	bSizer3->Add(mAdjustPosition, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mOKClose = new wxCheckBox(this, wxID_ANY, wxT("确定时是否关闭对话框"));
	bSizer3->Add(mOKClose, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bSizer5 = new wxBoxSizer(wxVERTICAL);
	mOKButton = new wxButton(this, ID_OK_BUTTON, wxT("确定"));
	bSizer5->Add(mOKButton, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);
	bSizer4->Add(bSizer5, 1, wxEXPAND, 5);
	wxBoxSizer* bSizer7 = new wxBoxSizer(wxVERTICAL);
	mCancelButton = new wxButton(this, ID_CANCEL_BUTTON, wxT("取消"));
	bSizer7->Add(mCancelButton, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);
	bSizer4->Add(bSizer7, 1, wxEXPAND, 5);
	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);
}

void ConnectWindowDialog::OnOKButton(wxCommandEvent& event)
{
	::Layout* layout = mLayoutEditorCore->getCurLayout();
	if (layout != NULL)
	{
		std::string rotatePropertyName = txProperty::getPropertyName<WindowPropertyRotation>();
		std::string positionPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		int windowCount = txStringUtility::stringToInt(mWindowCountText->GetValue().ToStdString());
		bool replaceWindow = true, replaceStart = true, replaceEnd = true;
		// 如果不是同时连接多个窗口,则只操作1个窗口
		if (windowCount <= 0)
		{
			replaceWindow = replaceStart = replaceEnd = false;
			windowCount = 1;
		}
		for (int i = 0; i < windowCount; ++i)
		{
			// 获得操作的窗口
			std::string windowName = mOperateWindowNameText->GetValue().ToStdString();
			if (replaceWindow)
			{
				// 如果名字中有可以替换的部分,则下次需要继续替换,否则不再替换
				replaceWindow = txMath::replaceStringKeyword(windowName, "i", i, false);
			}
			Window* window = layout->getWindow(windowName);
			if (window == NULL)
			{
				mEditorFrame->logError("error : can not find window : " + windowName + " in current layout!");
				return;
			}
			// 连接的起始窗口
			std::string startWindowName = mStartWindowNameText->GetValue().ToStdString();
			if (replaceStart)
			{
				replaceStart = txMath::replaceStringKeyword(startWindowName, "i", i, false);
			}
			Window* startWindow = layout->getWindow(startWindowName);
			if (startWindow == NULL)
			{
				mEditorFrame->logError("error : can not find start window : " + startWindowName + " in current layout!");
				return;
			}
			// 连接的终止窗口
			std::string endWindowName = mEndWindowNameText->GetValue().ToStdString();
			if (replaceEnd)
			{
				replaceEnd = txMath::replaceStringKeyword(endWindowName, "i", i, false);
			}
			Window* endWindow = layout->getWindow(endWindowName);
			if (endWindow == NULL)
			{
				mEditorFrame->logError("error : can not find end window : " + endWindowName + " in current layout!");
				return;
			}
			// 得到首尾两个窗口的中心点
			VECTOR2 startPixelCenter = startWindow->getPixelPosition() + startWindow->getPixelSize() / 2.0f;
			VECTOR2 endPixelCenter = endWindow->getPixelPosition() + endWindow->getPixelSize() / 2.0f;
			float rollAngle = -txMath::getAngleFromVectorToVector(VECTOR2(1.0f, 0.0f), endPixelCenter - startPixelCenter);
			CommandWindowSetProperty* cmdRotate = NEW_CMD(cmdRotate);
			cmdRotate->mPropertyName = rotatePropertyName;
			cmdRotate->mPropertyValue = txStringUtility::vector3ToString(VECTOR3(0.0f, 0.0f, rollAngle));
			mCommandSystem->pushCommand(cmdRotate, window);

			// 如果要调整位置
			if (mAdjustPosition->GetValue())
			{
				VECTOR2 windowCenter = (startPixelCenter + endPixelCenter) / 2.0f;
				const VECTOR2& windowSize = window->getPixelSize();
				VECTOR2 windowScreenPos = windowCenter - windowSize / 2.0f;
				VECTOR2 windowLocalPos = windowScreenPos;
				if (window->getParent() != NULL)
				{
					windowLocalPos = LayoutUtility::screenToLocalPosition(windowScreenPos, window->getParent()->getPixelPosition());
				}
				txPoint windowPointPos(txDim(0.0f, windowLocalPos.x), txDim(0.0f, windowLocalPos.y));
				CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
				cmdProperty->mPropertyName = positionPropertyName;
				cmdProperty->mPropertyValue = txStringUtility::pointToString(windowPointPos);
				cmdProperty->mOperator = this;
				mCommandSystem->pushCommand(cmdProperty, window);
			}
		}
	}
	if (mOKClose->GetValue())
	{
		Show(false);
	}
}

void ConnectWindowDialog::OnCancelButton(wxCommandEvent& event)
{
	Show(false);
}