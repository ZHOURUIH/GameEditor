#include "CodeMySQL.h"

void CodeMySQL::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppDataPath = cppGamePath + "DataBase/MySQL/Data/";
	string cppTablePath = cppGamePath + "DataBase/MySQL/Table/";

	// 解析模板文件
	string fileContent;
	openTxtFile("MySQL.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("未找到表格格式文件MySQL.txt");
		return;
	}
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	bool packetStart = false;
	myVector<MySQLInfo> mysqlInfoList;
	MySQLInfo tempInfo;
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
			string tableTitle = lines[i - 1];
			removeAll(tableTitle, '\t', ';');
			myVector<string> titleVector;
			split(tableTitle.c_str(), ":", titleVector);
			if (titleVector.size() != 2)
			{
				ERROR("mysql表格的格式错误: " + lines[i - 1]);
				return;
			}
			tempInfo.mMySQLClassName = titleVector[0];
			tempInfo.mMySQLTableName = titleVector[1];
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
	FOR_VECTOR_CONST(mysqlInfoList)
	{
		// 生成代码文件
		generateCppMySQLDataFile(mysqlInfoList[i], cppDataPath);
		generateCppMySQLTableFile(mysqlInfoList[i], cppTablePath);
	}
	// 上一层目录生成MySQLHeader.h
	string totalHeaderPath = cppDataPath;
	if (endWith(totalHeaderPath, "/") || endWith(totalHeaderPath, "\\"))
	{
		totalHeaderPath = totalHeaderPath.substr(0, totalHeaderPath.length() - 1);
	}
	totalHeaderPath = getFilePath(totalHeaderPath) + "/";
	generateCppMySQLTotalHeaderFile(mysqlInfoList, totalHeaderPath);
	generateCppMySQLRegisteFile(mysqlInfoList, totalHeaderPath);
	generateStringDefineMySQL(mysqlInfoList, cppStringDefinePath);
	generateMySQLInstanceDeclare(mysqlInfoList, totalHeaderPath);
	generateMySQLInstanceClear(mysqlInfoList, totalHeaderPath);
}

// 生成MySQLData.h和MySQLData.cpp文件
void CodeMySQL::generateCppMySQLDataFile(const MySQLInfo& mysqlInfo, string filePath)
{
	// 头文件
	string header;
	string className = "MD" + mysqlInfo.mMySQLClassName;
	string headerMacro = "_MD" + nameToUpper(mysqlInfo.mMySQLClassName) + "_H_";
	line(header, "#ifndef " + headerMacro);
	line(header, "#define " + headerMacro);
	line(header, "");
	line(header, "#include \"MySQLData.h\"");
	line(header, "");
	line(header, "// " + mysqlInfo.mComment);
	line(header, "class " + className + " : public MySQLData");
	line(header, "{");
	line(header, "\tBASE_CLASS(MySQLData);");
	line(header, "public:");
	uint memberCount = mysqlInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(header, "\tstatic const char* " + mysqlInfo.mMemberList[i].mMemberName + ";");
	}
	line(header, "public:");
	FOR_I(memberCount)
	{
		string memberLine = "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + ";";
		uint tabCount = generateAlignTableCount(memberLine, 40);
		FOR_I(tabCount)
		{
			memberLine += '\t';
		}
		memberLine += "// " + mysqlInfo.mMemberList[i].mComment;
		line(header, memberLine);
	}
	line(header, "public:");
	line(header, "\tstatic void fillColName(MySQLTable* table);");
	line(header, "\tvoid parseResult(Map<const char*, char*>& resultRow) override;");
	line(header, "\tvoid paramList(Array<1024>& params) const override;");
	line(header, "\tvoid clone(MySQLData* target) const override;");
	line(header, "\tvoid cloneWithFlag(MySQLData* target, llong flag) const override;");
	line(header, "\tvoid resetProperty() override;");
	line(header, "\tbool updateBool(bool value, int index) override;");
	line(header, "\tbool updateInt(int value, int index) override;");
	line(header, "\tbool updateFloat(float value, int index) override;");
	line(header, "\tbool updateLLong(llong value, int index) override;");
	line(header, "\tbool updateString(const string& value, int index) override;");
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	// 字段静态变量定义
	FOR_I(memberCount)
	{
		line(source, "const char* " + className + "::" + mysqlInfo.mMemberList[i].mMemberName + " = STR(" + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	// fillColName函数
	line(source, "");
	line(source, "void " + className + "::fillColName(MySQLTable* table)");
	line(source, "{");
	line(source, "\ttable->addColName(ID, 0);");
	FOR_I(memberCount)
	{
		line(source, "\ttable->addColName(" + mysqlInfo.mMemberList[i].mMemberName + ", " + intToString(i + 1) + ");");
	}
	line(source, "}");
	line(source, "");
	// parseResult函数
	line(source, "void " + className + "::parseResult(Map<const char*, char*>& resultRow)");
	line(source, "{");
	line(source, "\tparseLLong(mID, resultRow.get(ID, nullptr));");
	FOR_I(memberCount)
	{
		const string& typeName = mysqlInfo.mMemberList[i].mTypeName;
		const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
		if (typeName == "int")
		{
			line(source, "\tparseInt(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "uint")
		{
			line(source, "\tparseUInt(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "bool")
		{
			line(source, "\tparseBool(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "byte")
		{
			line(source, "\tparseByte(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "char")
		{
			line(source, "\tparseChar(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "short")
		{
			line(source, "\tparseShort(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "ushort")
		{
			line(source, "\tparseUShort(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "float")
		{
			line(source, "\tparseFloat(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "llong")
		{
			line(source, "\tparseLLong(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "string")
		{
			line(source, "\tparseString(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
	}
	line(source, "}");
	line(source, "");

	// paramList函数
	line(source, "void " + className + "::paramList(Array<1024>& params) const");
	line(source, "{");
	line(source, "\tappendValueLLong(params, mID, " + (memberCount > 0 ? string("true") : string("false")) + ");");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		string addComma = i != memberCount - 1 ? "true" : "false";
		if (typeName == "string")
		{
			if (memberInfo.mUTF8)
			{
				line(source, "\tappendValueStringUTF8(params, m" + memberName + ".c_str(), " + addComma + ");");
			}
			else
			{
				line(source, "\tappendValueString(params, m" + memberName + ".c_str(), " + addComma + ");");
			}
		}
		else if (typeName == "int")
		{
			line(source, "\tappendValueInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "uint")
		{
			line(source, "\tappendValueInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "bool")
		{
			line(source, "\tappendValueInt(params, m" + memberName + " ? 1 : 0, " + addComma + ");");
		}
		else if (typeName == "byte")
		{
			line(source, "\tappendValueInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "char")
		{
			line(source, "\tappendValueInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "short")
		{
			line(source, "\tappendValueInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "ushort")
		{
			line(source, "\tappendValueInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "float")
		{
			line(source, "\tappendValueFloat(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "llong")
		{
			line(source, "\tappendValueLLong(params, m" + memberName + ", " + addComma + ");");
		}
	}
	line(source, "}");
	line(source, "");

	// clone函数
	line(source, "void " + className + "::clone(MySQLData* target) const");
	line(source, "{");
	line(source, "\tbase::clone(target);");
	if (memberCount > 0)
	{
		line(source, "\tauto targetData = CAST<" + className + "*>(target);");
		FOR_I(memberCount)
		{
			const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
			line(source, "\ttargetData->m" + memberName + " = m" + memberName + ";");
		}
	}
	line(source, "}");
	line(source, "");

	// cloneWithFlag函数
	line(source, "void " + className + "::cloneWithFlag(MySQLData* target, llong flag) const");
	line(source, "{");
	line(source, "\tbase::cloneWithFlag(target, flag);");
	if (memberCount > 0)
	{
		line(source, "\tauto targetData = CAST<" + className + "*>(target);");
		FOR_I(memberCount)
		{
			const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
			line(source, "\tif (hasBit(flag, " + intToString(i + 1) + "))");
			line(source, "\t{");
			line(source, "\t\ttargetData->m" + memberName + " = m" + memberName + ";");
			line(source, "\t}");
		}
	}
	line(source, "}");
	line(source, "");

	// resetProperty函数
	line(source, "void " + className + "::resetProperty()");
	line(source, "{");
	line(source, "\tbase::resetProperty();");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "string")
		{
			line(source, "\tm" + memberName + ".clear();");
		}
		else if (typeName == "float")
		{
			line(source, "\tm" + memberName + " = 0.0f;");
		}
		else
		{
			line(source, "\tm" + memberName + " = 0;");
		}
	}
	line(source, "}");
	line(source, "");

	// updateBool函数
	line(source, "bool " + className + "::updateBool(bool value, int index)");
	line(source, "{");
	line(source, "\tbase::updateBool(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "bool")
		{
			line(source, "\tcase " + intToString(i + 1) + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + intToString(i + 1) + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateInt函数
	line(source, "bool " + className + "::updateInt(int value, int index)");
	line(source, "{");
	line(source, "\tbase::updateInt(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "int" || typeName == "ushort" || typeName == "short" || typeName == "byte" || typeName == "char")
		{
			line(source, "\tcase " + intToString(i + 1) + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + intToString(i + 1) + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateFloat函数
	line(source, "bool " + className + "::updateFloat(float value, int index)");
	line(source, "{");
	line(source, "\tbase::updateFloat(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "float")
		{
			line(source, "\tcase " + intToString(i + 1) + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + intToString(i + 1) + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateLLong函数
	line(source, "bool " + className + "::updateLLong(llong value, int index)");
	line(source, "{");
	line(source, "\tbase::updateLLong(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "llong")
		{
			line(source, "\tcase " + intToString(i + 1) + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + intToString(i + 1) + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateString函数
	line(source, "bool " + className + "::updateString(const string& value, int index)");
	line(source, "{");
	line(source, "\tbase::updateString(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "string")
		{
			line(source, "\tcase " + intToString(i + 1) + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + intToString(i + 1) + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}", false);

	writeFile(filePath + className + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(filePath + className + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// 生成MySQLTable.h和MySQLTable.cpp文件
void CodeMySQL::generateCppMySQLTableFile(const MySQLInfo& mysqlInfo, string filePath)
{
	string tableClassName = "MySQL" + mysqlInfo.mMySQLClassName;
	string dataClassName = "MD" + mysqlInfo.mMySQLClassName;

	// 头文件
	string header;
	string headerMacro = "_MYSQL" + nameToUpper(mysqlInfo.mMySQLClassName) + "_H_";
	line(header, "#ifndef " + headerMacro);
	line(header, "#define " + headerMacro);
	line(header, "");
	line(header, "#include \"MySQLTable.h\"");
	line(header, "");
	line(header, "class " + tableClassName + " : public MySQLTable");
	line(header, "{");
	line(header, "\tBASE_CLASS(MySQLTable);");
	line(header, "public:");
	line(header, "\t" + tableClassName + "(const char* tableName)");
	line(header, "\t\t:MySQLTable(tableName) {}");
	line(header, "\tvoid init(MYSQL * mysql) override;");
	line(header, "\tMySQLData* createData() override;");
	line(header, "protected:");
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void " + tableClassName + "::init(MYSQL* mysql)");
	line(source, "{");
	line(source, "\tMySQLTable::init(mysql);");
	line(source, "\t" + dataClassName + "::fillColName(this);");
	line(source, "}");
	line(source, "");
	line(source, "MySQLData* " + tableClassName + "::createData()");
	line(source, "{");
	line(source, "\treturn mMySQLDataBase->createData<" + dataClassName + ">(NAME(" + dataClassName + "));");
	line(source, "}", false);

	writeFile(filePath + tableClassName + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(filePath + tableClassName + ".cpp", ANSIToUTF8(source.c_str(), true));
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
		line(str0, "#include \"MD" + mysqlList[i].mMySQLClassName + ".h\"");
	}
	line(str0, "");
	FOR_I(packetCount)
	{
		line(str0, "#include \"MySQL" + mysqlList[i].mMySQLClassName + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "MySQLHeader.h", ANSIToUTF8(str0.c_str(), true));
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
	writeFile(filePath + "MySQLRegister.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "#include \"GameHeader.h\"");
	line(str1, "");
	line(str1, "#define REGISTE_MYSQL(classType, tableName) m##classType = mMySQLDataBase->registeTable<classType>(tableName);");
	line(str1, "");
	line(str1, "void MySQLRegister::registeAll()");
	line(str1, "{");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(str1, "\tREGISTE_MYSQL(MySQL" + mysqlList[i].mMySQLClassName + ", \"" + mysqlList[i].mMySQLTableName + "\");");
	}
	line(str1, "}", false);
	writeFile(filePath + "MySQLRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

// StringDefineMySQL.h和StringDefineMySQL.cpp
void CodeMySQL::generateStringDefineMySQL(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	// 头文件
	string header;
	line(header, "#ifdef _STRING_DEFINE_MYSQL_H_");
	line(header, "#error \"特殊头文件,只能在StringDefine.h中被包含\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_MYSQL_H_");
	line(header, "");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(header, stringDeclare("MD" + mysqlList[i].mMySQLClassName));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefineMySQL.h", ANSIToUTF8(header.c_str(), true));

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine("MD" + mysqlList[i].mMySQLClassName));
	}
	writeFile(filePath + "StringDefineMySQL.cpp", ANSIToUTF8(source.c_str(), true));
}

// MySQLInstanceDeclare.h和MySQLInstanceDeclare.cpp
void CodeMySQL::generateMySQLInstanceDeclare(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	// 头文件
	string header;
	line(header, "#ifdef _MYSQL_INSTANCE_DECLARE_H_");
	line(header, "#error \"特殊头文件,只能被GameBase所包含\"");
	line(header, "#else");
	line(header, "#define _MYSQL_INSTANCE_DECLARE_H_");
	line(header, "");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(header, "static MySQL" + mysqlList[i].mMySQLClassName + "* mMySQL" + mysqlList[i].mMySQLClassName + ";");
	}
	line(header, "");
	line(header, "#endif", false);
	writeFile(filePath + "MySQLInstanceDeclare.h", ANSIToUTF8(header.c_str(), true));

	string cpp;
	line(cpp, "");
	line(cpp, "#include \"GameBase.h\"");
	line(cpp, "");
	FOR_I(count)
	{
		line(cpp, "MySQL" + mysqlList[i].mMySQLClassName + "* GameBase::mMySQL" + mysqlList[i].mMySQLClassName + ";");
	}
	writeFile(filePath + "MySQLInstanceDeclare.cpp", ANSIToUTF8(cpp.c_str(), true));
}

// MySQLInstanceClear.h
void CodeMySQL::generateMySQLInstanceClear(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	string header;
	line(header, "#ifdef _MYSQL_INSTANCE_CLEAR_H_");
	line(header, "#error \"特殊头文件,只能被GameBase所包含\"");
	line(header, "#else");
	line(header, "#define _MYSQL_INSTANCE_CLEAR_H_");
	line(header, "");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(header, "mMySQL" + mysqlList[i].mMySQLClassName + " = nullptr;");
	}
	line(header, "");
	line(header, "#endif", false);
	writeFile(filePath + "MySQLInstanceClear.h", ANSIToUTF8(header.c_str(), true));
}