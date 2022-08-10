#include "SQLiteTableBase.h"
#include "Serializer.h"

string START_FALG = "#start";
string ENCRYPT_KEY = "ASLDIHQWILDjadiuahrfiqwdo!@##*^%ishduhasf#*$^(][][dajfgsdfsdgbweghwoeeghwoegh*(%&#$";

enum class SQLITE_OWNER : byte
{
	NONE,				// 不属于客户端或者服务器,仅表格辅助作用
	BOTH,				// 客户端和服务器都会用到
	CLIENT_ONLY,		// 仅客户端用
	SERVER_ONLY,		// 仅服务器用
};

struct SQLiteMember
{
	SQLITE_OWNER mOwner;
	string mTypeName;
	string mMemberName;
	string mComment;
};

struct SQLiteInfo
{
	Vector<SQLiteMember> mMemberList;
	SQLITE_OWNER mOwner;
	string mSQLiteName;
	string mComment;
	bool mHotFix;
};

SQLiteMember parseSQLiteMemberLine(const string& line, bool ignoreClientServer)
{
	SQLiteMember memberInfo;

	// 字段注释
	int pos = -1;
	string newLine = line;
	if (StringUtility::findString(newLine.c_str(), "//", &pos))
	{
		memberInfo.mComment = newLine.substr(pos + strlen("//"));
		StringUtility::removeStartAll(memberInfo.mComment, ' ');
		newLine = newLine.substr(0, pos);
	}

	// 该字段属于客户端还是服务器
	int rectStartIndex = (int)newLine.find_first_of('[');
	int rectEndIndex = (int)newLine.find_first_of(']', rectStartIndex);
	if (rectStartIndex >= 0 && rectEndIndex >= 0)
	{
		string owner = newLine.substr(rectStartIndex, rectEndIndex - rectStartIndex + 1);
		if (ignoreClientServer)
		{
			memberInfo.mOwner = SQLITE_OWNER::BOTH;
		}
		else
		{
			if (owner == "[Client]")
			{
				memberInfo.mOwner = SQLITE_OWNER::CLIENT_ONLY;
			}
			else if (owner == "[Server]")
			{
				memberInfo.mOwner = SQLITE_OWNER::SERVER_ONLY;
			}
			else if (owner == "[None]")
			{
				memberInfo.mOwner = SQLITE_OWNER::NONE;
			}
			else
			{
				memberInfo.mOwner = SQLITE_OWNER::BOTH;
			}
		}
		newLine.erase(rectStartIndex, rectEndIndex - rectStartIndex + 1);
	}
	else
	{
		memberInfo.mOwner = SQLITE_OWNER::BOTH;
	}
	// 字段类型和字段名
	Vector<string> memberStrList;
	StringUtility::split(newLine.c_str(), " ", memberStrList);
	memberInfo.mTypeName = memberStrList[0];
	memberInfo.mMemberName = memberStrList[1];
	return memberInfo;
}

void parseSQLiteTemplate(const string& filePath, Map<string, SQLiteInfo>& sqliteInfoList)
{
	// 解析模板文件
	string fileContent;
	FileUtility::openTxtFile(filePath, fileContent, true);
	if (fileContent.length() == 0)
	{
		cout << "未找到表格格式文件SQLite.txt" << endl;
		system("pause");
		return;
	}
	Vector<string> lines;
	StringUtility::split(fileContent.c_str(), "\r\n", lines);
	if (lines.size() == 0)
	{
		return;
	}
	bool ignoreClientServer = false;
	if (lines[0] == "IgnoreClientServer")
	{
		lines.eraseAt(0);
		ignoreClientServer = true;
	}

	bool packetStart = false;
	SQLiteInfo tempInfo;
	bool fileStart = false;
	FOR_CONST(lines)
	{
		if (lines[i] == START_FALG)
		{
			fileStart = true;
			continue;
		}
		if (!fileStart)
		{
			continue;
		}
		string line = lines[i];
		// 表格注释
		if (StringUtility::startWith(line, "//"))
		{
			string comment = line.substr(strlen("//"));
			StringUtility::removeStartAll(comment, ' ');
			tempInfo.mComment += comment;
			continue;
		}
		// 去除所有制表符,分号
		StringUtility::removeAll(line, '\t', ';');
		// 成员变量列表起始
		if (line == "{")
		{
			packetStart = true;
			string lastLine = lines[i - 1];
			int tagStartIndex = 0;
			int startIndex = -1;
			int endIndex = -1;
			// 查找标签
			Vector<string> tagList;
			while (true)
			{
				StringUtility::findString(lastLine.c_str(), "[", &startIndex, tagStartIndex);
				StringUtility::findString(lastLine.c_str(), "]", &endIndex, startIndex);
				if (startIndex < 0 || endIndex < 0)
				{
					break;
				}
				tagList.push_back(lastLine.substr(startIndex, endIndex - startIndex + 1));
				tagStartIndex = endIndex;
			}

			// 判断标签
			if (tagList.contains("[Client]"))
			{
				tempInfo.mOwner = SQLITE_OWNER::CLIENT_ONLY;
			}
			else if (tagList.contains("[Server]"))
			{
				tempInfo.mOwner = SQLITE_OWNER::SERVER_ONLY;
			}
			else if (tagList.contains("[None]"))
			{
				tempInfo.mOwner = SQLITE_OWNER::NONE;
			}
			else
			{
				tempInfo.mOwner = SQLITE_OWNER::BOTH;
			}
			tempInfo.mHotFix = tagList.contains("[HotFix]");

			// 获取原始的表格名称
			int firstTagPos = -1;
			if (StringUtility::findString(lastLine.c_str(), "[", &firstTagPos))
			{
				tempInfo.mSQLiteName = lastLine.substr(0, firstTagPos);
			}
			else
			{
				tempInfo.mSQLiteName = lastLine;
			}

			tempInfo.mMemberList.clear();
			// 添加默认的ID字段
			SQLiteMember idMember;
			idMember.mMemberName = "ID";
			idMember.mOwner = SQLITE_OWNER::BOTH;
			idMember.mTypeName = "int";
			idMember.mComment = "唯一ID";
			tempInfo.mMemberList.push_back(idMember);
			continue;
		}
		// 成员变量列表结束
		if (line == "}")
		{
			sqliteInfoList.insert(tempInfo.mSQLiteName, tempInfo);
			tempInfo.mComment = "";
			packetStart = false;
			continue;
		}
		if (packetStart)
		{
			tempInfo.mMemberList.push_back(parseSQLiteMemberLine(line, ignoreClientServer));
		}
	}
	END_CONST();
}

int main()
{
	string dataBasePath;
	string destPath;
	string typeDefinePath;
	Vector<string> lines;
	FileUtility::openTxtFileLines("./Config.txt", lines, true);
	FOR_VECTOR(lines)
	{
		Vector<string> params;
		StringUtility::split(lines[i], "=", params);
		if (params.size() != 2)
		{
			continue;
		}
		const string& paramName = params[0];
		if (paramName == "DataBasePath")
		{
			dataBasePath = params[1];
		}
		else if (paramName == "DestPath")
		{
			destPath = params[1];
		}
		else if (paramName == "TypeDefPath")
		{
			typeDefinePath = params[1];
		}
	}
	END(lines);

	Map<string, SQLiteInfo> sqliteInfoList;
	parseSQLiteTemplate(typeDefinePath, sqliteInfoList);
	if (sqliteInfoList.size() == 0)
	{
		return 0;
	}

	Vector<string> files;
	FileUtility::findFiles(dataBasePath, files, ".db");
	FOR_VECTOR(files)
	{
		Serializer serializer;
		SQLiteTableBase* table = new SQLiteTableBase();
		table->setTableName(StringUtility::getFileNameNoSuffix(files[i], true));
		table->init(files[i]);
		if (table->getTableName() == "Animation")
		{
			int a = 0;
		}
		const SQLiteInfo& sqliteTableInfo = sqliteInfoList[table->getTableName()];
		SQLiteDataReader* reader = table->doSelect();
		while (reader->read())
		{
			const auto& memberList = sqliteTableInfo.mMemberList;
			FOR_CONST_J(memberList)
			{
				const string& typeName = memberList[j].mTypeName;
				if (typeName == "bool")
				{
					int value = reader->getInt(j);
					serializer.write(value != 0);
				}
				else if (typeName == "byte")
				{
					byte value = (byte)reader->getInt(j);
					serializer.write(value);
				}
				else if (typeName == "char")
				{
					char value = (char)reader->getInt(j);
					serializer.write(value);
				}
				else if (typeName == "short")
				{
					short value = (short)reader->getInt(j);
					serializer.write(value);
				}
				else if (typeName == "ushort")
				{
					ushort value = (ushort)reader->getInt(j);
					serializer.write(value);
				}
				else if (typeName == "int")
				{
					int value = reader->getInt(j);
					serializer.write(value);
				}
				else if (typeName == "uint")
				{
					uint value = (uint)reader->getInt(j);
					serializer.write(value);
				}
				else if (typeName == "float")
				{
					float value = reader->getFloat(j);
					serializer.write(value);
				}
				else if (typeName == "llong")
				{
					llong value = (llong)reader->getLLong(j);
					serializer.write(value);
				}
				else if (typeName == "string" || 
						typeName == "Vector<bool>" || 
						typeName == "Vector<byte>" || 
						typeName == "Vector<char>" || 
						typeName == "Vector<short>" || 
						typeName == "Vector<ushort>" || 
						typeName == "Vector<int>" || 
						typeName == "Vector<uint>" || 
						typeName == "Vector<float>" || 
						typeName == "Vector<string>" || 
						typeName == "Vector2Int" || 
						typeName == "Vector2UShort")
				{
					string value;
					reader->getString(j, value);
					serializer.writeString(value.c_str());
				}
				else
				{
					cout << "无法识别的字段类型:" << typeName << endl;
					system("pause");
					return 0;
				}
			}
			END_CONST();
		}
		table->releaseReader(reader);
		// 重新计算密钥
		string key = ENCRYPT_KEY + table->getTableName();
		key = FileUtility::generateFileMD5(key.c_str(), key.length());
		char* buffer = serializer.getWriteableBuffer();
		uint bufferSize = serializer.getBufferSize();
		FOR_J(bufferSize)
		{
			buffer[j] = (buffer[j] - ((j << 1) & 0xFF)) ^ key[j % key.length()];
		}

		string fullPath = destPath + "/" + table->getTableName() + ".bytes";
		serializer.writeToFile(fullPath);
		cout << "生成文件:" << fullPath << endl;
	}
	END(files);
	return 0;
}