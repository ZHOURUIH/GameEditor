#include "CodeSQLite.h"

void CodeSQLite::generate()
{
	string cppDataPath = cppGamePath + "DataBase/SQLite/Data/";
	string cppTablePath = cppGamePath + "DataBase/SQLite/Table/";
	string csDataGamePath = csGamePath + "DataBase/Excel/Data/";
	string csDataHotFixPath = csHotfixGamePath + "DataBase/Excel/Data/";
	string csTableGamePath = csGamePath + "DataBase/Excel/Table/";
	string csTableHotFixPath = csHotfixGamePath + "DataBase/Excel/Table/";
	string csTableDeclareGamePath = csGamePath + "Common/";
	string csTableDeclareHotFixPath = csHotfixGamePath + "Common/";

	// 解析模板文件
	string fileContent;
	openTxtFile("SQLite.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("未找到表格格式文件SQLite.txt");
		return;
	}
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	if (lines.size() == 0)
	{
		return;
	}
	bool ignoreClientServer = false;
	if (lines[0] == "IgnoreClientServer")
	{
		lines.erase(0);
		ignoreClientServer = true;
	}

	bool packetStart = false;
	myVector<SQLiteInfo> sqliteInfoList;
	SQLiteInfo tempInfo;
	bool fileStart = false;
	FOR_VECTOR_CONST(lines)
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
		if (startWith(line, "//"))
		{
			string comment = line.substr(strlen("//"));
			removeStartAll(comment, ' ');
			tempInfo.mComment += comment;
			continue;
		}
		// 去除所有制表符,分号
		removeAll(line, '\t', ';');
		// 成员变量列表起始
		if (line == "{")
		{
			packetStart = true;
			string lastLine = lines[i - 1];
			int tagStartIndex = 0;
			int startIndex = -1;
			int endIndex = -1;
			// 查找标签
			myVector<string> tagList;
			while (true)
			{
				findString(lastLine.c_str(), "[", &startIndex, tagStartIndex);
				findString(lastLine.c_str(), "]", &endIndex, startIndex);
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
			if (findString(lastLine.c_str(), "[", &firstTagPos))
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
			sqliteInfoList.push_back(tempInfo);
			tempInfo.mComment = "";
			packetStart = false;
			continue;
		}
		if (packetStart)
		{
			tempInfo.mMemberList.push_back(parseSQLiteMemberLine(line, ignoreClientServer));
		}
	}
	if (cppGamePath.length() > 0)
	{
		// 删除C++的代码文件
		deleteFolder(cppDataPath);
	}
	
	if (csGamePath.length() > 0)
	{
		// 删除C#的代码文件,c#的只删除代码文件,不删除meta文件
		myVector<string> csDataFileList;
		findFiles(csDataGamePath, csDataFileList, ".cs");
		findFiles(csDataHotFixPath, csDataFileList, ".cs");
		FOR_VECTOR_CONST(csDataFileList)
		{
			deleteFile(csDataFileList[i]);
		}
		myVector<string> csTableFileList;
		findFiles(csTableGamePath, csTableFileList, ".cs");
		findFiles(csTableHotFixPath, csTableFileList, ".cs");
		FOR_VECTOR_CONST(csTableFileList)
		{
			deleteFile(csTableFileList[i]);
		}
	}

	// 生成代码文件
	FOR_VECTOR_CONST(sqliteInfoList)
	{
		if (cppGamePath.length() > 0)
		{
			// .h代码
			generateCppSQLiteDataFile(sqliteInfoList[i], cppDataPath);
			generateCppSQLiteTableFile(sqliteInfoList[i], cppTablePath);
		}
		if (csGamePath.length() > 0)
		{
			// .cs代码
			generateCSharpExcelDataFile(sqliteInfoList[i], csDataGamePath, csDataHotFixPath);
			generateCSharpExcelTableFile(sqliteInfoList[i], csTableGamePath, csTableHotFixPath);
		}
	}

	if (cppGamePath.length() > 0)
	{
		// 在上一层目录生成SQLiteHeader.h文件
		string headerPath = getFilePath(cppDataPath) + "/";
		generateCppSQLiteTotalHeaderFile(sqliteInfoList, headerPath);
		generateCppSQLiteRegisteFile(sqliteInfoList, headerPath);
		generateCppSQLiteInstanceDeclare(sqliteInfoList, headerPath);
		generateCppSQLiteSTLPoolRegister(sqliteInfoList, headerPath);
		generateCppSQLiteInstanceClear(sqliteInfoList, headerPath);
	}

	if (csGamePath.length() > 0)
	{
		// 在上一层目录生成SQLiteRegister.cs文件
		generateCSharpExcelRegisteFileFile(sqliteInfoList, getFilePath(csDataHotFixPath) + "/", getFilePath(csDataGamePath) + "/");
		generateCSharpExcelDeclare(sqliteInfoList, csTableDeclareHotFixPath, csTableDeclareGamePath);
	}
}

// TDSQLite.h和TDSQLite.cpp文件
void CodeSQLite::generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, const string& dataFilePath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::CLIENT_ONLY || sqliteInfo.mOwner == SQLITE_OWNER::NONE)
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
	line(header, "// " + sqliteInfo.mComment);
	line(header, "class " + dataClassName + " : public SQLiteData");
	line(header, "{");
	line(header, "public:");
	uint memberCount = sqliteInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(header, "\tstatic constexpr const char* " + sqliteInfo.mMemberList[i].mMemberName + " = STR(" + sqliteInfo.mMemberList[i].mMemberName + ");");
	}
	line(header, "public:");
	FOR_I(memberCount)
	{
		const SQLiteMember& member = sqliteInfo.mMemberList[i];
		if (member.mOwner == SQLITE_OWNER::SERVER_ONLY || member.mOwner == SQLITE_OWNER::BOTH)
		{
			string memberLine = "\t" + member.mTypeName + " m" + member.mMemberName + ";";
			uint tabCount = generateAlignTableCount(memberLine, 40);
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
	FOR_I(memberCount)
	{
		const SQLiteMember& member = sqliteInfo.mMemberList[i];
		const string& name = member.mMemberName;
		if (member.mOwner == SQLITE_OWNER::SERVER_ONLY || member.mOwner == SQLITE_OWNER::BOTH)
		{
			line(header, "\t\tregisteParam(m" + name + ", " + name + ");");
		}
		else
		{
			line(header, "\t\tregisteParamEmpty(" + name + ");");
		}
	}
	line(header, "\t}");
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	// TDSQLite.cpp
	string source;
	line(source, "#include \"" + dataClassName + ".h\"");
	line(source, "");
	FOR_I(memberCount)
	{
		line(source, "constexpr const char* " + dataClassName + "::" + sqliteInfo.mMemberList[i].mMemberName + ";");
	}

	writeFile(dataFilePath + dataClassName + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(dataFilePath + dataClassName + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// SQLiteTable.h文件
void CodeSQLite::generateCppSQLiteTableFile(const SQLiteInfo& sqliteInfo, const string& tableFilePath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::CLIENT_ONLY || sqliteInfo.mOwner == SQLITE_OWNER::NONE)
	{
		return;
	}
	// SQLiteTable.h
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
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
		line(table, "#include \"SQLiteTable.h\"");
		line(table, "");
		line(table, "class " + tableClassName + " : public SQLiteTable<" + dataClassName + ">");
		line(table, "{");
		line(table, "public:");
		line(table, "};");
		line(table, "");
		line(table, "#endif", false);

		writeFile(tableFileName, ANSIToUTF8(table.c_str(), true));
	}
}

// SQLiteHeader.h文件
void CodeSQLite::generateCppSQLiteTotalHeaderFile(const myVector<SQLiteInfo>& sqliteList, const string& filePath)
{
	string str0;
	line(str0, "#ifndef _SQLITE_HEADER_H_");
	line(str0, "#define _SQLITE_HEADER_H_");
	line(str0, "");
	uint packetCount = sqliteList.size();
	FOR_I(packetCount)
	{
		if (sqliteList[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteList[i].mOwner != SQLITE_OWNER::BOTH)
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
void CodeSQLite::generateCppSQLiteRegisteFile(const myVector<SQLiteInfo>& sqliteList, const string& filePath)
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
	line(str1, "#define REGISTE_SQLITE(classType, tableName) m##classType = NEW(classType, m##classType);mSQLiteManager->addSQLiteTable(m##classType, tableName)");
	line(str1, "");
	line(str1, "void SQLiteRegister::registeAll()");
	line(str1, "{");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteList[i].mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		line(str1, "\tREGISTE_SQLITE(SQLite" + sqliteList[i].mSQLiteName + ", \"" + sqliteList[i].mSQLiteName + "\");");
	}
	line(str1, "}", false);
	writeFile(filePath + "SQLiteRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

// SQLiteInstanceDeclare.h和SQLiteInstanceDeclare.cpp
void CodeSQLite::generateCppSQLiteInstanceDeclare(const myVector<SQLiteInfo>& sqliteList, const string& filePath)
{
	string header;
	line(header, "#ifdef _SQLITE_INSTANCE_DECLARE_H_");
	line(header, "#error \"特殊头文件,只能被GameBase所包含\"");
	line(header, "#else");
	line(header, "#define _SQLITE_INSTANCE_DECLARE_H_");
	line(header, "");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteList[i].mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		line(header, "static SQLite" + sqliteList[i].mSQLiteName + "* mSQLite" + sqliteList[i].mSQLiteName + ";");
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "SQLiteInstanceDeclare.h", ANSIToUTF8(header.c_str(), true));

	string source;
	line(source, "");
	line(source, "#include \"GameBase.h\"");
	line(source, "");
	FOR_I(count)
	{
		if (sqliteList[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteList[i].mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		line(source, "SQLite" + sqliteList[i].mSQLiteName + "* GameBase::mSQLite" + sqliteList[i].mSQLiteName + ";");
	}
	writeFile(filePath + "SQLiteInstanceDeclare.cpp", ANSIToUTF8(source.c_str(), true));
}

// SQLiteSTLPoolRegister.h
void CodeSQLite::generateCppSQLiteSTLPoolRegister(const myVector<SQLiteInfo>& sqliteList, const string& filePath)
{
	string header;
	line(header, "#ifdef _SQLITE_STL_POOL_REGISTER_H_");
	line(header, "#error \"特殊头文件,只能被GameSTLPoolRegister.cpp所包含\"");
	line(header, "#else");
	line(header, "#define _SQLITE_STL_POOL_REGISTER_H_");
	line(header, "");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteList[i].mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		line(header, "REGISTE_VECTOR_POOL(TD" + sqliteList[i].mSQLiteName + "*);");
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "SQLiteSTLPoolRegister.h", ANSIToUTF8(header.c_str(), true));
}

void CodeSQLite::generateCppSQLiteInstanceClear(const myVector<SQLiteInfo>& sqliteList, const string& filePath)
{
	string header;
	line(header, "#ifdef _SQLITE_INSTANCE_CLEAR_H_");
	line(header, "#error \"特殊头文件,只能被GameBase所包含\"");
	line(header, "#else");
	line(header, "#define _SQLITE_INSTANCE_CLEAR_H_");
	line(header, "");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteList[i].mOwner != SQLITE_OWNER::BOTH)
		{
			continue;
		}
		line(header, "mSQLite" + sqliteList[i].mSQLiteName + " = nullptr;");
	}
	line(header, "");
	line(header, "#endif", false);
	writeFile(filePath + "SQLiteInstanceClear.h", ANSIToUTF8(header.c_str(), true));
}

// TDExcel.cs文件
void CodeSQLite::generateCSharpExcelDataFile(const SQLiteInfo& sqliteInfo, const string& dataFileGamePath, const string& dataFileHotFixPath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY || sqliteInfo.mOwner == SQLITE_OWNER::NONE)
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
		string typeName = member.mTypeName;
		// 因为模板文件是按照C++来写的,但是有些类型在C#中是没有的,所以要转换为C#中对应的类型
		// myVector替换为List,char替换为sbyte
		if (startWith(typeName, "Vector<"))
		{
			strReplaceAll(typeName, "Vector<", "List<");
		}
		else if (typeName == "char")
		{
			typeName = "sbyte";
		}
		else if (typeName == "llong")
		{
			typeName = "long";
		}

		// 列表类型的成员变量存储到单独的列表,因为需要分配内存
		if (findString(typeName.c_str(), "List", NULL))
		{
			listMemberList.push_back(make_pair(typeName, member.mMemberName));
			listMemberSet.insert(member.mMemberName);
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

		string memberLine = "\t" + publicType + " " + typeName + " m" + member.mMemberName + ";";
		uint tabCount = generateAlignTableCount(memberLine, 44);
		FOR_I(tabCount)
		{
			memberLine += '\t';
		}
		memberLine += "// " + sqliteInfo.mMemberList[i].mComment;
		line(file, memberLine);
	}
	if (listMemberList.size() > 0)
	{
		line(file, "\tpublic " + dataClassName + "()");
		line(file, "\t{");
		FOR_VECTOR(listMemberList)
		{
			line(file, "\t\tm" + listMemberList[i].second + " = new " + listMemberList[i].first + "();");
		}
		END(listMemberList);
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
		if (memberInfo.mTypeName == "string")
		{
			line(file, "\t\treader.readString(out m" + memberInfo.mMemberName + ");");
		}
		else if (listMemberSet.contains(memberInfo.mMemberName))
		{
			line(file, "\t\treader.read(m" + memberInfo.mMemberName + ");");
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
	if (sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY || sqliteInfo.mOwner == SQLITE_OWNER::NONE)
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
	line(table, "\tprotected List<" + dataClassName + "> mDataList;");
	line(table, "\tprotected bool mDataAvailable;");
	line(table, "\tpublic " + tableClassName + "()");
	line(table, "\t{");
	line(table, "\t\tmDataList = new List<" + dataClassName + ">();");
	line(table, "\t\tmDataAvailable = false;");
	line(table, "\t}");
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

// ExcelRegister.cs文件
void CodeSQLite::generateCSharpExcelRegisteFileFile(const myVector<SQLiteInfo>& sqliteInfo, const string& fileHotFixPath, const string& fileGamePath)
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
	uint count = sqliteInfo.size();
	FOR_I(count)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteInfo[i].mOwner != SQLITE_OWNER::NONE && !sqliteInfo[i].mHotFix)
		{
			string lineStr = "\t\tregisteTable(out mExcel%s, typeof(ED%s), \"%s\");";
			replaceAll(lineStr, "%s", sqliteInfo[i].mSQLiteName);
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

	// 热更工程中的表格注册
	string hotFixfile;
	line(hotFixfile, "using System;");
	line(hotFixfile, "using static GBR;");
	line(hotFixfile, "using static FrameBase;");
	line(hotFixfile, "");
	line(hotFixfile, "public class ExcelRegisterILR");
	line(hotFixfile, "{");
	line(hotFixfile, "\tpublic static void registeAll()");
	line(hotFixfile, "\t{");
	FOR_I(count)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteInfo[i].mOwner != SQLITE_OWNER::NONE && sqliteInfo[i].mHotFix)
		{
			string lineStr = "\t\tregisteTable(out mExcel%s, typeof(ED%s), \"%s\");";
			replaceAll(lineStr, "%s", sqliteInfo[i].mSQLiteName);
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

	writeFile(fileHotFixPath + "ExcelRegisterILR.cs", ANSIToUTF8(hotFixfile.c_str(), true));
}

// GameBaseExcel.cs文件
void CodeSQLite::generateCSharpExcelDeclare(const myVector<SQLiteInfo>& sqliteInfo, const string& fileHotFixPath, const string& fileGamePath)
{
	// 主工程中的表格注册
	string mainFile;
	line(mainFile, "using System;");
	line(mainFile, "");
	line(mainFile, "// GameBase的部分类,用于定义Excel表格的对象");
	line(mainFile, "public partial class GB");
	line(mainFile, "{");
	uint sqliteCount = sqliteInfo.size();
	FOR_I(sqliteCount)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteInfo[i].mOwner != SQLITE_OWNER::NONE && !sqliteInfo[i].mHotFix)
		{
			line(mainFile, "\tpublic static Excel" + sqliteInfo[i].mSQLiteName + " mExcel" + sqliteInfo[i].mSQLiteName + ";");
		}
	}
	line(mainFile, "}", false);

	writeFile(fileGamePath + "GameBaseExcel.cs", ANSIToUTF8(mainFile.c_str(), true));

	// 热更工程中的表格注册
	string hotFixfile;
	line(hotFixfile, "using System;");
	line(hotFixfile, "");
	line(hotFixfile, "// FrameBase的部分类,用于定义Excel表格的对象");
	line(hotFixfile, "public partial class GBR");
	line(hotFixfile, "{");
	FOR_I(sqliteCount)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteInfo[i].mOwner != SQLITE_OWNER::NONE && sqliteInfo[i].mHotFix)
		{
			line(hotFixfile, "\tpublic static Excel" + sqliteInfo[i].mSQLiteName + " mExcel" + sqliteInfo[i].mSQLiteName + ";");
		}
	}
	line(hotFixfile, "}", false);

	writeFile(fileHotFixPath + "GameBaseExcelILR.cs", ANSIToUTF8(hotFixfile.c_str(), true));
}