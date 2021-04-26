#include "CodeUtility.h"
#include "Utility.h"

string CodeUtility::ServerProjectPath;
string CodeUtility::GameProjectPath;
string CodeUtility::cppProjectPath;
string CodeUtility::cppGamePath;
string CodeUtility::cppFramePath;
string CodeUtility::cppStringDefinePath;
string CodeUtility::csGamePath;

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
	return true;
}

MySQLMember CodeUtility::parseMySQLMemberLine(string line)
{
	MySQLMember memberInfo;
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
	// 该字段属于客户端还是服务器
	int rectStartIndex = line.find_first_of('[');
	int rectEndIndex = line.find_first_of(']', rectStartIndex);
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
		int lengthMarcoStart = line.find_first_of('[');
		int lengthMarcoEnd = line.find_first_of(']');
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
	int length = packetName.length();
	const int prefixLength = 2;
	if (packetName.substr(0, prefixLength) != "CS" && packetName.substr(0, prefixLength) != "SC")
	{
		ERROR("包名前缀错误");
		return "";
	}
	int lastIndex = prefixLength;
	// 从3开始,因为第0,1个是前缀,第2个始终都是大写,会截取出空字符串
	for (int i = prefixLength + 1; i < length; ++i)
	{
		// 已大写字母为分隔符
		if (packetName[i] >= 'A' && packetName[i] <= 'Z')
		{
			macroList.push_back(packetName.substr(lastIndex, i - lastIndex));
			lastIndex = i;
		}
	}
	macroList.push_back(packetName.substr(lastIndex, length - lastIndex));
	string headerMacro = packetName.substr(0, prefixLength);
	FOR_VECTOR_CONST(macroList)
	{
		headerMacro += "_" + toUpper(macroList[i]);
	}
	return headerMacro;
}

string CodeUtility::nameToUpper(const string& sqliteName)
{
	// 根据大写字母拆分
	myVector<string> macroList;
	int length = sqliteName.length();
	int lastIndex = 0;
	// 从1开始,因为第0个始终都是大写,会截取出空字符串
	for (int i = 1; i < length; ++i)
	{
		// 以大写字母为分隔符
		if (sqliteName[i] >= 'A' && sqliteName[i] <= 'Z')
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

string CodeUtility::cSharpMemberDeclareString(const PacketMember& memberInfo)
{
	// c#里面不用char,使用byte,也没有ullong,使用long
	string typeName = memberInfo.mTypeName;
	if (typeName == "char")
	{
		typeName = "byte";
	}
	else if (typeName == "ullong")
	{
		typeName = "ulong";
	}
	string str;
	if (memberInfo.mIsArray)
	{
		typeName = toUpper(typeName) + "S";
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += "GameDefine." + memberInfo.mArrayLengthMacro[i];
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