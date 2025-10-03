#include "AllHeader.h"

CustomMessageBox::CustomMessageBox(wxWindow* parent, const wxString& title, const wxString& message, const wxString& yesText, const wxString& noText, const wxString& cancelText, int flag)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(500, 300))
{
    // ���ֿؼ�
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    // �����Ϣ�ı�
    mainSizer->Add(new wxStaticText(this, wxID_ANY, message), 0, wxALL | wxALIGN_CENTER, 10);
    // ������ť��
    wxStdDialogButtonSizer* btnSizer = new wxStdDialogButtonSizer();
    // �Զ��尴ť����
    if (hasFlag(flag, wxYES))
    {
        btnSizer->AddButton(new wxButton(this, wxID_YES, yesText));
    }
    if (hasFlag(flag, wxNO))
    {
        btnSizer->AddButton(new wxButton(this, wxID_NO, noText));
    }
    if (hasFlag(flag, wxCANCEL))
    {
        btnSizer->AddButton(new wxButton(this, wxID_CANCEL, cancelText));
    }
    btnSizer->Realize();
    mainSizer->Add(btnSizer, 0, wxEXPAND | wxALL, 10);
    SetSizerAndFit(mainSizer);
    // �󶨰�ť�¼�
    Bind(wxEVT_BUTTON, &CustomMessageBox::OnButtonClick, this);
}

void CustomMessageBox::OnButtonClick(wxCommandEvent& event)
{
    m_result = event.GetId(); // ��¼����İ�ťID
    EndModal(m_result);
}