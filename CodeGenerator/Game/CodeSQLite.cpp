#include "CodeSQLite.h"
#include "SQLiteDescription.h"

void CodeSQLite::generate()
{
	print("正在生成SQLite");

	// 先读取表格描述
	myVector<SQLiteInfo> sqliteInfoList;
	myVector<string> dbFiles;
	findFiles(SQLitePath, dbFiles, ".db");
	FOR_VECTOR(dbFiles)
	{
		SQLiteDescription table;
		table.setTableName("Z_Description");
		table.init(dbFiles[i]);
		const myMap<int, TDDescription*>& list = table.queryAll();
		if (list.size() == 0)
		{
			continue;
		}
		SQLiteInfo info;
		info.mMemberList.clear();
		info.mSQLiteName = getFileNameNoSuffix(dbFiles[i], true);
		FOREACH(item, list)
		{
			TDDescription* data = item->second;
			if (item->first == 1)
			{
				info.mComment = data->mName;
			}
			else if (item->first == 2)
			{
				info.mHotFix = StringUtility::stringToBool(data->mName);
			}
			else if (item->first == 3)
			{
				if (data->mName == "Game")
				{
					info.mServerOwner = SQLITE_SERVER_OWNER::GAME;
				}
				else if (data->mName == "GameCore")
				{
					info.mServerOwner = SQLITE_SERVER_OWNER::GAME_CORE;
				}
				else
				{
					ERROR("需要指定服务器层级:" + info.mSQLiteName);
				}
			}
			else if (item->first == 4)
			{
				if (data->mName == "All")
				{
					info.mOwner = SQLITE_OWNER::BOTH;
				}
				else if (data->mName == "Client")
				{
					info.mOwner = SQLITE_OWNER::CLIENT_ONLY;
				}
				else if (data->mName == "Server")
				{
					info.mOwner = SQLITE_OWNER::SERVER_ONLY;
				}
				else if (data->mName == "None")
				{
					info.mOwner = SQLITE_OWNER::NONE;
				}
				else
				{
					ERROR("表格所属错误:" + info.mSQLiteName);
				}
			}
			else if (item->first == 5)
			{
				info.mClientSQLite = StringUtility::stringToBool(data->mName);
			}
			else
			{
				SQLiteMember member;
				if (data->mOwner == "All")
				{
					member.mOwner = SQLITE_OWNER::BOTH;
				}
				else if (data->mOwner == "Client")
				{
					member.mOwner = SQLITE_OWNER::CLIENT_ONLY;
				}
				else if (data->mOwner == "Server")
				{
					member.mOwner = SQLITE_OWNER::SERVER_ONLY;
				}
				else if (data->mOwner == "None")
				{
					member.mOwner = SQLITE_OWNER::NONE;
				}
				else
				{
					ERROR("owner错误:" + info.mSQLiteName);
				}
				member.mMemberName = data->mName;
				member.mComment = data->mDesc;
				member.mTypeName = data->mType;
				int leftPos = 0;
				int rightPos = 0;
				if (findSubstr(member.mTypeName, "(", &leftPos) && findSubstr(member.mTypeName, ")", &rightPos))
				{
					member.mEnumRealType = member.mTypeName.substr(leftPos + 1, rightPos - leftPos - 1);
					member.mTypeName = member.mTypeName.erase(leftPos, rightPos - leftPos + 1);
				}
				info.mMemberList.push_back(member);
			}
		}
		sqliteInfoList.push_back(info);
	}
	
	// cpp
	string cppGameDataPath = cppGamePath + "DataBase/SQLite/Data/";
	string cppGameTablePath = cppGamePath + "DataBase/SQLite/Table/";
	// 筛选出Server的Game层表格
	myVector<SQLiteInfo> serverGameSQLiteList;
	for (const SQLiteInfo& info : sqliteInfoList)
	{
		if ((info.mOwner == SQLITE_OWNER::BOTH || info.mOwner == SQLITE_OWNER::SERVER_ONLY) && info.mServerOwner == SQLITE_SERVER_OWNER::GAME)
		{
			serverGameSQLiteList.push_back(info);
		}
	}
	// 删除C++的代码文件
	deleteFolder(cppGameDataPath);
	// SQLite的Table文件选择性删除,只删除非服务器使用的文件
	string patterns[2]{ ".cpp", ".h" };
	myVector<string> cppGameTableList;
	findFiles(cppGameTablePath, cppGameTableList, patterns, 2);
	for (const string& str : cppGameTableList)
	{
		bool needDelete = true;
		for (const SQLiteInfo& info : serverGameSQLiteList)
		{
			if ("SQLite" + info.mSQLiteName == getFileNameNoSuffix(str, true))
			{
				needDelete = false;
				break;
			}
		}
		if (needDelete)
		{
			deleteFile(str);
		}
	}

	// 生成代码文件
	for (const SQLiteInfo& info : serverGameSQLiteList)
	{
		// .h代码
		generateCppSQLiteDataFile(info, cppGameDataPath);
		generateCppSQLiteTableFile(info, cppGameTablePath);
	}

	const string gameBaseHeaderPath = cppGamePath + "Common/GameBase.h";
	const string gameBaseSourcePath = cppGamePath + "Common/GameBase.cpp";
	const string gameSTLPoolSourcePath = cppGamePath + "Common/GameSTLPoolRegister.cpp";
	generateCppGameSQLiteRegisteFile(serverGameSQLiteList, getFilePath(cppGameDataPath) + "/");
	generateCppSQLiteInstanceDeclare(serverGameSQLiteList, gameBaseHeaderPath, "");
	generateCppSQLiteInstanceDefine(serverGameSQLiteList, gameBaseSourcePath);
	generateCppSQLiteSTLPoolRegister(serverGameSQLiteList, gameSTLPoolSourcePath);
	generateCppSQLiteInstanceClear(serverGameSQLiteList, gameBaseSourcePath);

	string cppGameCoreDataPath = cppGameCorePath + "DataBase/SQLite/Data/";
	string cppGameCoreTablePath = cppGameCorePath + "DataBase/SQLite/Table/";
	// 筛选出Server的GameCore层表格
	myVector<SQLiteInfo> serverGameCoreSQLiteList;
	for (const SQLiteInfo& info : sqliteInfoList)
	{
		if ((info.mOwner == SQLITE_OWNER::BOTH || info.mOwner == SQLITE_OWNER::SERVER_ONLY) && info.mServerOwner == SQLITE_SERVER_OWNER::GAME_CORE)
		{
			serverGameCoreSQLiteList.push_back(info);
		}
	}
	// 删除C++的代码文件
	deleteFolder(cppGameCoreDataPath);
	// SQLite的Table文件选择性删除,只删除非服务器使用的文件
	myVector<string> cppGameCoreTableList;
	findFiles(cppGameCoreTablePath, cppGameCoreTableList, patterns, 2);
	for (const string& str : cppGameCoreTableList)
	{
		bool needDelete = true;
		for (const SQLiteInfo& info : serverGameCoreSQLiteList)
		{
			if ("SQLite" + info.mSQLiteName == getFileNameNoSuffix(str, true))
			{
				needDelete = false;
				break;
			}
		}
		if (needDelete)
		{
			deleteFile(str);
		}
	}

	// 生成代码文件
	for (const SQLiteInfo& info : serverGameCoreSQLiteList)
	{
		// .h代码
		generateCppSQLiteDataFile(info, cppGameCoreDataPath);
		generateCppSQLiteTableFile(info, cppGameCoreTablePath);
	}

	const string gameCoreBaseHeaderPath = cppGameCorePath + "Common/GameCoreBase.h";
	const string gameCoreBaseSourcePath = cppGameCorePath + "Common/GameCoreBase.cpp";
	const string gameCoreSTLPoolSourcePath = cppGameCorePath + "Common/GameCoreSTLPoolRegister.cpp";
	generateCppGameCoreSQLiteRegisteFile(serverGameCoreSQLiteList, getFilePath(cppGameCoreDataPath) + "/");
	generateCppSQLiteInstanceDeclare(serverGameCoreSQLiteList, gameCoreBaseHeaderPath, "MICRO_LEGEND_CORE_API ");
	generateCppSQLiteInstanceDefine(serverGameCoreSQLiteList, gameCoreBaseSourcePath);
	generateCppSQLiteSTLPoolRegister(serverGameCoreSQLiteList, gameCoreSTLPoolSourcePath);
	generateCppSQLiteInstanceClear(serverGameCoreSQLiteList, gameCoreBaseSourcePath);

	//------------------------------------------------------------------------------------------------------------------------------
	// csharp
	if (!csGamePath.empty())
	{
		string csExcelDataGamePath = csGamePath + "DataBase/Excel/Data/";
		string csExcelDataHotFixPath = ClientHotFixPath + "DataBase/Excel/Data/";
		string csExcelTableGamePath = csGamePath + "DataBase/Excel/Table/";
		string csExcelTableHotFixPath = ClientHotFixPath + "DataBase/Excel/Table/";
		string csExcelTableDeclareGamePath = csGamePath + "Common/";
		string csExcelTableDeclareHotFixPath = ClientHotFixPath + "Common/";

		string csSQLiteDataGamePath = csGamePath + "DataBase/SQLite/Data/";
		string csSQLiteDataHotFixPath = ClientHotFixPath + "DataBase/SQLite/Data/";
		string csSQLiteTableGamePath = csGamePath + "DataBase/SQLite/Table/";
		string csSQLiteTableHotFixPath = ClientHotFixPath + "DataBase/SQLite/Table/";
		// 筛选出Client的表格
		myVector<SQLiteInfo> clientSQLiteList;
		for (const SQLiteInfo& info : sqliteInfoList)
		{
			if (info.mOwner == SQLITE_OWNER::BOTH || info.mOwner == SQLITE_OWNER::CLIENT_ONLY)
			{
				clientSQLiteList.push_back(info);
			}
		}
		// 删除C#的代码文件,c#的只删除代码文件,不删除meta文件
		myVector<string> csDataFileList;
		findFiles(csExcelDataGamePath, csDataFileList, ".cs");
		findFiles(csExcelDataHotFixPath, csDataFileList, ".cs");
		findFiles(csSQLiteDataGamePath, csDataFileList, ".cs");
		findFiles(csSQLiteDataHotFixPath, csDataFileList, ".cs");
		for (const string& str : csDataFileList)
		{
			deleteFile(str);
		}
		myVector<string> csTableFileList;
		findFiles(csExcelTableGamePath, csTableFileList, ".cs");
		findFiles(csExcelTableHotFixPath, csTableFileList, ".cs");
		for (const string& str : csTableFileList)
		{
			deleteFile(str);
		}

		// 生成代码文件
		for (const SQLiteInfo& info : clientSQLiteList)
		{
			// .cs代码的SQLite格式
			if (info.mClientSQLite)
			{
				generateCSharpSQLiteDataFile(info, csSQLiteDataGamePath, csSQLiteDataHotFixPath);
				generateCSharpSQLiteTableFile(info, csSQLiteTableGamePath, csSQLiteTableHotFixPath);
			}
			// .cs代码的Excel格式
			else
			{
				generateCSharpExcelDataFile(info, csExcelDataGamePath, csExcelDataHotFixPath);
				generateCSharpExcelTableFile(info, csExcelTableGamePath, csExcelTableHotFixPath);
			}
		}

		// 在上一层目录生成SQLiteRegister.cs和SQLiteRegister.cs文件
		generateCSharpExcelRegisteFileFile(clientSQLiteList, getFilePath(csExcelDataHotFixPath) + "/", getFilePath(csExcelDataGamePath) + "/");
		generateCSharpSQLiteRegisteFileFile(clientSQLiteList, getFilePath(csSQLiteDataHotFixPath) + "/", getFilePath(csSQLiteDataGamePath) + "/");
		generateCSharpExcelDeclare(clientSQLiteList, csExcelTableDeclareHotFixPath, csExcelTableDeclareGamePath);
	}
	print("完成生成SQLite");
	print("");
}

// TDSQLite.h和TDSQLite.cpp文件
void CodeSQLite::generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, const string& dataFilePath)
{
	// 不含ID的成员字段列表
	myVector<SQLiteMember> memberNoIDList;
	for (const SQLiteMember& member : sqliteInfo.mMemberList)
	{
		if (member.mMemberName == "ID")
		{
			continue;
		}
		memberNoIDList.push_back(member);
	}
	// 不含ID以及非服务器字段的成员字段列表
	myVector<SQLiteMember> memberUsedInServerNoIDList;
	for (const SQLiteMember& member : memberNoIDList)
	{
		if (member.mOwner != SQLITE_OWNER::SERVER_ONLY && member.mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		memberUsedInServerNoIDList.push_back(member);
	}

	// TDSQLite.h
	string header;
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
	line(header, "#pragma once");
	line(header, "");
	line(header, "#include \"SQLiteData.h\"");
	if (sqliteInfo.mServerOwner == SQLITE_SERVER_OWNER::GAME_CORE)
	{
		line(header, "#include \"GameCoreDefine.h\"");
	}
	line(header, "");
	line(header, "// " + sqliteInfo.mComment);
	if (sqliteInfo.mServerOwner == SQLITE_SERVER_OWNER::GAME)
	{
		line(header, "class " + dataClassName + " : public SQLiteData");
	}
	else
	{
		line(header, "class MICRO_LEGEND_CORE_API " + dataClassName + " : public SQLiteData");
	}
	
	line(header, "{");
	line(header, "\tBASE(" + dataClassName + ", SQLiteData);");
	if (memberUsedInServerNoIDList.size() > 0)
	{
		line(header, "public:");
		for (const SQLiteMember& member : memberUsedInServerNoIDList)
		{
			string memberLine;
			if (member.mTypeName == "byte" ||
				member.mTypeName == "char" ||
				member.mTypeName == "ushort" ||
				member.mTypeName == "short" ||
				member.mTypeName == "int" ||
				member.mTypeName == "uint" ||
				member.mTypeName == "llong" ||
				member.mTypeName == "ullong")
			{
				memberLine = "\t" + member.mTypeName + " m" + member.mMemberName + " = 0;";
			}
			else if (member.mTypeName == "bool")
			{
				memberLine = "\t" + member.mTypeName + " m" + member.mMemberName + " = false;";
			}
			else if (member.mTypeName == "float")
			{
				memberLine = "\t" + member.mTypeName + " m" + member.mMemberName + " = 0.0f;";
			}
			else if (!member.mEnumRealType.empty())
			{
				if (startWith(member.mTypeName, "Vector<"))
				{
					memberLine = "\t" + member.mTypeName + " m" + member.mMemberName + ";";
				}
				else
				{
					memberLine = "\t" + member.mTypeName + " m" + member.mMemberName + " = (" + member.mTypeName + ")0;";
				}
			}
			else
			{
				memberLine = "\t" + member.mTypeName + " m" + member.mMemberName + ";";
			}
			uint tabCount = generateAlignTableCount(memberLine, 60);
			FOR_I(tabCount)
			{
				memberLine += '\t';
			}
			memberLine += "// " + member.mComment;
			line(header, memberLine);
		}
	}
	line(header, "public:");
	line(header, "\t" + dataClassName + "()");
	line(header, "\t{");
	FOR_VECTOR(memberNoIDList)
	{
		const SQLiteMember& member = memberNoIDList[i];
		const string& name = member.mMemberName;
		if (member.mOwner != SQLITE_OWNER::SERVER_ONLY && member.mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		if (member.mEnumRealType.empty())
		{
			line(header, "\t\tregisteParam(m" + name + ", " + StringUtility::intToString(i + 1) + ");");
		}
		else
		{
			if (startWith(member.mTypeName, "Vector<"))
			{
				line(header, "\t\tregisteEnumListParam<" + member.mEnumRealType + ">(m" + name + ", " + StringUtility::intToString(i + 1) + ");");
			}
			else
			{
				line(header, "\t\tregisteEnumParam<" + member.mEnumRealType + ">(m" + name + ", " + StringUtility::intToString(i + 1) + ");");
			}
		}
	}
	line(header, "\t}");
	line(header, "\tvoid clone(SQLiteData* target) override;");
	line(header, "\tvoid checkAllColName(SQLiteTableBase* table) override;");
	line(header, "};", false);
	writeFile(dataFilePath + dataClassName + ".h", ANSIToUTF8(header.c_str(), true));

	// TDSQLite.cpp
	string source;
	line(source, "#include \"" + dataClassName + ".h\"");
	line(source, "");
	line(source, "void " + dataClassName + "::clone(SQLiteData* target)");
	line(source, "{");
	line(source, "\tbase::clone(target);");
	// 先检查一下有没有需要拷贝的属性
	if (memberUsedInServerNoIDList.size() > 0)
	{
		line(source, "\tauto* targetData = static_cast<" + dataClassName + "*>(target);");
		for (const SQLiteMember& member : memberUsedInServerNoIDList)
		{
			// 如果是列表则调用列表的clone
			if (startWith(member.mTypeName, "Vector<"))
			{
				line(source, "\tm" + member.mMemberName + ".clone(targetData->m" + member.mMemberName + ");");
			}
			else
			{
				line(source, "\ttargetData->m" + member.mMemberName + " = m" + member.mMemberName + ";");
			}
		}
	}
	line(source, "}");
	line(source, "");
	line(source, "void " + dataClassName + "::checkAllColName(SQLiteTableBase* table)");
	line(source, "{");
	FOR_VECTOR(memberNoIDList)
	{
		const SQLiteMember& member = memberNoIDList[i];
		if (member.mOwner != SQLITE_OWNER::SERVER_ONLY && member.mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		line(source, "\ttable->checkColName(\"" + member.mMemberName + "\", " + StringUtility::intToString(i + 1) + ");");
	}
	line(source, "}", false);
	writeFile(dataFilePath + dataClassName + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// SQLiteTable.h文件
void CodeSQLite::generateCppSQLiteTableFile(const SQLiteInfo& sqliteInfo, const string& tableFilePath)
{
	// SQLiteTable.h
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
	string tableClassName = "SQLite" + sqliteInfo.mSQLiteName;
	string tableFileName = tableFilePath + tableClassName + ".h";
	if (!isFileExist(tableFileName))
	{
		string table;
		line(table, "#pragma once");
		line(table, "");
		line(table, "#include \"" + dataClassName + ".h\"");
		line(table, "#include \"SQLiteTable.h\"");
		line(table, "");
		if (sqliteInfo.mServerOwner == SQLITE_SERVER_OWNER::GAME)
		{
			line(table, "class " + tableClassName + " : public SQLiteTable<" + dataClassName + ">");
		}
		else
		{
			line(table, "class MICRO_LEGEND_CORE_API " + tableClassName + " : public SQLiteTable<" + dataClassName + ">");
		}
		line(table, "{");
		line(table, "public:");
		line(table, "};", false);

		writeFile(tableFileName, ANSIToUTF8(table.c_str(), true));
	}
}

// SQLiteRegister.h和SQLiteRegister.cpp文件
void CodeSQLite::generateCppGameSQLiteRegisteFile(const myVector<SQLiteInfo>& sqliteList, const string& filePath)
{
	// SQLiteRegister.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"GameBase.h\"");
	line(str0, "");
	line(str0, "class GameSQLiteRegister");
	line(str0, "{");
	line(str0, "public:");
	line(str0, "\tstatic void registeAll();");
	line(str0, "};", false);
	writeFile(filePath + "GameSQLiteRegister.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "#include \"GameHeader.h\"");
	line(str1, "");
	line(str1, "void GameSQLiteRegister::registeAll()");
	line(str1, "{");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		const string& sqliteName = sqliteList[i].mSQLiteName;
		line(str1, "\tmSQLite" + sqliteName + " = new SQLite" + sqliteName + "();");
	}
	line(str1, "");
	FOR_I(count)
	{
		const string& sqliteName = sqliteList[i].mSQLiteName;
		line(str1, "\tmSQLiteManager->addSQLiteTable(mSQLite" + sqliteName + ", \"" + sqliteName + "\");");
	}
	line(str1, "}", false);
	writeFile(filePath + "GameSQLiteRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

// SQLiteRegister.h和SQLiteRegister.cpp文件
void CodeSQLite::generateCppGameCoreSQLiteRegisteFile(const myVector<SQLiteInfo>& sqliteList, const string& filePath)
{
	// SQLiteRegister.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"GameCoreBase.h\"");
	line(str0, "");
	line(str0, "class MICRO_LEGEND_CORE_API GameCoreSQLiteRegister");
	line(str0, "{");
	line(str0, "public:");
	line(str0, "\tstatic void registeAll();");
	line(str0, "};", false);
	writeFile(filePath + "GameCoreSQLiteRegister.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "#include \"GameCoreHeader.h\"");
	line(str1, "");
	line(str1, "void GameCoreSQLiteRegister::registeAll()");
	line(str1, "{");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteList[i].mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		const string& sqliteName = sqliteList[i].mSQLiteName;
		line(str1, "\tmSQLite" + sqliteName + " = new SQLite" + sqliteName + "();");
	}
	line(str1, "");
	FOR_I(count)
	{
		if (sqliteList[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteList[i].mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		const string& sqliteName = sqliteList[i].mSQLiteName;
		line(str1, "\tmSQLiteManager->addSQLiteTable(mSQLite" + sqliteName + ", \"" + sqliteName + "\");");
	}
	line(str1, "}", false);
	writeFile(filePath + "GameCoreSQLiteRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

void CodeSQLite::generateCppSQLiteInstanceDeclare(const myVector<SQLiteInfo>& sqliteList, const string& gameBaseHeaderFileName, const string& exprtMacro)
{
	// 更新GameBase.h的特定部分代码
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseHeaderFileName, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// SQLite"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "};"); }))
	{
		return;
	}

	for (const SQLiteInfo& info : sqliteList)
	{
		codeList.insert(++lineStart, "\t" + exprtMacro + "extern SQLite" + info.mSQLiteName + "* mSQLite" + info.mSQLiteName + ";");
	}
	writeFile(gameBaseHeaderFileName, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeSQLite::generateCppSQLiteInstanceDefine(const myVector<SQLiteInfo>& sqliteList, const string& gameBaseCppFileName)
{
	// 更新GameBase.cpp的特定部分代码
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseCppFileName, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// SQLite Define"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}
	for (const SQLiteInfo& info : sqliteList)
	{
		codeList.insert(++lineStart, "\tSQLite" + info.mSQLiteName + "* mSQLite" + info.mSQLiteName + ";");
	}
	writeFile(gameBaseCppFileName, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

// SQLiteSTLPoolRegister.h
void CodeSQLite::generateCppSQLiteSTLPoolRegister(const myVector<SQLiteInfo>& sqliteList, const string& gameSTLPoolFile)
{
	// 更新GameBase.h的特定部分代码
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameSTLPoolFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// SQLite数据类型"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}
	for (const SQLiteInfo& info : sqliteList)
	{
		codeList.insert(++lineStart, "\tmVectorPoolManager->registeVectorPool<TD" + info.mSQLiteName + "*>();");
	}
	writeFile(gameSTLPoolFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeSQLite::generateCppSQLiteInstanceClear(const myVector<SQLiteInfo>& sqliteList, const string& gameBaseCppFileName)
{
	// 更新GameBase.cpp的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseCppFileName, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// SQLite Clear"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const SQLiteInfo& info : sqliteList)
	{
		codeList.insert(++lineStart, "\t\tmSQLite" + info.mSQLiteName + " = nullptr;");
	}
	writeFile(gameBaseCppFileName, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

// ExcelData.cs文件
void CodeSQLite::generateCSharpExcelDataFile(const SQLiteInfo& sqliteInfo, const string& dataFileGamePath, const string& dataFileHotFixPath)
{
	if (sqliteInfo.mClientSQLite || sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY || sqliteInfo.mOwner == SQLITE_OWNER::NONE)
	{
		return;
	}
	string file;
	string dataClassName = "ED" + sqliteInfo.mSQLiteName;
	line(file, "using System;");
	line(file, "using System.Collections.Generic;");
	line(file, "using UnityEngine;");
	line(file, "");
	line(file, "// " + sqliteInfo.mComment);
	line(file, "public class " + dataClassName + " : ExcelData");
	line(file, "{");
	uint memberCount = sqliteInfo.mMemberList.size();
	mySet<string> listMemberSet;
	myVector<pair<string, string>> listMemberList;
	FOR_I(memberCount)
	{
		const SQLiteMember& member = sqliteInfo.mMemberList[i];
		if (member.mMemberName == "ID")
		{
			continue;
		}
		// 不在客户端使用的则不定义成员变量
		if (member.mOwner != SQLITE_OWNER::CLIENT_ONLY && member.mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		string typeName = convertToCSharpType(member.mTypeName);
		//if (!member.mEnumRealType.empty())
		//{
		//	int pos0;
		//	if (findString(typeName.c_str(), "List<", &pos0))
		//	{
		//		int pos1;
		//		findString(typeName.c_str(), ">", &pos1);
		//		replace(typeName, pos0 + (int)strlen("List<"), pos1, member.mEnumRealType);
		//	}
		//	else
		//	{
		//		typeName = member.mTypeName;
		//	}
		//}
		// 列表类型的成员变量存储到单独的列表,因为需要分配内存
		bool isList = findString(typeName.c_str(), "List", nullptr);
		if (isList)
		{
			listMemberList.push_back(make_pair(typeName, member.mMemberName));
			listMemberSet.insert(member.mMemberName);
		}
		string memberLine;
		if (mUseILRuntime || !isList)
		{
			memberLine = "\tpublic " + typeName + " m" + member.mMemberName + ";";
		}
		else
		{
			memberLine = "\tpublic " + typeName + " m" + member.mMemberName + " = new();";
		}
		uint tabCount = generateAlignTableCount(memberLine, 52);
		FOR_I(tabCount)
		{
			memberLine += '\t';
		}
		memberLine += "// " + sqliteInfo.mMemberList[i].mComment;
		line(file, memberLine);
	}
	if (mUseILRuntime && listMemberList.size() > 0)
	{
		line(file, "\tpublic " + dataClassName + "()");
		line(file, "\t{");
		FOR_VECTOR(listMemberList)
		{
			line(file, "\t\tm" + listMemberList[i].second + " = new " + listMemberList[i].first + "();");
		}
		line(file, "\t}");
	}
	line(file, "\tpublic override void read(SerializerRead reader)");
	line(file, "\t{");
	line(file, "\t\tbase.read(reader);");
	FOR_I(memberCount)
	{
		const SQLiteMember& memberInfo = sqliteInfo.mMemberList[i];
		if (memberInfo.mMemberName == "ID")
		{
			continue;
		}
		// 不在客户端使用的则不读取
		if (memberInfo.mOwner != SQLITE_OWNER::CLIENT_ONLY && memberInfo.mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		string typeName = convertToCSharpType(memberInfo.mTypeName);
		if (typeName == "string")
		{
			line(file, "\t\treader.readString(out m" + memberInfo.mMemberName + ");");
		}
		else if (listMemberSet.contains(memberInfo.mMemberName))
		{
			if (memberInfo.mEnumRealType == "byte")
			{
				line(file, "\t\treader.readEnumByteList(m" + memberInfo.mMemberName + ");");
			}
			else
			{
				line(file, "\t\treader.readList(m" + memberInfo.mMemberName + ");");
			}
		}
		else if (memberInfo.mEnumRealType == "byte")
		{
			line(file, "\t\treader.readEnumByte(out m" + memberInfo.mMemberName + ");");
		}
		else
		{
			line(file, "\t\treader.read(out m" + memberInfo.mMemberName + ");");
		}
	}
	line(file, "\t}");
	line(file, "}", false);
	string dataFilePath = sqliteInfo.mHotFix ? dataFileHotFixPath : dataFileGamePath;
	writeFile(dataFilePath + dataClassName + ".cs", ANSIToUTF8(file.c_str(), true));
}

// ExcelTable.cs文件
void CodeSQLite::generateCSharpExcelTableFile(const SQLiteInfo& sqliteInfo, const string& tableFileGamePath, const string& tableFileHotFixPath)
{
	if (sqliteInfo.mClientSQLite || sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY || sqliteInfo.mOwner == SQLITE_OWNER::NONE)
	{
		return;
	}
	// SQLiteTable.cs文件
	string dataClassName = "ED" + sqliteInfo.mSQLiteName;
	string tableClassName = "Excel" + sqliteInfo.mSQLiteName;
	string table;
	line(table, "using System;");
	line(table, "using System.Collections.Generic;");
	line(table, "");
	line(table, "public partial class " + tableClassName + " : ExcelTable");
	line(table, "{");
	line(table, "\t// 由于基类无法知道子类的具体类型,所以将List类型的列表定义到子类中.因为大部分时候外部使用的都是List类型的列表");
	line(table, "\t// 并且ILRuntime热更对于模板支持不太好,所以尽量避免使用模板");
	line(table, "\t// 此处定义一个List是为了方便外部可直接获取,避免每次queryAll时都会创建列表");
	if (mUseILRuntime)
	{
		line(table, "\tprotected List<" + dataClassName + "> mDataList;");
	}
	else
	{
		line(table, "\tprotected List<" + dataClassName + "> mDataList = new();");
	}
	line(table, "\tprotected bool mDataAvailable;");
	if (mUseILRuntime)
	{
		line(table, "\tpublic " + tableClassName + "()");
		line(table, "\t{");
		line(table, "\t\tmDataList = new List<" + dataClassName + ">();");
		line(table, "\t}");
	}
	line(table, "\tpublic " + dataClassName + " query(int id, bool errorIfNull = true)");
	line(table, "\t{");
	line(table, "\t\treturn getData<" + dataClassName + ">(id, errorIfNull);");
	line(table, "\t}");
	line(table, "\tpublic List<" + dataClassName + "> queryAll()");
	line(table, "\t{");
	line(table, "\t\tif (!mDataAvailable)");
	line(table, "\t\t{");
	line(table, "\t\t\tforeach (var item in getDataList())");
	line(table, "\t\t\t{");
	line(table, "\t\t\t\tmDataList.Add(item.Value as " + dataClassName + ");");
	line(table, "\t\t\t}");
	line(table, "\t\t\tmDataAvailable = true;");
	line(table, "\t\t}");
	line(table, "\t\treturn mDataList;");
	line(table, "\t}");
	line(table, "}", false);
	string tableFilePath = sqliteInfo.mHotFix ? tableFileHotFixPath : tableFileGamePath;
	writeFile(tableFilePath + tableClassName + ".cs", ANSIToUTF8(table.c_str(), true));
}

// SQLiteData.cs文件
void CodeSQLite::generateCSharpSQLiteDataFile(const SQLiteInfo& sqliteInfo, const string& dataFileGamePath, const string& dataFileHotFixPath)
{
	if (!sqliteInfo.mClientSQLite || sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY || sqliteInfo.mOwner == SQLITE_OWNER::NONE)
	{
		return;
	}
	string file;
	string dataClassName = "SD" + sqliteInfo.mSQLiteName;
	line(file, "using Mono.Data.Sqlite;");
	line(file, "using System;");
	line(file, "using System.Collections.Generic;");
	line(file, "using UnityEngine;");
	line(file, "");
	line(file, "// " + sqliteInfo.mComment);
	line(file, "public class " + dataClassName + " : SQLiteData");
	line(file, "{");
	myVector<pair<string, string>> listMemberList;
	for (const SQLiteMember& member : sqliteInfo.mMemberList)
	{
		if (member.mMemberName == "ID")
		{
			continue;
		}
		line(file, "\tpublic const string " + member.mMemberName + " = " + "\"" + member.mMemberName + "\";");
	}
	for (const SQLiteMember& member : sqliteInfo.mMemberList)
	{
		if (member.mMemberName == "ID")
		{
			continue;
		}
		// 因为模板文件是按照C++来写的,但是有些类型在C#中是没有的,所以要转换为C#中对应的类型
		string typeName = cppTypeToCSharpType(member.mTypeName);
		if (!member.mEnumRealType.empty())
		{
			int pos0;
			if (findString(typeName.c_str(), "List<", &pos0))
			{
				int pos1;
				findString(typeName.c_str(), ">", &pos1);
				replace(typeName, pos0 + (int)strlen("List<"), pos1, member.mEnumRealType);
			}
			else
			{
				typeName = convertToCSharpType(member.mEnumRealType);
			}
		}

		string publicType;
		if (member.mOwner == SQLITE_OWNER::CLIENT_ONLY || member.mOwner == SQLITE_OWNER::BOTH)
		{
			publicType = "public";
		}
		else
		{
			publicType = "protected";
		}
		// 列表类型的成员变量存储到单独的列表,因为需要分配内存
		bool isList = findString(typeName.c_str(), "List", nullptr);
		if (isList)
		{
			listMemberList.push_back(make_pair(typeName, member.mMemberName));
		}

		string memberLine;
		if (mUseILRuntime || !isList)
		{
			memberLine = "\t" + publicType + " " + typeName + " m" + member.mMemberName + ";";
		}
		else
		{
			memberLine = "\t" + publicType + " " + typeName + " m" + member.mMemberName + " = new();";
		}
		uint tabCount = generateAlignTableCount(memberLine, 52);
		FOR_I(tabCount)
		{
			memberLine += '\t';
		}
		memberLine += "// " + member.mComment;
		line(file, memberLine);
	}
	if (mUseILRuntime && listMemberList.size() > 0)
	{
		line(file, "\tpublic " + dataClassName + "()");
		line(file, "\t{");
		FOR_VECTOR(listMemberList)
		{
			line(file, "\t\tm" + listMemberList[i].second + " = new " + listMemberList[i].first + "();");
		}
		line(file, "\t}");
	}
	line(file, "\tpublic override void parse(SqliteDataReader reader)");
	line(file, "\t{");
	line(file, "\t\tbase.parse(reader);");
	const uint memberCount = sqliteInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		const SQLiteMember& member = sqliteInfo.mMemberList[i];
		if (member.mMemberName == "ID")
		{
			continue;
		}
		line(file, "\t\tparseParam(reader, ref m" + member.mMemberName + ", " + intToString(i) + ");");
	}
	line(file, "\t}");
	line(file, "}", false);
	string dataFilePath = sqliteInfo.mHotFix ? dataFileHotFixPath : dataFileGamePath;
	writeFile(dataFilePath + dataClassName + ".cs", ANSIToUTF8(file.c_str(), true));
}

// SQLiteTable.cs文件
void CodeSQLite::generateCSharpSQLiteTableFile(const SQLiteInfo& sqliteInfo, const string& tableFileGamePath, const string& tableFileHotFixPath)
{
	if (!sqliteInfo.mClientSQLite || sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY || sqliteInfo.mOwner == SQLITE_OWNER::NONE)
	{
		return;
	}
	string tableClassName = "SQLite" + sqliteInfo.mSQLiteName;
	string tableFilePath = sqliteInfo.mHotFix ? tableFileHotFixPath : tableFileGamePath;
	const string fullPath = tableFilePath + tableClassName + ".cs";
	// 不覆盖现有文件
	if (isFileExist(fullPath))
	{
		return;
	}
	// SQLiteTable.cs文件
	string table;
	line(table, "using System;");
	line(table, "using System.Collections.Generic;");
	line(table, "");
	line(table, "public class " + tableClassName + " : SQLiteTable");
	line(table, "{");
	line(table, "}", false);
	writeFile(fullPath, ANSIToUTF8(table.c_str(), true));
}

// ExcelRegister.cs文件
void CodeSQLite::generateCSharpExcelRegisteFileFile(const myVector<SQLiteInfo>& sqliteInfo, const string& fileHotFixPath, const string& fileGamePath)
{
	bool hasMainMember = false;
	for (const SQLiteInfo& info : sqliteInfo)
	{
		if (!info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && !info.mHotFix)
		{
			hasMainMember = true;
			break;
		}
	}

	if (hasMainMember)
	{
		// 主工程中的表格注册
		string mainFile;
		line(mainFile, "using System;");
		line(mainFile, "using static GB;");
		line(mainFile, "using static FrameBase;");
		line(mainFile, "");
		line(mainFile, "public class ExcelRegisterMain");
		line(mainFile, "{");
		line(mainFile, "\tpublic static void registeAll()");
		line(mainFile, "\t{");
		for (const SQLiteInfo& info : sqliteInfo)
		{
			if (!info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && !info.mHotFix)
			{
				string lineStr = "\t\tregisteTable(out mExcel%s, typeof(ED%s), \"%s\");";
				replaceAll(lineStr, "%s", info.mSQLiteName);
				line(mainFile, lineStr);
			}
		}
		line(mainFile, "\t}");
		line(mainFile, "\t//------------------------------------------------------------------------------------------------------------------------------");
		line(mainFile, "\tprotected static void registeTable<T>(out T table, Type dataType, string tableName) where T : ExcelTable");
		line(mainFile, "\t{");
		line(mainFile, "\t\ttable = mExcelManager.registe(tableName, typeof(T), dataType) as T;");
		line(mainFile, "\t}");
		line(mainFile, "}", false);

		writeFile(fileGamePath + "ExcelRegisterMain.cs", ANSIToUTF8(mainFile.c_str(), true));
	}
	else
	{
		deleteFile(fileGamePath + "ExcelRegisterMain.cs");
	}

	bool hasHotFixMember = false;
	for (const SQLiteInfo& info : sqliteInfo)
	{
		if (!info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && info.mHotFix)
		{
			hasHotFixMember = true;
			break;
		}
	}

	if (hasHotFixMember)
	{
		// 热更工程中的表格注册
		string hotFixfile;
		line(hotFixfile, "using System;");
		line(hotFixfile, "using static GBR;");
		line(hotFixfile, "using static FrameBase;");
		line(hotFixfile, "");
		line(hotFixfile, "public class ExcelRegister");
		line(hotFixfile, "{");
		line(hotFixfile, "\tpublic static void registeAll()");
		line(hotFixfile, "\t{");
		for (const SQLiteInfo& info : sqliteInfo)
		{
			if (!info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && info.mHotFix)
			{
				string lineStr = "\t\tregisteTable(out mExcel%s, typeof(ED%s), \"%s\");";
				replaceAll(lineStr, "%s", info.mSQLiteName);
				line(hotFixfile, lineStr);
			}
		}
		line(hotFixfile, "");
		line(hotFixfile, "\t\t// 进入热更以后,所有资源都处于可用状态");
		line(hotFixfile, "\t\tmExcelManager.resourceAvailable();");
		line(hotFixfile, "\t}");
		line(hotFixfile, "\t//------------------------------------------------------------------------------------------------------------------------------");
		line(hotFixfile, "\tprotected static void registeTable<T>(out T table, Type dataType, string tableName) where T : ExcelTable");
		line(hotFixfile, "\t{");
		line(hotFixfile, "\t\ttable = mExcelManager.registe(tableName, typeof(T), dataType) as T;");
		line(hotFixfile, "\t}");
		line(hotFixfile, "}", false);

		writeFile(fileHotFixPath + "ExcelRegister.cs", ANSIToUTF8(hotFixfile.c_str(), true));
	}
	else
	{
		deleteFile(fileHotFixPath + "ExcelRegister.cs");
	}
}

// SQLiteRegister.cs文件
void CodeSQLite::generateCSharpSQLiteRegisteFileFile(const myVector<SQLiteInfo>& sqliteInfo, const string& fileHotFixPath, const string& fileGamePath)
{
	bool hasMainMember = false;
	for (const SQLiteInfo& info : sqliteInfo)
	{
		if (info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && !info.mHotFix)
		{
			hasMainMember = true;
			break;
		}
	}

	if (hasMainMember)
	{
		// 主工程中的表格注册
		string mainFile;
		line(mainFile, "using System;");
		line(mainFile, "using static GB;");
		line(mainFile, "using static FrameBase;");
		line(mainFile, "");
		line(mainFile, "public class SQLiteRegisterMain");
		line(mainFile, "{");
		line(mainFile, "\tpublic static void registeAll()");
		line(mainFile, "\t{");
		for (const SQLiteInfo& info : sqliteInfo)
		{
			if (info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && !info.mHotFix)
			{
				string lineStr = "\t\tregisteTable(out mSQLite%s, typeof(SD%s), \"%s\");";
				replaceAll(lineStr, "%s", info.mSQLiteName);
				line(mainFile, lineStr);
			}
		}
		line(mainFile, "\t}");
		line(mainFile, "\t//-------------------------------------------------------------------------------------------------------------");
		line(mainFile, "\tprotected static void registeTable<T>(out T sqliteTable, Type dataType, string tableName) where T : SQLiteTable");
		line(mainFile, "\t{");
		line(mainFile, "\t\tsqliteTable = mSQLiteManager.registeTable(typeof(T), dataType, tableName) as T;");
		line(mainFile, "\t}");
		line(mainFile, "}", false);

		writeFile(fileGamePath + "SQLiteRegisterMain.cs", ANSIToUTF8(mainFile.c_str(), true));
	}
	else
	{
		deleteFile(fileGamePath + "SQLiteRegisterMain.cs");
	}

	// 热更工程中的表格注册
	bool hasHotFixSQLite = false;
	for (const SQLiteInfo& info : sqliteInfo)
	{
		if (info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && info.mHotFix)
		{
			hasHotFixSQLite = true;
		}
	}
	if (hasHotFixSQLite)
	{
		string hotFixfile;
		line(hotFixfile, "using System;");
		line(hotFixfile, "using static GBR;");
		line(hotFixfile, "using static FrameBase;");
		line(hotFixfile, "");
		line(hotFixfile, "public class SQLiteRegister");
		line(hotFixfile, "{");
		line(hotFixfile, "\tpublic static void registeAll()");
		line(hotFixfile, "\t{");
		for (const SQLiteInfo& info : sqliteInfo)
		{
			if (info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && info.mHotFix)
			{
				string lineStr = "\t\tregisteTable(out mSQLite%s, typeof(SD%s), \"%s\");";
				replaceAll(lineStr, "%s", info.mSQLiteName);
				line(hotFixfile, lineStr);
			}
		}
		line(hotFixfile, "");
		line(hotFixfile, "\t\t// 进入热更以后,所有资源都处于可用状态");
		line(hotFixfile, "\t\tmSQLiteManager.resourceAvailable();");
		line(hotFixfile, "\t}");
		line(hotFixfile, "\t//------------------------------------------------------------------------------------------------------------------------------");
		line(hotFixfile, "\tprotected static void registeTable<T>(out T table, Type dataType, string tableName) where T : SQLiteTable");
		line(hotFixfile, "\t{");
		line(hotFixfile, "\t\ttable = mSQLiteManager.registeTable(typeof(T), dataType, tableName) as T;");
		line(hotFixfile, "\t}");
		line(hotFixfile, "}", false);

		writeFile(fileHotFixPath + "SQLiteRegister.cs", ANSIToUTF8(hotFixfile.c_str(), true));
	}
	else
	{
		deleteFile(fileHotFixPath + "SQLiteRegister.cs");
	}
}

// GameBaseExcel.cs文件
void CodeSQLite::generateCSharpExcelDeclare(const myVector<SQLiteInfo>& sqliteInfo, const string& fileHotFixPath, const string& fileGamePath)
{
	bool hasMainMember = false;
	for (const SQLiteInfo& info : sqliteInfo)
	{
		if (!info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && !info.mHotFix)
		{
			hasMainMember = true;
			break;
		}
	}

	if (hasMainMember)
	{
		// 主工程中的表格注册
		string mainFile;
		line(mainFile, "using System;");
		line(mainFile, "");
		line(mainFile, "// GameBase的部分类,用于定义Excel表格的对象");
		line(mainFile, "public partial class GB");
		line(mainFile, "{");
		for (const SQLiteInfo& info : sqliteInfo)
		{
			if (!info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && !info.mHotFix)
			{
				line(mainFile, "\tpublic static Excel" + info.mSQLiteName + " mExcel" + info.mSQLiteName + ";");
			}
		}
		line(mainFile, "}", false);

		writeFile(fileGamePath + "GameBaseExcel.cs", ANSIToUTF8(mainFile.c_str(), true));
	}
	else
	{
		deleteFile(fileGamePath + "GameBaseExcel.cs");
	}

	bool hasHotfixMember = false;
	for (const SQLiteInfo& info : sqliteInfo)
	{
		if (!info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && info.mHotFix)
		{
			hasHotfixMember = true;
			break;
		}
	}

	if (hasHotfixMember)
	{
		// 热更工程中的表格注册
		string hotFixfile;
		line(hotFixfile, "using System;");
		line(hotFixfile, "");
		line(hotFixfile, "// FrameBase的部分类,用于定义Excel表格的对象");
		line(hotFixfile, "public partial class GBR");
		line(hotFixfile, "{");
		for (const SQLiteInfo& info : sqliteInfo)
		{
			if (!info.mClientSQLite && info.mOwner != SQLITE_OWNER::SERVER_ONLY && info.mOwner != SQLITE_OWNER::NONE && info.mHotFix)
			{
				line(hotFixfile, "\tpublic static Excel" + info.mSQLiteName + " mExcel" + info.mSQLiteName + ";");
			}
		}
		line(hotFixfile, "}", false);

		writeFile(fileHotFixPath + "GameBaseExcelILR.cs", ANSIToUTF8(hotFixfile.c_str(), true));
	}
	else
	{
		deleteFile(fileHotFixPath + "GameBaseExcelILR.cs");
	}
}