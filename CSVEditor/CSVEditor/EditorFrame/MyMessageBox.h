#pragma once

#include "FrameHeader.h"

// �Զ�����Ϣ�Ի���֧���޸İ�ť���֣�
class CustomMessageBox : public wxDialog
{
public:
    CustomMessageBox(wxWindow* parent, const wxString& title, const wxString& message, const wxString& yesText, const wxString& noText, const wxString& cancelText, int flag);
    // ���ؽ����ӳ�䵽��׼ wxID_YES/wxID_NO/wxID_CANCEL��
    int GetResult() const { return m_result; }
protected:
    void OnButtonClick(wxCommandEvent& event);
protected:
    int m_result = wxID_CANCEL; // Ĭ�Ϸ���ȡ��
};