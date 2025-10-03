#pragma once

#include "wxHeader.h"
#include "EditorEnum.h"
#include "FrameHeader.h"

namespace EditorUtility
{
	// ����һ��csv�ļ�������
	void parseCSV(const string& fullContent, Vector<Vector<string>>& result);
	// ��csv�ļ����һ���ַ�������,addCommaOrReturnΪtrue���������Ӷ���,������ӻ���
	void appendString(string& file, const string& value, bool addCommaOrReturn);
	string getOwnerString(OWNER owner);
	OWNER getOwner(const string& owner);
    int dialogYesNoCancel(const wxString& title, const wxString& message, const wxString& yesText, const wxString& noText, const wxString& cancelText);
    int dialogYesNoCancel(const wxString& message, const wxString& yesText, const wxString& noText, const wxString& cancelText);
    int dialogYesNoCancel(const wxString& title, const wxString& message);
    int dialogYesNoCancel(const wxString& message);
    int dialogYesNo(const wxString& title, const wxString& message, const wxString& yesText, const wxString& noText);
    int dialogYesNo(const wxString& message, const wxString& yesText, const wxString& noText);
    int dialogYesNo(const wxString& title, const wxString& message);
    int dialogYesNo(const wxString& message);
    int dialogOK(const wxString& title, const wxString& message, const wxString& okText);
    int dialogOK(const wxString& message, const wxString& okText);
    int dialogOK(const wxString& message);
};

using namespace EditorUtility;