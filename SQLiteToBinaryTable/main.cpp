#include "SQLiteTableBase.h"
#include "Serializer.h"
#include "FileContent.h"

string START_FALG = "#start";

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
	bool mClientSQLite;
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
	// 枚举类型的实际基础数据类型
	int leftPos = 0;
	int rightPos = 0;
	if (StringUtility::findString(memberInfo.mTypeName, "(", &leftPos) && 
		StringUtility::findString(memberInfo.mTypeName, ")", &rightPos))
	{
		string realType = memberInfo.mTypeName.substr(leftPos + 1, rightPos - leftPos - 1);
		// 列表类型,则替换列表的元素类型
		int leftListPos = 0;
		int rightListPos = 0;
		if (StringUtility::findString(memberInfo.mTypeName, "Vector<", &leftListPos) && 
			StringUtility::findString(memberInfo.mTypeName, ">", &rightListPos))
		{
			StringUtility::replace(memberInfo.mTypeName, strlen("Vector<") + leftListPos, rightListPos, realType);
		}
		// 非列表,则直接替换类型
		else
		{
			memberInfo.mTypeName = realType;
		}
		memberInfo.mMemberName = memberInfo.mMemberName.substr(0, leftPos);
	}
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
				if (!StringUtility::findString(lastLine.c_str(), "[", &startIndex, tagStartIndex))
				{
					break;
				}
				if (!StringUtility::findString(lastLine.c_str(), "]", &endIndex, startIndex))
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
			else
			{
				tempInfo.mOwner = SQLITE_OWNER::BOTH;
			}
			tempInfo.mHotFix = tagList.contains("[HotFix]");
			tempInfo.mClientSQLite = tagList.contains("[ClientSQLite]");

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
	Vector<string> destSQLitePath;
	string typeDefinePath;
	Vector<string> lines;
	FileUtility::openTxtFileLines("./SQLiteToBinaryTableConfig.txt", lines, true);
	for (const string& line : lines)
	{
		Vector<string> params;
		StringUtility::split(line, "=", params);
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
		else if (StringUtility::startWith(paramName, "DestSQLitePath"))
		{
			destSQLitePath.push_back(params[1]);
		}
		else if (paramName == "TypeDefPath")
		{
			typeDefinePath = params[1];
		}
	}

	Map<string, SQLiteInfo> sqliteInfoList;
	parseSQLiteTemplate(typeDefinePath, sqliteInfoList);
	if (sqliteInfoList.size() == 0)
	{
		return 0;
	}

	Vector<string> files;
	FileUtility::findFiles(dataBasePath, files, ".db");
	for(const string& file : files)
	{
		string tableName = StringUtility::getFileNameNoSuffix(file, true);
		const SQLiteInfo& sqliteTableInfo = sqliteInfoList[tableName];
		// 仅服务器使用的表格不需要转换或者拷贝到客户端
		if (sqliteTableInfo.mOwner == SQLITE_OWNER::SERVER_ONLY)
		{
			continue;
		}
		// 如果在客户端也是使用sqlite表格,则直接加密拷贝到客户端
		if (sqliteTableInfo.mClientSQLite)
		{
			FileContent content;
			if (!FileUtility::openBinaryFile(file, &content))
			{
				cout << "打开文件失败:" << file << endl;
				system("pause");
			}
			// 加密
			string key = "ASLD" + tableName;
			key = FileUtility::generateFileMD5(key.c_str(), key.length()) + "23y35y9832635872349862365274732047chsudhgkshgwshfoweh238c42384fync9388v45982nc3484";
			FOR_I(content.mFileSize)
			{
				content.mBuffer[i] ^= key[i % key.length()];
			}
			for (const string& path : destSQLitePath)
			{
				string sqliteFilePath = path + "/" + StringUtility::getFileNameNoSuffix(file, true) + ".bytes";
				FileUtility::writeFile(sqliteFilePath, content.mBuffer, content.mFileSize);
				cout << "加密并拷贝文件:" << sqliteFilePath << endl;
			}
		}
		else
		{
			Serializer serializer;
			SQLiteTableBase table;
			table.setTableName(tableName);
			table.init(file);
			SQLiteDataReader* reader = table.doSelect();
			if (reader == nullptr)
			{
				cout << "加载表格失败:" << file << endl;
				system("pause");
				return 0;
			}
			while (reader->read())
			{
				const auto& memberList = sqliteTableInfo.mMemberList;
				FOR_CONST_J(memberList)
				{
					const string& typeName = memberList[j].mTypeName;
					if (memberList[j].mOwner != SQLITE_OWNER::CLIENT_ONLY && memberList[j].mOwner != SQLITE_OWNER::BOTH)
					{
						continue;
					}
					if (typeName == "bool")
					{
						serializer.write(reader->getInt(j) != 0);
					}
					else if (typeName == "byte")
					{
						serializer.write((byte)reader->getInt(j));
					}
					else if (typeName == "char")
					{
						serializer.write((char)reader->getInt(j));
					}
					else if (typeName == "short")
					{
						serializer.write((short)reader->getInt(j));
					}
					else if (typeName == "ushort")
					{
						serializer.write((ushort)reader->getInt(j));
					}
					else if (typeName == "int")
					{
						serializer.write(reader->getInt(j));
					}
					else if (typeName == "uint")
					{
						serializer.write((uint)reader->getInt(j));
					}
					else if (typeName == "float")
					{
						serializer.write(reader->getFloat(j));
					}
					else if (typeName == "llong")
					{
						serializer.write(reader->getLLong(j));
					}
					else if (typeName == "string")
					{
						string value;
						reader->getString(j, value, false);
						serializer.writeString(value.c_str());
					}
					else if (typeName == "Vector<bool>")
					{
						string value;
						reader->getString(j, value);
						Vector<bool> bools;
						StringUtility::stringToBools(value, bools);
						serializer.writeArray(bools);
					}
					else if (typeName == "Vector<byte>")
					{
						string value;
						reader->getString(j, value);
						Vector<byte> bytes;
						StringUtility::stringToBytes(value, bytes);
						serializer.writeArray(bytes);
					}
					else if (typeName == "Vector<short>")
					{
						string value;
						reader->getString(j, value);
						Vector<short> shorts;
						StringUtility::stringToShorts(value, shorts);
						serializer.writeArray(shorts);
					}
					else if (typeName == "Vector<ushort>")
					{
						string value;
						reader->getString(j, value);
						Vector<ushort> ushorts;
						StringUtility::stringToUShorts(value, ushorts);
						serializer.writeArray(ushorts);
					}
					else if (typeName == "Vector<int>")
					{
						string value;
						reader->getString(j, value);
						Vector<int> ints;
						StringUtility::stringToInts(value, ints);
						serializer.writeArray(ints);
					}
					else if (typeName == "Vector<uint>")
					{
						string value;
						reader->getString(j, value);
						Vector<uint> uints;
						StringUtility::stringToUInts(value, uints);
						serializer.writeArray(uints);
					}
					else if (typeName == "Vector<float>")
					{
						string value;
						reader->getString(j, value);
						Vector<float> floats;
						StringUtility::stringToFloats(value, floats);
						serializer.writeArray(floats);
					}
					else if (typeName == "Vector<string>")
					{
						string value;
						reader->getString(j, value, false);
						Vector<string> strings;
						StringUtility::split(value, ",", strings);
						serializer.writeArray(strings);
					}
					else if (typeName == "Vector2Short")
					{
						string value;
						reader->getString(j, value);
						Vector<short> shorts;
						StringUtility::stringToShorts(value, shorts);
						if (shorts.size() == 0)
						{
							llong id = reader->getLLong(0);
							cout << "字段内容错误,类型Vector2Short,字段名" << memberList[j].mMemberName << ",表格:" << tableName + ",ID:" + StringUtility::llongToString(id) << endl;
							system("pause");
							return 0;
						}
						serializer.write(shorts[0]);
						serializer.write(shorts[1]);
					}
					else if (typeName == "Vector2UShort")
					{
						string value;
						reader->getString(j, value);
						Vector<ushort> ushorts;
						StringUtility::stringToUShorts(value, ushorts);
						if (ushorts.size() == 0)
						{
							llong id = reader->getLLong(0);
							cout << "字段内容错误,类型Vector2Short,字段名" << memberList[j].mMemberName << ",表格:" << tableName + ",ID:" + StringUtility::llongToString(id) << endl;
							system("pause");
							return 0;
						}
						serializer.write(ushorts[0]);
						serializer.write(ushorts[1]);
					}
					else if (typeName == "Vector2Int")
					{
						string value;
						reader->getString(j, value);
						Vector<int> ints;
						StringUtility::stringToInts(value, ints);
						if (ints.size() == 0)
						{
							llong id = reader->getLLong(0);
							cout << "字段内容错误,类型Vector2Int,字段名" << memberList[j].mMemberName << ",表格:" << tableName + ",ID:" + StringUtility::llongToString(id) << endl;
							system("pause");
							return 0;
						}
						serializer.write(ints[0]);
						serializer.write(ints[1]);
					}
					else if (typeName == "Vector2UInt")
					{
						string value;
						reader->getString(j, value);
						Vector<uint> uints;
						StringUtility::stringToUInts(value, uints);
						if (uints.size() == 0)
						{
							llong id = reader->getLLong(0);
							cout << "字段内容错误,类型Vector2Int,字段名" << memberList[j].mMemberName << ",表格:" << tableName + ",ID:" + StringUtility::llongToString(id) << endl;
							system("pause");
							return 0;
						}
						serializer.write(uints[0]);
						serializer.write(uints[1]);
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
			table.releaseReader(reader);
			// 重新计算密钥
			string key = "ASLD" + tableName;
			key = FileUtility::generateFileMD5(key.c_str(), key.length()) + "23y35y983";
			char* buffer = serializer.getWriteableBuffer();
			uint bufferSize = serializer.getBufferSize();
			FOR_J(bufferSize)
			{
				buffer[j] = (buffer[j] - ((j << 1) & 0xFF)) ^ key[j % key.length()];
			}

			string fullPath = destPath + "/" + tableName + ".bytes";
			serializer.writeToFile(fullPath);
			cout << "生成文件:" << fullPath << endl;
		}
	}
	return 0;
}