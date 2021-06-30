#include "CodeUtility.h"
#include "Utility.h"

string CodeUtility::ServerProjectPath;
string CodeUtility::GameProjectPath;
string CodeUtility::cppProjectPath;
string CodeUtility::cppGamePath;
string CodeUtility::cppFramePath;
string CodeUtility::cppStringDefinePath;
string CodeUtility::csGamePath;
string CodeUtility::csHotfixGamePath;
string CodeUtility::START_FALG = "#start";

bool CodeUtility::initPath()
{
	ServerProjectPath = getEnvironmentValue("SERVER_PROJECT_PATH");
	GameProjectPath = getEnvironmentValue("GAME_PROJECT_PATH");
	if (ServerProjectPath == "" || GameProjectPath == "")
	{
		return false;
	}
	cppProjectPath = ServerProjectPath + "MicroLegend_Server/";
	cppGamePath = cppProjectPath + "Game/";
	cppFramePath = cppProjectPath + "Frame/";
	cppStringDefinePath = cppGamePath + "StringDefine/";
	csGamePath = GameProjectPath + "Assets/Scripts/Game/";
	csHotfixGamePath = GameProjectPath + "HotFix/Game/";
	return true;
}

MySQLMember CodeUtility::parseMySQLMemberLine(string line)
{
	MySQLMember memberInfo;

	// 字段注释
	int pos = -1;
	if (findString(line.c_str(), "//", &pos))
	{
		memberInfo.mComment = line.substr(pos + strlen("//"));
		removeStartAll(memberInfo.mComment, ' ');
		line = line.substr(0, pos);
	}

	// 字段类型和字段名
	myVector<string> memberStrList;
	split(line.c_str(), " ", memberStrList);
	if (findString(memberStrList[0].c_str(), "(utf8)", NULL))
	{
		strReplaceAll(memberStrList[0], "(utf8)", "");
		memberInfo.mUTF8 = true;
	}
	else
	{
		memberInfo.mUTF8 = false;
	}
	memberInfo.mTypeName = memberStrList[0];
	memberInfo.mMemberName = memberStrList[1];
	return memberInfo;
}

SQLiteMember CodeUtility::parseSQLiteMemberLine(string line, bool ignoreClientServer)
{
	SQLiteMember memberInfo;
	
	// 字段注释
	int pos = -1;
	if (findString(line.c_str(), "//", &pos))
	{
		memberInfo.mComment = line.substr(pos + strlen("//"));
		removeStartAll(memberInfo.mComment, ' ');
		line = line.substr(0, pos);
	}

	// 该字段属于客户端还是服务器
	int rectStartIndex = (int)line.find_first_of('[');
	int rectEndIndex = (int)line.find_first_of(']', rectStartIndex);
	if (rectStartIndex >= 0 && rectEndIndex >= 0)
	{
		string owner = line.substr(rectStartIndex, rectEndIndex - rectStartIndex + 1);
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
			else
			{
				memberInfo.mOwner = SQLITE_OWNER::BOTH;
			}
		}
		line.erase(rectStartIndex, rectEndIndex - rectStartIndex + 1);
	}
	else
	{
		memberInfo.mOwner = SQLITE_OWNER::BOTH;
	}
	// 字段类型和字段名
	myVector<string> memberStrList;
	split(line.c_str(), " ", memberStrList);
	memberInfo.mTypeName = memberStrList[0];
	memberInfo.mMemberName = memberStrList[1];
	return memberInfo;
}

PacketMember CodeUtility::parseMemberLine(const string& line)
{
	PacketMember memberInfo;
	// 数组成员变量
	if (line.find_first_of('[') != -1)
	{
		// 如果是数组,则优先处理[]内部的常量
		int lengthMarcoStart = (int)line.find_first_of('[');
		int lengthMarcoEnd = (int)line.find_first_of(']');
		string lengthMacro = line.substr(lengthMarcoStart + 1, lengthMarcoEnd - lengthMarcoStart - 1);
		strReplaceAll(lengthMacro, " ", "");
		split(lengthMacro.c_str(), "*", memberInfo.mArrayLengthMacro);
		// 常量处理完了,再判断变量类型和变量名
		string memberLine = line.substr(0, lengthMarcoStart);
		myVector<string> memberStrList;
		split(memberLine.c_str(), " ", memberStrList);
		memberInfo.mTypeName = memberStrList[0];
		strReplaceAll(memberInfo.mTypeName, "\t", "");
		memberInfo.mIsArray = true;
		memberInfo.mMemberName = memberStrList[1];
		// 只有数组成员变量才会有第三个参数
		if (memberStrList.size() == 3)
		{
			memberInfo.mVariableLength = stringToBool(memberStrList[2]);
		}
		else
		{
			memberInfo.mVariableLength = true;
		}
	}
	// 普通成员变量
	else
	{
		myVector<string> memberStrList;
		split(line.c_str(), " ", memberStrList);
		if (memberStrList.size() != 2 && memberStrList.size() != 3)
		{
			ERROR("成员变量行错误:" + line);
			return PacketMember();
		}
		memberInfo.mTypeName = memberStrList[0];
		strReplaceAll(memberInfo.mTypeName, "\t", "");
		memberInfo.mIsArray = false;
		memberInfo.mMemberName = memberStrList[1];
	}
	return memberInfo;
}

string CodeUtility::packetNameToUpper(const string& packetName)
{
	// 根据大写字母拆分
	myVector<string> macroList;
	int length = (int)packetName.length();
	const int prefixLength = 2;
	if (packetName.substr(0, prefixLength) != "CS" && packetName.substr(0, prefixLength) != "SC")
	{
		ERROR("包名前缀错误");
		return "";
	}
	return packetName.substr(0, prefixLength) + nameToUpper(packetName.substr(prefixLength));
}

string CodeUtility::nameToUpper(const string& sqliteName, bool preUnderLine)
{
	// 根据大写字母拆分
	myVector<string> macroList;
	int length = (int)sqliteName.length();
	int lastIndex = 0;
	// 从1开始,因为第0个始终都是大写,会截取出空字符串,最后一个字母也肯不会被分割
	for (int i = 1; i < length; ++i)
	{
		// 以大写字母为分隔符,但是连续的大写字符不能被分隔
		// 非连续数字也会分隔
		char curChar = sqliteName[i];
		char lastChar = sqliteName[i - 1];
		char nextChar = i + 1 < length ? sqliteName[i + 1] : '\0';
		if (isUpper(curChar) && (!isUpper(lastChar) || (nextChar != '\0' && !isUpper(nextChar))) ||
			isNumber(curChar) && (!isNumber(lastChar) || (nextChar != '\0' && !isNumber(nextChar))))
		{
			macroList.push_back(sqliteName.substr(lastIndex, i - lastIndex));
			lastIndex = i;
		}
	}
	macroList.push_back(sqliteName.substr(lastIndex, length - lastIndex));
	string headerMacro;
	FOR_VECTOR_CONST(macroList)
	{
		headerMacro += "_" + toUpper(macroList[i]);
	}
	if (!preUnderLine)
	{
		headerMacro = headerMacro.erase(0, 1);
	}
	return headerMacro;
}

string CodeUtility::cppPushParamString(const PacketMember& memberInfo)
{
	string str;
	if (memberInfo.mIsArray)
	{
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += "GD::" + memberInfo.mArrayLengthMacro[i];
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		if (!memberInfo.mVariableLength)
		{
			str = "pushParam(" + memberInfo.mMemberName + ", " + lengthMacro + ", false);";
		}
		else
		{
			str = "pushParam(" + memberInfo.mMemberName + ", " + lengthMacro + ");";
		}
	}
	else
	{
		str = "pushParam(" + memberInfo.mMemberName + ");";
	}
	return str;
}

string CodeUtility::cppMemberDeclareString(const PacketMember& memberInfo)
{
	string str;
	if (memberInfo.mIsArray)
	{
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += "GD::" + memberInfo.mArrayLengthMacro[i];
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		str = memberInfo.mTypeName + " " + memberInfo.mMemberName + "[" + lengthMacro + "];";
	}
	else
	{
		str = memberInfo.mTypeName + " " + memberInfo.mMemberName + ";";
	}
	return str;
}

string CodeUtility::cSharpPushParamString(const PacketMember& memberInfo)
{
	string str;
	if (memberInfo.mIsArray)
	{
		str = "pushParam(" + memberInfo.mMemberName + ", " + boolToString(memberInfo.mVariableLength) + ");";
	}
	else
	{
		str = "pushParam(" + memberInfo.mMemberName + ");";
	}
	return str;
}

string CodeUtility::cSharpMemberDeclareString(const PacketMember& memberInfo, bool isHotFix)
{
	// c#里面不用char,使用byte,也没有ullong,使用long
	string typeName = memberInfo.mTypeName;
	if (typeName == "char")
	{
		typeName = "byte";
	}
	else if (typeName == "ullong" || typeName == "llong")
	{
		typeName = "long";
	}
	string str;
	if (memberInfo.mIsArray)
	{
		typeName = toUpper(typeName) + "S";
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			if (isHotFix)
			{
				lengthMacro += "GD." + memberInfo.mArrayLengthMacro[i];
			}
			else
			{
				lengthMacro += "GameDefine." + memberInfo.mArrayLengthMacro[i];
			}
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		str = "public " + typeName + " " + memberInfo.mMemberName + " = new " + typeName + "(" + lengthMacro + ");";
	}
	else
	{
		typeName = toUpper(typeName);
		str = "public " + typeName + " " + memberInfo.mMemberName + " = new " + typeName + "();";
	}
	return str;
}

void CodeUtility::parsePacketName(const string& line, PacketInfo& packetInfo)
{
	int tagStartIndex = 0;
	int startIndex = -1;
	int endIndex = -1;
	// 查找标签
	myVector<string> tagList;
	while (true)
	{
		findString(line.c_str(), "[", &startIndex, tagStartIndex);
		findString(line.c_str(), "]", &endIndex, startIndex);
		if (startIndex < 0 || endIndex < 0)
		{
			break;
		}
		tagList.push_back(line.substr(startIndex, endIndex - startIndex + 1));
		tagStartIndex = endIndex;
	}
	packetInfo.mHotFix = !tagList.contains("[NoHotFix]");
	packetInfo.mShowInfo = !tagList.contains("[false]");
	packetInfo.mClientExecuteInMain = !tagList.contains("[ClientInThread]");
	packetInfo.mServerExecuteInMain = !tagList.contains("[ServerInThread]");
	// 获取原始的表格名称
	int firstTagPos = -1;
	if (findString(line.c_str(), "[", &firstTagPos))
	{
		packetInfo.mPacketName = line.substr(0, firstTagPos);
	}
	else
	{
		packetInfo.mPacketName = line;
	}
}