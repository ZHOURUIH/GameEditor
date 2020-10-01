#include "CodeMySQL.h"

void CodeMySQL::generateMySQLCode(string cppDataPath, string cppStringDefinePath)
{
	// 解析模板文件
	string fileContent;
	openTxtFile("MySQL.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("未找到表格格式文件MySQL.txt");
		return;
	}
	fileContent = UTF8ToANSI(fileContent.c_str(), true);
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	bool packetStart = false;
	myVector<MySQLInfo> mysqlInfoList;
	MySQLInfo tempInfo;
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
			tempInfo.mMySQLName = lines[i - 1];
			tempInfo.mMemberList.clear();
			continue;
		}
		// 成员变量列表结束
		if (line == "}")
		{
			mysqlInfoList.push_back(tempInfo);
			packetStart = false;
			continue;
		}
		if (packetStart)
		{
			tempInfo.mMemberList.push_back(parseMySQLMemberLine(line));
		}
	}
	deleteFolder(cppDataPath);
	myVector<string> mysqlList;
	FOR_VECTOR_CONST(mysqlInfoList)
	{
		// 生成代码文件
		generateCppMySQLDataFile(mysqlInfoList[i], cppDataPath);
		mysqlList.push_back(mysqlInfoList[i].mMySQLName);
	}
	// 上一层目录生成MySQLHeader.h
	string totalHeaderPath = cppDataPath;
	if (endWith(totalHeaderPath, "/") || endWith(totalHeaderPath, "\\"))
	{
		totalHeaderPath = totalHeaderPath.substr(0, totalHeaderPath.length() - 1);
	}
	totalHeaderPath = getFilePath(totalHeaderPath);
	generateCppMySQLTotalHeaderFile(mysqlInfoList, totalHeaderPath);
	generateCppMySQLRegisteFile(mysqlInfoList, totalHeaderPath);
	generateStringDefineMySQL(mysqlList, cppStringDefinePath);
	generateMySQLClassDeclare(mysqlList, totalHeaderPath);
	generateMySQLInstanceDeclare(mysqlList, totalHeaderPath);
}

// 生成MySQLData.h和MySQLData.cpp文件
void CodeMySQL::generateCppMySQLDataFile(const MySQLInfo& mysqlInfo, string filePath)
{
	// 头文件
	string header;
	string className = "MD" + mysqlInfo.mMySQLName;
	string headerMacro = "_MD" + nameToUpper(mysqlInfo.mMySQLName) + "_H_";
	line(header, "#ifndef " + headerMacro);
	line(header, "#define " + headerMacro);
	line(header, "");
	line(header, "#include \"MySQLData.h\"");
	line(header, "");
	line(header, "class MySQLTable;");
	line(header, "class " + className + " : public MySQLData");
	line(header, "{");
	line(header, "public:");
	uint memberCount = mysqlInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(header, "\tCOL(" + mysqlInfo.mMemberList[i].mTypeName + ", " + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	line(header, "public:");
	line(header, "\tstatic void fillColName(MySQLTable* table);");
	line(header, "\tvoid resultRowToTableData(myMap<const char*, char*>& resultRow) override;");
	line(header, "\tvoid paramList(char* params, uint size) const override;");
	line(header, "\tvoid resetProperty() override;");
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	// 源文件
	string source;
	line(source, "#include \"" + className + ".h\"");
	line(source, "#include \"Utility.h\"");
	line(source, "#include \"MySQLTable.h\"");
	line(source, "");
	// 字段静态变量定义
	FOR_I(memberCount)
	{
		line(source, "COL_DEFINE(" + className + ", " + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	// fillColName函数
	line(source, "");
	line(source, "void " + className + "::fillColName(MySQLTable* table)");
	line(source, "{");
	FOR_I(memberCount)
	{
		line(source, "\ttable->addColName(" + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	line(source, "}");
	line(source, "");
	// resultRowToTableData函数
	line(source, "void " + className + "::resultRowToTableData(myMap<const char*, char*>& resultRow)");
	line(source, "{");
	FOR_I(memberCount)
	{
		line(source, "\tPARSE(" + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	line(source, "}");
	line(source, "");
	// paramList函数
	line(source, "void " + className + "::paramList(char* params, uint size) const");
	line(source, "{");
	FOR_I(memberCount)
	{
		if (i != memberCount - 1)
		{
			if (mysqlInfo.mMemberList[i].mTypeName == "string")
			{
				string isUTF8Str = mysqlInfo.mMemberList[i].mUTF8 ? "true" : "false";
				line(source, "\tAPPEND_STRING(" + mysqlInfo.mMemberList[i].mMemberName + ", " + isUTF8Str + ");");
			}
			else
			{
				line(source, "\tAPPEND_VALUE(" + mysqlInfo.mMemberList[i].mMemberName + ");");
			}
		}
		else
		{
			if (mysqlInfo.mMemberList[i].mTypeName == "string")
			{
				string isUTF8Str = mysqlInfo.mMemberList[i].mUTF8 ? "true" : "false";
				line(source, "\tAPPEND_STRING_END(" + mysqlInfo.mMemberList[i].mMemberName + ", " + isUTF8Str + ");");
			}
			else
			{
				line(source, "\tAPPEND_VALUE_END(" + mysqlInfo.mMemberList[i].mMemberName + ");");
			}
		}
	}
	line(source, "}");
	line(source, "");
	// resetProperty函数
	line(source, "void " + className + "::resetProperty()");
	line(source, "{");
	line(source, "\tMySQLData::resetProperty();");
	FOR_I(memberCount)
	{
		if (mysqlInfo.mMemberList[i].mTypeName == "string")
		{
			line(source, "\tm" + mysqlInfo.mMemberList[i].mMemberName + ".clear();");
		}
		else if (mysqlInfo.mMemberList[i].mTypeName == "float")
		{
			line(source, "\tm" + mysqlInfo.mMemberList[i].mMemberName + " = 0.0f;");
		}
		else
		{
			line(source, "\tm" + mysqlInfo.mMemberList[i].mMemberName + " = 0;");
		}
	}
	line(source, "}", false);
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + className + ".h", header);
	writeFile(filePath + className + ".cpp", source);
}

// MySQLHeader.h文件
void CodeMySQL::generateCppMySQLTotalHeaderFile(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	string str0;
	line(str0, "#ifndef _MYSQL_HEADER_H_");
	line(str0, "#define _MYSQL_HEADER_H_");
	line(str0, "");
	uint packetCount = mysqlList.size();
	FOR_I(packetCount)
	{
		line(str0, "#include \"MD" + mysqlList[i].mMySQLName + ".h\"");
	}
	line(str0, "");
	FOR_I(packetCount)
	{
		line(str0, "#include \"MySQL" + mysqlList[i].mMySQLName + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "MySQLHeader.h", str0);
}

// MySQLRegiste.h和MySQLRegiste.cpp文件
void CodeMySQL::generateCppMySQLRegisteFile(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	// MySQLRegiste.h
	string str0;
	line(str0, "#ifndef _MYSQL_REGISTER_H_");
	line(str0, "#define _MYSQL_REGISTER_H_");
	line(str0, "");
	line(str0, "#include \"GameBase.h\"");
	line(str0, "");
	line(str0, "class MySQLRegister : public GameBase");
	line(str0, "{");
	line(str0, "public:");
	line(str0, "\tstatic void registeAll();");
	line(str0, "};");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "MySQLRegister.h", str0);

	string str1;
	line(str1, "#include \"MySQLRegister.h\"");
	line(str1, "#include \"MySQLDataBase.h\"");
	line(str1, "#include \"MySQLHeader.h\"");
	line(str1, "");
	line(str1, "#define REGISTE_MYSQL(classType, tableName) m##classType = mMySQLDataBase->registeTable<classType>(tableName);");
	line(str1, "");
	line(str1, "void MySQLRegister::registeAll()");
	line(str1, "{");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(str1, "\tREGISTE_MYSQL(MySQL" + mysqlList[i].mMySQLName + ", \"" + mysqlList[i].mMySQLName + "\");");
	}
	line(str1, "}", false);
	str1 = ANSIToUTF8(str1.c_str(), true);
	writeFile(filePath + "MySQLRegister.cpp", str1);
}

// StringDefineMySQL.h和StringDefineMySQL.cpp
void CodeMySQL::generateStringDefineMySQL(const myVector<string>& mysqlList, string filePath)
{
	// 头文件
	string header;
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(header, "DECLARE_STRING(MD" + mysqlList[i] + ");");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineMySQL.h", header);

	// 源文件
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"MySQLHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, "DEFINE_STRING(MD" + mysqlList[i] + ");");
	}
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineMySQL.cpp", source);
}

// MySQLClassDeclare.h
void CodeMySQL::generateMySQLClassDeclare(const myVector<string>& mysqlList, string filePath)
{
	// 头文件
	string header;
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(header, "class MySQL" + mysqlList[i] + ";");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "MySQLClassDeclare.h", header);
}

// MySQLInstanceDeclare.h和MySQLInstanceDeclare.cpp
void CodeMySQL::generateMySQLInstanceDeclare(const myVector<string>& mysqlList, string filePath)
{
	// 头文件
	string header;
	line(header, "// auto generated file, so it looks might be strange");
	line(header, "");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(header, "static MySQL" + mysqlList[i] + "* mMySQL" + mysqlList[i] + ";");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "MySQLInstanceDeclare.h", header);

	string cpp;
	line(cpp, "// auto generated file, so it looks might be strange");
	line(cpp, "");
	line(cpp, "#include \"GameBase.h\"");
	line(cpp, "");
	FOR_I(count)
	{
		line(cpp, "MySQL" + mysqlList[i] + "* GameBase::mMySQL" + mysqlList[i] + ";");
	}
	cpp = ANSIToUTF8(cpp.c_str(), true);
	writeFile(filePath + "MySQLInstanceDeclare.cpp", cpp);
}