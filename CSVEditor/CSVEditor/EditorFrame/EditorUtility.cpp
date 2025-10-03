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
					// 转义的双引号 ""
					field.push_back('"');
					++i;
				}
				else
				{
					// 进入或退出引号
					inQuotes = !inQuotes;
				}
			}
			else if (c == ',' && !inQuotes)
			{
				// 逗号分隔列
				row.emplace_back(move(field));
			}
			else if ((c == '\n' || c == '\r') && !inQuotes)
			{
				// 遇到换行，完成一行
				if (!field.empty() || row.size() != 0)
				{
					row.emplace_back(move(field));
					result.emplace_back(move(row));
				}
				// 处理 \r\n 的情况：如果当前是 \r 且下一个是 \n，就跳过 \n
				if (c == '\r' && i + 1 < fullContent.size() && fullContent[i + 1] == '\n')
				{
					++i;
				}
			}
			else
			{
				// 普通字符
				field.push_back(c);
			}
		}

		// 最后一行如果没加进去，要补上
		if (!field.empty() || row.size() != 0)
		{
			row.emplace_back(move(field));
			result.emplace_back(move(row));
		}
	}

	void appendString(string& file, const string& value, bool addCommaOrReturn)
	{
		string cell = value;
		// 判断是否需要加引号
		if ((cell.find(',') != -1) || (cell.find('"') != -1) ||
			(cell.find('\n') != -1) || (cell.find('\r') != -1))
		{
			string escaped;
			escaped.reserve(cell.size());
			for (char c : cell)
			{
				if (c == '"')
				{
					escaped += "\"\"";  // 替换 " 为 ""
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