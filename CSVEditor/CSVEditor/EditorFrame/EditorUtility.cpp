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
				if (!field.empty() || row.size() != 0)
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
		if (!field.empty() || row.size() != 0)
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
}