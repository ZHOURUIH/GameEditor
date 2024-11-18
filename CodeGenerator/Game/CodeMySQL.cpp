#include "CodeMySQL.h"

void CodeMySQL::generate()
{
	print("��������MySQL");
	// ����ģ���ļ�
	// �����ļ��Ƿ��Ѿ���ʼ����
	bool fileStart = false;
	// �Ƿ��Ѿ���ʼ����һ������������
	bool packetStart = false;
	// �Ƿ��Ѿ���ʼ����һ������е�����
	bool indexStart = false;
	myVector<string> lines = openTxtFileLines("MySQL.txt");
	if (lines.size() == 0)
	{
		ERROR("δ�ҵ�����ʽ�ļ�MySQL.txt");
		return;
	}
	myVector<MySQLInfo> mySQLInfoList;
	MySQLInfo tempInfo;
	FOR_VECTOR(lines)
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
		// ȥ�������Ʊ��,�ֺ�
		removeAll(line, '\t', ';');
		// ��Ա�����б���ʼ
		if (line == "{")
		{
			indexStart = false;
			packetStart = true;
			string comment;
			// ���ע��,ֻȡһ��,��֧�ֶ���
			if (i >= 2 && startWith(lines[i - 2], "//"))
			{
				comment = lines[i - 2].substr(strlen("//"));
				removeStartAll(comment, ' ');
			}
			string tableTitle = lines[i - 1];
			removeAll(tableTitle, '\t', ';');
			myVector<string> titleVector;
			split(tableTitle.c_str(), ":", titleVector);
			if (titleVector.size() != 2)
			{
				ERROR("mysql���ĸ�ʽ����: " + lines[i - 1]);
				return;
			}
			// �Ƴ���ǩ
			tempInfo.init(titleVector[0], titleVector[1], comment);
			continue;
		}
		// ��Ա�����б����
		if (line == "}")
		{
			mySQLInfoList.push_back(tempInfo);
			packetStart = false;
			continue;
		}
		if (line == "index:")
		{
			indexStart = true;
			continue;
		}
		if (packetStart)
		{
			// ��ǰ���ڽ�������
			if (indexStart)
			{
				tempInfo.mIndexList.push_back(line);
			}
			// ��ǰ���ڽ����б���ֶ�
			else
			{
				tempInfo.mMemberList.push_back(parseMySQLMemberLine(line));
			}
		}
	}
	// Game
	string cppGameDataPath = cppGamePath + "DataBase/MySQL/Data/";
	string cppGameTablePath = cppGamePath + "DataBase/MySQL/Table/";
	deleteFolder(cppGameDataPath);
	for (const MySQLInfo& info : mySQLInfoList)
	{
		// ���ɴ����ļ�
		generateCppMySQLDataFile(info, cppGameDataPath);
		generateCppMySQLTableFile(info, cppGameTablePath);
	}
	// ��һ��Ŀ¼����MySQLHeader.h
	string totalHeaderGamePath = cppGameDataPath;
	removeEnd(totalHeaderGamePath, '/');
	totalHeaderGamePath = getFilePath(totalHeaderGamePath) + "/";
	const string gameBaseHeaderPath = cppGamePath + "Common/GameBase.h";
	const string gameBaseSourcePath = cppGamePath + "Common/GameBase.cpp";
	generateCppMySQLRegisteFile(mySQLInfoList, totalHeaderGamePath);
	myVector<string> newGameList;
	for (const MySQLInfo& info : mySQLInfoList)
	{
		newGameList.push_back("MD" + info.mMySQLClassName);
	}
	generateMySQLInstanceDeclare(mySQLInfoList, gameBaseHeaderPath, "");
	generateMySQLInstanceDefine(mySQLInfoList, gameBaseSourcePath);
	generateMySQLInstanceClear(mySQLInfoList, gameBaseSourcePath);
	print("�������MySQL");
	print("");
}

// ����MySQLData.h��MySQLData.cpp�ļ�
void CodeMySQL::generateCppMySQLDataFile(const MySQLInfo& mysqlInfo, const string& filePath)
{
	// ͷ�ļ�
	string header;
	string className = "MD" + mysqlInfo.mMySQLClassName;
	line(header, "#pragma once");
	line(header, "");
	line(header, "#include \"MySQLData.h\"");
	line(header, "");
	line(header, "// " + mysqlInfo.mComment);
	line(header, "class " + className + " : public MySQLData");
	line(header, "{");
	line(header, "\tBASE(" + className + ", MySQLData);");
	line(header, "public:");
	line(header, "\tstatic const int ID;");
	const uint memberCount = mysqlInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(header, "\tstatic const int " + mysqlInfo.mMemberList[i].mMemberName + ";");
	}

	line(header, "\tstatic const string Name_ID;");
	FOR_I(memberCount)
	{
		line(header, "\tstatic const string Name_" + mysqlInfo.mMemberList[i].mMemberName + ";");
	}
	line(header, "public:");
	FOR_I(memberCount)
	{
		string memberLine;
		const string& typeName = mysqlInfo.mMemberList[i].mTypeName;
		if (typeName == "int" || typeName == "uint" || typeName == "byte" || typeName == "char" || typeName == "short" || typeName == "ushort" || typeName == "llong")
		{
			memberLine = "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + " = 0;";
		}
		else if (typeName == "bool")
		{
			memberLine = "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + " = false;";
		}
		else if (typeName == "float")
		{
			memberLine = "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + " = 0.0f;";
		}
		else if (typeName == "string")
		{
			memberLine = "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + ";";
		}
		const uint tabCount = generateAlignTableCount(memberLine, 48);
		FOR_I(tabCount)
		{
			memberLine += '\t';
		}
		memberLine += "// " + mysqlInfo.mMemberList[i].mComment;
		line(header, memberLine);
	}
	line(header, "public:");
	line(header, "\tstatic void fillColName(MySQLTable* table);");
	line(header, "\tvoid parseResult(const HashMap<int, char*>& resultRow) override;");
	line(header, "\tvoid paramList(string& params) const override;");
	line(header, "\tvoid generateUpdate(string& params, ullong flag) const override;");
	line(header, "\tvoid clone(MySQLData* target) const override;");
	line(header, "\tvoid cloneWithFlag(MySQLData* target, ullong flag) const override;");
	line(header, "\tvoid resetProperty() override;");
	line(header, "\tbool updateBool(bool value, int index) override;");
	line(header, "\tbool updateInt(int value, int index) override;");
	line(header, "\tbool updateFloat(float value, int index) override;");
	line(header, "\tbool updateLLong(llong value, int index) override;");
	line(header, "\tbool updateString(const string& value, int index) override;");
	line(header, "\tbool hasBool(bool value, int index) override;");
	line(header, "\tbool hasInt(int value, int index) override;");
	line(header, "\tbool hasFloat(float value, int index) override;");
	line(header, "\tbool hasLLong(llong value, int index) override;");
	line(header, "\tbool hasString(const string& value, int index) override;");
	line(header, "\tbool getBool(int index) override;");
	line(header, "\tint getInt(int index) override;");
	line(header, "\tfloat getFloat(int index) override;");
	line(header, "\tllong getLLong(int index) override;");
	line(header, "\tconst string& getString(int index) override;");
	line(header, "};", false);

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	// �ֶξ�̬��������
	line(source, "const int " + className + "::ID = 0;");
	FOR_I(memberCount)
	{
		line(source, "const int " + className + "::" + mysqlInfo.mMemberList[i].mMemberName + " = " + StringUtility::intToString(i + 1) + ";");
	}
	line(source, "const string " + className + "::Name_ID = \"ID\";");
	FOR_I(memberCount)
	{
		line(source, "const string " + className + "::Name_" + mysqlInfo.mMemberList[i].mMemberName + " = \"" + mysqlInfo.mMemberList[i].mMemberName + "\";");
	}
	// fillColName����
	line(source, "");
	line(source, "void " + className + "::fillColName(MySQLTable* table)");
	line(source, "{");
	line(source, "\ttable->addColName(Name_ID);");
	FOR_I(memberCount)
	{
		line(source, "\ttable->addColName(Name_" + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	line(source, "}");
	line(source, "");
	// parseResult����
	line(source, "void " + className + "::parseResult(const HashMap<int, char*>& resultRow)");
	line(source, "{");
	line(source, "\tparseLLong(mID, resultRow.tryGet(ID));");
	FOR_I(memberCount)
	{
		const string& typeName = mysqlInfo.mMemberList[i].mTypeName;
		const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
		if (typeName == "int")
		{
			line(source, "\tparseInt(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
		else if (typeName == "uint")
		{
			line(source, "\tparseUInt(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
		else if (typeName == "bool")
		{
			line(source, "\tparseBool(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
		else if (typeName == "byte")
		{
			line(source, "\tparseByte(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
		else if (typeName == "char")
		{
			line(source, "\tparseChar(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
		else if (typeName == "short")
		{
			line(source, "\tparseShort(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
		else if (typeName == "ushort")
		{
			line(source, "\tparseUShort(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
		else if (typeName == "float")
		{
			line(source, "\tparseFloat(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
		else if (typeName == "llong")
		{
			line(source, "\tparseLLong(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
		else if (typeName == "string")
		{
			line(source, "\tparseString(m" + memberName + ", resultRow.tryGet(" + memberName + "));");
		}
	}
	line(source, "}");
	line(source, "");

	// paramList����
	line(source, "void " + className + "::paramList(string& params) const");
	line(source, "{");
	line(source, "\tsqlAddLLong(params, mID, " + (memberCount > 0 ? string("true") : string("false")) + ");");
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
				line(source, "\tsqlAddStringUTF8(params, m" + memberName + ", " + addComma + ");");
			}
			else
			{
				line(source, "\tsqlAddString(params, m" + memberName + ", " + addComma + ");");
			}
		}
		else if (typeName == "int")
		{
			line(source, "\tsqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "uint")
		{
			line(source, "\tsqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "bool")
		{
			line(source, "\tsqlAddInt(params, m" + memberName + " ? 1 : 0, " + addComma + ");");
		}
		else if (typeName == "byte")
		{
			line(source, "\tsqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "char")
		{
			line(source, "\tsqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "short")
		{
			line(source, "\tsqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "ushort")
		{
			line(source, "\tsqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "float")
		{
			line(source, "\tsqlAddFloat(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "llong")
		{
			line(source, "\tsqlAddLLong(params, m" + memberName + ", " + addComma + ");");
		}
	}
	line(source, "}");
	line(source, "");

	// generateUpdate����
	line(source, "void " + className + "::generateUpdate(string& params, const ullong flag) const");
	line(source, "{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		line(source, "\tif (hasBit(flag, " + memberName + "))");
		line(source, "\t{");
		if (typeName == "string")
		{
			if (memberInfo.mUTF8)
			{
				line(source, "\t\tsqlUpdateStringUTF8(params, Name_" + memberName + ", m" + memberName + ");");
			}
			else
			{
				line(source, "\t\tsqlUpdateString(params, Name_" + memberName + ", m" + memberName + ");");
			}
		}
		else if (typeName == "int")
		{
			line(source, "\t\tsqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "uint")
		{
			line(source, "\t\tsqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "bool")
		{
			line(source, "\t\tsqlUpdateBool(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "byte")
		{
			line(source, "\t\tsqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "char")
		{
			line(source, "\t\tsqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "short")
		{
			line(source, "\t\tsqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "ushort")
		{
			line(source, "\t\tsqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "float")
		{
			line(source, "\t\tsqlUpdateFloat(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "llong")
		{
			line(source, "\t\tsqlUpdateLLong(params, Name_" + memberName + ", m" + memberName + ");");
		}
		line(source, "\t}");
	}
	line(source, "\t// ȥ�����Ķ���");
	line(source, "\tconst int length = (int)params.length();");
	line(source, "\tif (length > 0)");
	line(source, "\t{");
	line(source, "\t\tparams.erase(length - 1);");
	line(source, "\t}");
	line(source, "}");
	line(source, "");

	// clone����
	line(source, "void " + className + "::clone(MySQLData* target) const");
	line(source, "{");
	line(source, "\tbase::clone(target);");
	if (memberCount > 0)
	{
		line(source, "\tauto* targetData = static_cast<This*>(target);");
		FOR_I(memberCount)
		{
			const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
			line(source, "\ttargetData->m" + memberName + " = m" + memberName + ";");
		}
	}
	line(source, "}");
	line(source, "");

	// cloneWithFlag����
	line(source, "void " + className + "::cloneWithFlag(MySQLData* target, const ullong flag) const");
	line(source, "{");
	line(source, "\tbase::cloneWithFlag(target, flag);");
	if (memberCount > 0)
	{
		line(source, "\tauto* targetData = static_cast<This*>(target);");
		FOR_I(memberCount)
		{
			const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
			line(source, "\tif (hasBit(flag, " + memberName + "))");
			line(source, "\t{");
			line(source, "\t\ttargetData->m" + memberName + " = m" + memberName + ";");
			line(source, "\t}");
		}
	}
	line(source, "}");
	line(source, "");

	// resetProperty����
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

	// updateBool����
	line(source, "bool " + className + "::updateBool(const bool value, const int index)");
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
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateInt����
	line(source, "bool " + className + "::updateInt(const int value, const int index)");
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
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateFloat����
	line(source, "bool " + className + "::updateFloat(const float value, const int index)");
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
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateLLong����
	line(source, "bool " + className + "::updateLLong(const llong value, const int index)");
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
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateString����
	line(source, "bool " + className + "::updateString(const string& value, const int index)");
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
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");

	// hasBool����
	line(source, "bool " + className + "::hasBool(const bool value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "bool")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// hasInt����
	line(source, "bool " + className + "::hasInt(const int value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "int" || typeName == "ushort" || typeName == "short" || typeName == "byte" || typeName == "char")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// hasFloat����
	line(source, "bool " + className + "::hasFloat(const float value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "float")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// hasLLong����
	line(source, "bool " + className + "::hasLLong(const llong value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "llong")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// hasString����
	line(source, "bool " + className + "::hasString(const string& value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "string")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// getBool����
	line(source, "bool " + className + "::getBool(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "bool")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// getInt����
	line(source, "int " + className + "::getInt(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "int" || typeName == "ushort" || typeName == "short" || typeName == "byte" || typeName == "char")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return 0;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn 0;");
	line(source, "}");
	line(source, "");

	// getFloat����
	line(source, "float " + className + "::getFloat(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "float")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return 0.0f;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn 0.0f;");
	line(source, "}");
	line(source, "");

	// getLLong����
	line(source, "llong " + className + "::getLLong(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "llong")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return 0;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn 0;");
	line(source, "}");
	line(source, "");

	// getString����
	line(source, "const string& " + className + "::getString(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "string")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return FrameDefine::EMPTY;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn FrameDefine::EMPTY;");
	line(source, "}", false);

	writeFile(filePath + className + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(filePath + className + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// ����MySQLTable.h��MySQLTable.cpp�ļ�
void CodeMySQL::generateCppMySQLTableFile(const MySQLInfo& mysqlInfo, const string& filePath)
{
	string tableClassName = "MySQL" + mysqlInfo.mMySQLClassName;
	string dataClassName = "MD" + mysqlInfo.mMySQLClassName;

	// ͷ�ļ�
	string header;
	line(header, "#pragma once");
	line(header, "");
	line(header, "#include \"MySQLTableT.h\"");
	line(header, "#include \"" + dataClassName + ".h\"");
	line(header, "");
	line(header, "class " + tableClassName + " : public MySQLTableT<" + dataClassName + ">");
	line(header, "{");
	line(header, "\tBASE(" + tableClassName + ", MySQLTableT<" + dataClassName + ">);");
	line(header, "public:");
	line(header, "\texplicit " + tableClassName + "(const char* tableName) :base(tableName) {}");
	if (mysqlInfo.mIndexList.size() > 0)
	{
		line(header, "\tvoid lateInit() override;");
	}
	line(header, "};", false);

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	if (mysqlInfo.mIndexList.size() > 0)
	{
		line(source, "void " + tableClassName + "::lateInit()");
		line(source, "{");
		line(source, "\tbase::lateInit();");
		for (const string& indexName : mysqlInfo.mIndexList)
		{
			line(source, "\texecuteNonQuery((string(\"ALTER TABLE \") + mTableName + \" ADD INDEX " + indexName + "(\" + " + dataClassName + "::Name_" + indexName + " + \")\").c_str(), false, true);");
		}
		line(source, "}", false);
	}

	writeFile(filePath + tableClassName + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(filePath + tableClassName + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// MySQLRegiste.h��MySQLRegiste.cpp�ļ�
void CodeMySQL::generateCppMySQLRegisteFile(const myVector<MySQLInfo>& mysqlList, const string& filePath)
{
	// MySQLRegiste.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"GameBase.h\"");
	line(str0, "");
	line(str0, "class MySQLRegister");
	line(str0, "{");
	line(str0, "public:");
	line(str0, "\tstatic void registeAll();");
	line(str0, "};", false);
	writeFile(filePath + "MySQLRegister.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "#include \"GameHeader.h\"");
	line(str1, "");
	line(str1, "void MySQLRegister::registeAll()");
	line(str1, "{");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		const string& mysqlClassName = mysqlList[i].mMySQLClassName;
		line(str1, "\tmMySQL" + mysqlClassName + " = mMySQLManager->registeTable<MySQL" + mysqlClassName + ">(\"" + mysqlList[i].mMySQLTableName + "\");");
	}
	line(str1, "}", false);
	writeFile(filePath + "MySQLRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

void CodeMySQL::generateMySQLInstanceDeclare(const myVector<MySQLInfo>& mysqlList, const string& gameBaseHeaderFile, const string& exprtMacro)
{
	// ����GameBase.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseHeaderFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// MySQL"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "};"); }))
	{
		return;
	}
	for (const MySQLInfo& info : mysqlList)
	{
		codeList.insert(++lineStart, "\t" + exprtMacro + "extern MySQL" + info.mMySQLClassName + "* mMySQL" + info.mMySQLClassName + ";");
	}
	writeFile(gameBaseHeaderFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeMySQL::generateMySQLInstanceDefine(const myVector<MySQLInfo>& mysqlList, const string& gameBaseSourceFile)
{
	// ����GameBase.cpp���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseSourceFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// MySQL Define"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "};"); }))
	{
		return;
	}
	for (const MySQLInfo& info : mysqlList)
	{
		codeList.insert(++lineStart, "\tMySQL" + info.mMySQLClassName + "* mMySQL" + info.mMySQLClassName + ";");
	}
	writeFile(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeMySQL::generateMySQLInstanceClear(const myVector<MySQLInfo>& mysqlList, const string& gameBaseSourceFile)
{
	// ����GameBase.cpp���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseSourceFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// MySQL Clear"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "};"); }))
	{
		return;
	}

	for (const MySQLInfo& info : mysqlList)
	{
		codeList.insert(++lineStart, "\t\tmMySQL" + info.mMySQLClassName + " = nullptr;");
	}
	writeFile(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}