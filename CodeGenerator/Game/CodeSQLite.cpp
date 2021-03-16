#include "CodeSQLite.h"

void CodeSQLite::generate()
{
	string cppDataPath = cppGamePath + "DataBase/SQLite/Data/";
	string cppTablePath = cppGamePath + "DataBase/SQLite/Table/";
	string csDataPath = csGamePath + "DataBase/SQLite/Data/";
	string csTablePath = csGamePath + "DataBase/SQLite/Table/";

	// 解析模板文件
	string fileContent;
	openTxtFile("SQLite.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("未找到表格格式文件SQLite.txt");
		return;
	}
	fileContent = UTF8ToANSI(fileContent.c_str(), true);
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	bool packetStart = false;
	myVector<SQLiteInfo> sqliteInfoList;
	SQLiteInfo tempInfo;
	FOR_VECTOR_CONST(lines)
	{
		string line = lines[i];
		// 忽略注释
		if (startWith(line, "//"))
		{
			continue;
		}
		// 如果后面插有注释,则去除
		int pos = -1;
		if (findString(line.c_str(), "//", &pos))
		{
			line = line.substr(0, pos);
		}
		// 去除所有制表符
		strReplaceAll(line, "\t", "");
		// 去除所有的分号
		strReplaceAll(line, ";", "");
		// 成员变量列表起始
		if (line == "{")
		{
			packetStart = true;
			string lastLine = lines[i - 1];
			int startIndex = -1;
			int endIndex = -1;
			findString(lastLine.c_str(), "[", &startIndex);
			findString(lastLine.c_str(), "]", &endIndex, startIndex);
			if (startIndex >= 0 && endIndex >= 0)
			{
				tempInfo.mSQLiteName = lastLine.substr(0, startIndex);
				string owner = lastLine.substr(startIndex, endIndex - startIndex + 1);
				if (owner == "[Client]")
				{
					tempInfo.mOwner = SQLITE_OWNER::CLIENT_ONLY;
				}
				else if (owner == "[Server]")
				{
					tempInfo.mOwner = SQLITE_OWNER::SERVER_ONLY;
				}
				else
				{
					tempInfo.mOwner = SQLITE_OWNER::BOTH;
				}
			}
			else
			{
				tempInfo.mSQLiteName = lastLine;
				tempInfo.mOwner = SQLITE_OWNER::BOTH;
			}
			tempInfo.mMemberList.clear();
			// 添加默认的ID字段
			SQLiteMember idMember;
			idMember.mMemberName = "ID";
			idMember.mOwner = SQLITE_OWNER::BOTH;
			idMember.mTypeName = "int";
			tempInfo.mMemberList.push_back(idMember);
			continue;
		}
		// 成员变量列表结束
		if (line == "}")
		{
			sqliteInfoList.push_back(tempInfo);
			packetStart = false;
			continue;
		}
		if (packetStart)
		{
			tempInfo.mMemberList.push_back(parseSQLiteMemberLine(line));
		}
	}
	// 删除C++的代码文件
	deleteFolder(cppDataPath);
	// 删除C#的代码文件,c#的只删除代码文件,不删除meta文件
	myVector<string> csDataFileList;
	findFiles(csDataPath, csDataFileList, ".cs");
	FOR_VECTOR_CONST(csDataFileList)
	{
		deleteFile(csDataFileList[i]);
	}
	myVector<string> csTableFileList;
	findFiles(csTablePath, csTableFileList, ".cs");
	FOR_VECTOR_CONST(csTableFileList)
	{
		deleteFile(csTableFileList[i]);
	}

	// 生成代码文件
	FOR_VECTOR_CONST(sqliteInfoList)
	{
		// .h代码
		generateCppSQLiteDataFile(sqliteInfoList[i], cppDataPath, cppTablePath);
		// .cs代码
		generateCSharpSQLiteDataFile(sqliteInfoList[i], csDataPath, csTablePath);
	}

	// 在上一层目录生成SQLiteHeader.h文件
	string headerPath = cppDataPath;
	if (endWith(headerPath, "/") || endWith(headerPath, "\\"))
	{
		headerPath = headerPath.substr(0, headerPath.length() - 1);
	}
	headerPath = getFilePath(headerPath) + "/";
	generateCppSQLiteTotalHeaderFile(sqliteInfoList, headerPath);
	generateCppSQLiteRegisteFile(sqliteInfoList, headerPath);
	generateCppSQLiteInstanceDeclare(sqliteInfoList, headerPath);

	// 在上一层目录生成SQLiteRegister.cs文件
	string registerPath = csDataPath;
	if (registerPath[registerPath.length() - 1] == '/' || registerPath[registerPath.length() - 1] == '\\')
	{
		registerPath = registerPath.substr(0, registerPath.length() - 1);
	}
	registerPath = getFilePath(registerPath) + "/";
	generateCSharpSQLiteRegisteFileFile(sqliteInfoList, registerPath);
}

// TDSQLite.h和TDSQLite.cpp,SQLiteTable.h文件
void CodeSQLite::generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath, string tableFilePath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::CLIENT_ONLY)
	{
		return;
	}
	// TDSQLite.h
	string header;
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
	string headerMacro = "_TD" + nameToUpper(sqliteInfo.mSQLiteName) + "_H_";
	line(header, "#ifndef " + headerMacro);
	line(header, "#define " + headerMacro);
	line(header, "");
	line(header, "#include \"SQLiteData.h\"");
	line(header, "");
	line(header, "class " + dataClassName + " : public SQLiteData");
	line(header, "{");
	line(header, "public:");
	uint memberCount = sqliteInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(header, "\tstatic const char* " + sqliteInfo.mMemberList[i].mMemberName + ";");
	}
	line(header, "public:");
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mOwner != SQLITE_OWNER::CLIENT_ONLY)
		{
			line(header, "\t" + sqliteInfo.mMemberList[i].mTypeName + " m" + sqliteInfo.mMemberList[i].mMemberName + ";");
		}
	}
	line(header, "public:");
	line(header, "\t" + dataClassName + "()");
	line(header, "\t{");
	FOR_I(memberCount)
	{
		const string& name = sqliteInfo.mMemberList[i].mMemberName;
		if (sqliteInfo.mMemberList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			
			line(header, "\t\tregisteParamEmpty(" + name + ");");
		}
		else
		{
			line(header, "\t\tregisteParam(m" + name + ", " + name + ");");
		}
	}
	line(header, "\t}");
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	// TDSQLite.cpp
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(memberCount)
	{
		line(source, "const char* " + dataClassName + "::" + sqliteInfo.mMemberList[i].mMemberName + " = STR(" + sqliteInfo.mMemberList[i].mMemberName + ");");
	}

	writeFile(dataFilePath + dataClassName + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(dataFilePath + dataClassName + ".cpp", ANSIToUTF8(source.c_str(), true));

	// SQLiteTable.h
	string tableClassName = "SQLite" + sqliteInfo.mSQLiteName;
	string tableFileName = tableFilePath + tableClassName + ".h";
	if (!isFileExist(tableFileName))
	{
		string table;
		string tableHeaderMarco = "_SQLITE" + nameToUpper(sqliteInfo.mSQLiteName) + "_H_";
		line(table, "#ifndef " + tableHeaderMarco);
		line(table, "#define " + tableHeaderMarco);
		line(table, "");
		line(table, "#include \"" + dataClassName + ".h\"");
		line(table, "");
		line(table, "class " + tableClassName + " : public SQLiteTable<" + dataClassName + ">");
		line(table, "{");
		line(table, "public:");
		line(table, "\t" + tableClassName + "(const char* tableName, ISQLite* sqlite)");
		line(table, "\t\t:SQLiteTable(tableName, sqlite) {}");
		line(table, "};");
		line(table, "");
		line(table, "#endif", false);

		writeFile(tableFileName, ANSIToUTF8(table.c_str(), true));
	}
}

// SQLiteHeader.h文件
void CodeSQLite::generateCppSQLiteTotalHeaderFile(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	string str0;
	line(str0, "#ifndef _SQLITE_HEADER_H_");
	line(str0, "#define _SQLITE_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"SQLite.h\"");
	uint packetCount = sqliteList.size();
	FOR_I(packetCount)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(str0, "#include \"SQLite" + sqliteList[i].mSQLiteName + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "SQLiteHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// SQLiteRegister.h和SQLiteRegister.cpp文件
void CodeSQLite::generateCppSQLiteRegisteFile(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	// SQLiteRegister.h
	string str0;
	line(str0, "#ifndef _SQLITE_REGISTER_H_");
	line(str0, "#define _SQLITE_REGISTER_H_");
	line(str0, "");
	line(str0, "#include \"GameBase.h\"");
	line(str0, "");
	line(str0, "class SQLiteRegister : public GameBase");
	line(str0, "{");
	line(str0, "public:");
	line(str0, "\tstatic void registeAll();");
	line(str0, "};");
	line(str0, "");
	line(str0, "#endif", false);
	writeFile(filePath + "SQLiteRegister.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "#include \"GameHeader.h\"");
	line(str1, "");
	line(str1, "#define REGISTE_SQLITE(classType, tableName) m##classType = NEW(classType, m##classType, tableName, sqlite);sqlite->addTable(m##classType);");
	line(str1, "");
	line(str1, "void SQLiteRegister::registeAll()");
	line(str1, "{");
	line(str1, "\tSQLite* sqlite = mSQLiteManager->createSQLite(GD::SQLITE_DATA_BASE);");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(str1, "\tREGISTE_SQLITE(SQLite" + sqliteList[i].mSQLiteName + ", \"" + sqliteList[i].mSQLiteName + "\");");
	}
	line(str1, "}", false);
	writeFile(filePath + "SQLiteRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

// SQLiteInstanceDeclare.h和SQLiteInstanceDeclare.cpp
void CodeSQLite::generateCppSQLiteInstanceDeclare(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	string str0;
	line(str0, "// auto generated file, so it looks might be strange");
	line(str0, "");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(str0, "static SQLite" + sqliteList[i].mSQLiteName + "* mSQLite" + sqliteList[i].mSQLiteName + ";");
	}
	writeFile(filePath + "SQLiteInstanceDeclare.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "// auto generated file, so it looks might be strange");
	line(str1, "");
	line(str1, "#include \"GameBase.h\"");
	line(str1, "");
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(str1, "SQLite" + sqliteList[i].mSQLiteName + "* GameBase::mSQLite" + sqliteList[i].mSQLiteName + ";");
	}
	writeFile(filePath + "SQLiteInstanceDeclare.cpp", ANSIToUTF8(str1.c_str(), true));
}

// TDSQLite.cs和SQLiteTable.cs文件
void CodeSQLite::generateCSharpSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath, string tableFilePath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY)
	{
		return;
	}
	// TDSQLite.cs文件
	string file;
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
	line(file, "using Mono.Data.Sqlite;");
	line(file, "using System;");
	line(file, "using System.Collections.Generic;");
	line(file, "using UnityEngine;");
	line(file, "");
	line(file, "public class " + dataClassName + " : SQLiteData");
	line(file, "{");
	uint memberCount = sqliteInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mMemberName == "ID")
		{
			continue;
		}
		line(file, "\tpublic static string " + sqliteInfo.mMemberList[i].mMemberName + " = \"" + sqliteInfo.mMemberList[i].mMemberName + "\";");
	}
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mMemberName == "ID")
		{
			continue;
		}
		string typeName = sqliteInfo.mMemberList[i].mTypeName;
		// 因为模板文件是按照C++来写的,但是有些类型在C#中是没有的,所以要转换为C#中对应的类型
		// myVector替换为List,Vector2UShort替换为Vector2Int,char替换为sbyte
		if (startWith(typeName, "myVector"))
		{
			strReplaceAll(typeName, "myVector", "List");
		}
		else if (typeName == "Vector2UShort")
		{
			typeName = "Vector2Int";
		}
		else if (typeName == "char")
		{
			typeName = "sbyte";
		}
		if (findString(typeName.c_str(), "List", NULL))
		{
			line(file, "\tpublic " + typeName + " m" + sqliteInfo.mMemberList[i].mMemberName + " = new " + typeName + "();");
		}
		else
		{
			line(file, "\tpublic " + typeName + " m" + sqliteInfo.mMemberList[i].mMemberName + ";");
		}
	}
	line(file, "\tpublic override void parse(SqliteDataReader reader)");
	line(file, "\t{");
	line(file, "\t\tbase.parse(reader);");
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mMemberName == "ID")
		{
			continue;
		}
		line(file, "\t\tparseParam(reader, ref m" + sqliteInfo.mMemberList[i].mMemberName + ", " + sqliteInfo.mMemberList[i].mMemberName + ");");
	}
	line(file, "\t}");
	line(file, "\tpublic static void link(SQLiteTable table)");
	line(file, "\t{");
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mLinkTable.length() > 0)
		{
			line(file, "\t\ttable.link(" + sqliteInfo.mMemberList[i].mMemberName + ", mSQLite" + sqliteInfo.mMemberList[i].mLinkTable + ");");
		}
	}
	line(file, "\t}");
	line(file, "\tpublic override bool checkData()");
	line(file, "\t{");
	// 需要检测空格的字段
	myVector<string> checkValueTypeList{"bool", "byte", "short", "ushort", "int", "uint""float", "Vector2", "Vector2Int", "Vector2UShort", 
										"myVector<byte>", "myVector<short>", "myVector<ushort>", "myVector<int>", "myVector<uint>", "myVector<float>" };
	myVector<string> checkMemberList;
	FOR_I(memberCount)
	{
		const SQLiteMember& info = sqliteInfo.mMemberList[i];
		if (checkValueTypeList.contains(info.mTypeName))
		{
			checkMemberList.push_back(info.mMemberName);
		}
	}
	if (checkMemberList.size() == 1)
	{
		line(file, "\t\treturn !mValues[" + checkMemberList[0] + "].Contains(\" \");");
	}
	else
	{
		FOR_VECTOR(checkMemberList)
		{
			if (i == 0)
			{
				line(file, "\t\treturn !mValues[" + checkMemberList[i] + "].Contains(\" \") && ");
			}
			else if (i == checkMemberListCount - 1)
			{
				line(file, "\t\t\t\t!mValues[" + checkMemberList[i] + "].Contains(\" \");");
			}
			else
			{
				line(file, "\t\t\t\t!mValues[" + checkMemberList[i] + "].Contains(\" \") && ");
			}
		}
		END(colList);
	}
	line(file, "\t}");
	line(file, "}", false);
	writeFile(dataFilePath + dataClassName + ".cs", ANSIToUTF8(file.c_str(), true));

	// SQLiteTable.cs文件
	string table;
	line(table, "using System;");
	line(table, "using System.Collections.Generic;");
	line(table, "");
	string tableClassName = "SQLite" + sqliteInfo.mSQLiteName;
	line(table, "public partial class " + tableClassName + " : SQLiteTable");
	line(table, "{");
	line(table, "\tpublic override void linkTable()");
	line(table, "\t{");
	line(table, "\t\t// 之所以此处还是调用TableData的函数,是为了使链接表格的代码也跟表格结构代码一起自动生成");
	line(table, "\t\t" + dataClassName + ".link(this);");
	line(table, "\t}");
	line(table, "\tpublic " + dataClassName + " query(int id)");
	line(table, "\t{");
	line(table, "\t\treturn query(Typeof<" + dataClassName + ">(), id) as " + dataClassName + ";");
	line(table, "\t}");
	line(table, "\tpublic void queryAll(List<" + dataClassName + "> list)");
	line(table, "\t{");
	line(table, "\t\tqueryAll(Typeof<" + dataClassName + ">(), list);");
	line(table, "\t}");
	line(table, "}", false);
	writeFile(tableFilePath + tableClassName + ".cs", ANSIToUTF8(table.c_str(), true));
}

// SQLiteRegister.cs文件
void CodeSQLite::generateCSharpSQLiteRegisteFileFile(const myVector<SQLiteInfo>& sqliteInfo, string filePath)
{
	string file;
	line(file, "using System;");
	line(file, "using System.Collections.Generic;");
	line(file, "");
	line(file, "public class SQLiteRegister : GameBase");
	line(file, "{");
	line(file, "\tpublic static void registeAll()");
	line(file, "\t{");
	uint count = sqliteInfo.size();
	FOR_I(count)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY)
		{
			string lineStr = "\t\tregisteTable(out mSQLite%s, Typeof<SQLite%s>(), Typeof<TD%s>(), \"%s\");";
			replaceAll(lineStr, "%s", sqliteInfo[i].mSQLiteName);
			line(file, lineStr);
		}
	}
	line(file, "\t\tmSQLite.linkAllTable();");
	line(file, "\t}");
	line(file, "\t//-------------------------------------------------------------------------------------------------------------");
	line(file, "\tprotected static void registeTable<T>(out T sqliteTable, Type tableType, Type dataType, string tableName) where T : SQLiteTable");
	line(file, "\t{");
	line(file, "\t\tsqliteTable = mSQLite.registeTable(tableType, dataType, tableName) as T;");
	line(file, "\t}");
	line(file, "}", false);

	writeFile(filePath + "SQLiteRegister.cs", ANSIToUTF8(file.c_str(), true));
}