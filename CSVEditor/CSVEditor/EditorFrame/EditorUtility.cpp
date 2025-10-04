#include "AllHeader.h"

namespace EditorUtility
{
	void parseCSV(const string& fullContent, Vector<Vector<string>>& result)
	{
		Vector<string> row;
		string field;
		bool inQuotes = false;
		for (int i = 0; i < (int)fullContent.size(); ++i)
		{
			const char c = fullContent[i];
			if (c == '"')
			{
				if (inQuotes && i + 1 < fullContent.size() && fullContent[i + 1] == '"')
				{
					// ת���˫���� ""
					field.push_back('"');
					++i;
				}
				else
				{
					// ������˳�����
					inQuotes = !inQuotes;
				}
			}
			else if (c == ',' && !inQuotes)
			{
				// ���ŷָ���
				row.emplace_back(move(field));
			}
			else if ((c == '\n' || c == '\r') && !inQuotes)
			{
				// �������У����һ��
				if (!field.empty() || !row.isEmpty())
				{
					row.emplace_back(move(field));
					result.emplace_back(move(row));
				}
				// ���� \r\n ������������ǰ�� \r ����һ���� \n�������� \n
				if (c == '\r' && i + 1 < fullContent.size() && fullContent[i + 1] == '\n')
				{
					++i;
				}
			}
			else
			{
				// ��ͨ�ַ�
				field.push_back(c);
			}
		}

		// ���һ�����û�ӽ�ȥ��Ҫ����
		if (!field.empty() || !row.isEmpty())
		{
			row.emplace_back(move(field));
			result.emplace_back(move(row));
		}
	}

	void appendString(string& file, const string& value, bool addCommaOrReturn)
	{
		string cell = value;
		// �ж��Ƿ���Ҫ������
		if ((cell.find(',') != -1) || (cell.find('"') != -1) ||
			(cell.find('\n') != -1) || (cell.find('\r') != -1))
		{
			string escaped;
			escaped.reserve(cell.size());
			for (char c : cell)
			{
				if (c == '"')
				{
					escaped += "\"\"";  // �滻 " Ϊ ""
				}
				else
				{
					escaped += c;
				}
			}
			cell = "\"" + escaped + "\"";
		}
		file += cell;
		if (addCommaOrReturn)
		{
			file += ",";
		}
		else
		{
			file += "\n";
		}
	}

	string getOwnerString(OWNER owner)
	{
		if (owner == OWNER::NONE)
		{
			return "None";
		}
		else if (owner == OWNER::SERVER)
		{
			return "Server";
		}
		else if (owner == OWNER::CLIENT)
		{
			return "Client";
		}
		else if (owner == OWNER::BOTH)
		{
			return "Both";
		}
		return "";
	}

	OWNER getOwner(const string& owner)
	{
		if (owner == "None")
		{
			return OWNER::NONE;
		}
		else if (owner == "Server")
		{
			return OWNER::SERVER;
		}
		else if (owner == "Client")
		{
			return OWNER::CLIENT;
		}
		else if (owner == "Both")
		{
			return OWNER::BOTH;
		}
		return OWNER::NONE;
	}

	int dialogYesNoCancel(const wxString& title, const wxString& message, const wxString& yesText, const wxString& noText, const wxString& cancelText)
	{
		CustomMessageBox dlg(mEditorFrame, title, message, yesText, noText, cancelText, wxYES | wxNO | wxCANCEL);
		return dlg.ShowModal();
	}

	int dialogYesNoCancel(const wxString& message, const wxString& yesText, const wxString& noText, const wxString& cancelText)
	{
		return dialogYesNoCancel("��ʾ", message, yesText, noText, cancelText);
	}

	int dialogYesNoCancel(const wxString& title, const wxString& message)
	{
		return dialogYesNoCancel(title, message, "��", "��", "ȡ��");
	}

	int dialogYesNoCancel(const wxString& message)
	{
		return dialogYesNoCancel("��ʾ", message, "��", "��", "ȡ��");
	}

	int dialogYesNo(const wxString& title, const wxString& message, const wxString& yesText, const wxString& noText)
	{
		CustomMessageBox dlg(mEditorFrame, title, message, yesText, noText, "", wxYES | wxNO);
		return dlg.ShowModal();
	}

	int dialogYesNo(const wxString& message, const wxString& yesText, const wxString& noText)
	{
		return dialogYesNo("��ʾ", message, yesText, noText);
	}

	int dialogYesNo(const wxString& title, const wxString& message)
	{
		return dialogYesNo(title, message, "��", "��");
	}

	int dialogYesNo(const wxString& message)
	{
		return dialogYesNo("��ʾ", message, "��", "��");
	}

	int dialogOK(const wxString& title, const wxString& message, const wxString& okText)
	{
		CustomMessageBox dlg(mEditorFrame, title, message, okText, "", "", wxYES);
		return dlg.ShowModal();
	}

	int dialogOK(const wxString& title, const wxString& message)
	{
		return dialogOK(title, message, "ȷ��");
	}

	int dialogOK(const wxString& message)
	{
		return dialogOK("��ʾ", message, "ȷ��");
	}
}