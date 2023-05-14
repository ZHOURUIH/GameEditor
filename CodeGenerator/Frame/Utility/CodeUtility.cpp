#include "CodeUtility.h"
#include "Utility.h"

string CodeUtility::ServerProjectPath;
string CodeUtility::ClientProjectPath;
myVector<string> CodeUtility::ServerExcludeIncludePath;
string CodeUtility::cppGameProjectPath;
string CodeUtility::cppBattleCoreProjectPath;
string CodeUtility::cppFrameProjectPath;
string CodeUtility::cppGamePath;
string CodeUtility::cppBattleCorePath;
string CodeUtility::cppFramePath;
string CodeUtility::cppGameStringDefineFile;
string CodeUtility::csGamePath;
string CodeUtility::csHotfixGamePath;
string CodeUtility::START_FALG = "#start";

bool CodeUtility::initPath()
{
	myVector<string> configLines;
	openTxtFileLines("./CodeGenerator_Config.txt", configLines);
	if (configLines.size() == 0)
	{
		cout << "未找到配置文件CodeGenerator_Config.txt" << endl;
		return false;
	}
	FOR_VECTOR(configLines)
	{
		myVector<string> params;
		removeAll(configLines[i], ' ', '\t');
		split(configLines[i].c_str(), "=", params);
		if (params.size() != 2)
		{
			continue;
		}
		if (params[0] == "SERVER_PROJECT_PATH")
		{
			ServerProjectPath = params[1];
		}
		else if (params[0] == "GAME_PROJECT_PATH")
		{
			ClientProjectPath = params[1];
		}
		else if (params[0] == "SERVER_EXCLUDE_INCLUDE_PATH")
		{
			rightToLeft(params[1]);
			split(params[1].c_str(), ",", ServerExcludeIncludePath);
		}
	}
	END(configLines);

	if (ServerProjectPath == "" && ClientProjectPath == "")
	{
		return false;
	}

	if (ServerProjectPath.length() > 0)
	{
		rightToLeft(ServerProjectPath);
		validPath(ServerProjectPath);
		cppGameProjectPath = ServerProjectPath + "MicroLegend_Server/";
		cppBattleCoreProjectPath = ServerProjectPath + "MicroLegend_Server_BattleCore/";
		cppFrameProjectPath = ServerProjectPath + "MicroLegend_Server_Frame/";
		cppGamePath = cppGameProjectPath + "Game/";
		cppBattleCorePath = cppBattleCoreProjectPath + "BattleCore/";
		cppFramePath = cppFrameProjectPath + "Frame/";
		cppGameStringDefineFile = cppGamePath + "StringDefine/StringDefine.h";
	}
	if (ClientProjectPath.length() > 0)
	{
		rightToLeft(ClientProjectPath);
		validPath(ClientProjectPath);
		csGamePath = ClientProjectPath + "Assets/Scripts/Game/";
		csHotfixGamePath = ClientProjectPath + "HotFix/Game/";
	}
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
			else if (owner == "[None]")
			{
				memberInfo.mOwner = SQLITE_OWNER::NONE;
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
	return "pushParam(" + memberInfo.mMemberName + ");";
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
		str = "Array<" + lengthMacro + ", " + memberInfo.mTypeName + "> " + memberInfo.mMemberName + ";";
	}
	else
	{
		str = memberInfo.mTypeName + " " + memberInfo.mMemberName + ";";
	}
	return str;
}

string CodeUtility::cSharpPushParamString(const PacketMember& memberInfo)
{
	return "pushParam(" + memberInfo.mMemberName + ");";
}

string CodeUtility::cppTypeToCSharpType(const string& cppType)
{
	if (cppType == "char")
	{
		return "byte";
	}
	else if (cppType == "ullong" || cppType == "llong")
	{
		return "long";
	}
	return cppType;
}

string CodeUtility::cSharpMemberDeclareString(const PacketMember& memberInfo, bool isHotFix)
{
	// c#里面不用char,使用byte,也没有ullong,使用long
	string typeName = toUpper(cppTypeToCSharpType(memberInfo.mTypeName));
	string str;
	if (memberInfo.mIsArray)
	{
		typeName += "S";
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += memberInfo.mArrayLengthMacro[i];
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		str = "public " + typeName + " " + memberInfo.mMemberName + " = new " + typeName + "(" + lengthMacro + ");";
	}
	else
	{
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
	packetInfo.mUDP = tagList.contains("[UDP]");
	packetInfo.mShowInfo = !tagList.contains("[NoLog]");
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

string CodeUtility::convertToCSharpType(const string& cppType)
{
	// 因为模板文件是按照C++来写的,但是有些类型在C#中是没有的,所以要转换为C#中对应的类型
	// Vector替换为List,char替换为sbyte,llong替换为long
	if (startWith(cppType, "Vector<"))
	{
		string newType = cppType;
		strReplaceAll(newType, "Vector<", "List<");
		return newType;
	}
	if (cppType == "char")
	{
		return "sbyte";
	}
	if (cppType == "llong")
	{
		return "long";
	}
	return cppType;
}

bool CodeUtility::findCustomCode(const string& fullPath, myVector<string>& codeList, int& lineStart, 
								const LineMatchCallback& startLineMatch, const LineMatchCallback& endLineMatch)
{
	openTxtFileLines(fullPath, codeList);
	lineStart = -1;
	int endCode = -1;
	for (int i = 0; i < codeList.size(); ++i)
	{
		if (lineStart < 0 && startLineMatch(codeList[i]))
		{
			lineStart = i;
			continue;
		}
		if (lineStart >= 0)
		{
			if (endLineMatch(codeList[i]))
			{
				endCode = i;
				break;
			}
		}
	}
	if (lineStart < 0)
	{
		cout << "找不到代码特定起始段,文件名:" << fullPath << endl;
		return false;
	}
	if (endCode < 0)
	{
		cout << "找不到代码特定结束段,文件名:" << fullPath << endl;
		return false;
	}
	int removeLineCount = endCode - lineStart - 1;
	for (int i = 0; i < removeLineCount; ++i)
	{
		codeList.erase(lineStart + 1);
	}
	return true;
}

myVector<string> CodeUtility::findTargetHeaderFile(const string& path, const string& filePrefix, const LineMatchCallback& lineMatch)
{
	myVector<string> needDefineCmds;
	myVector<string> cmdFiles;
	findFiles(path, cmdFiles, ".h");
	for (const string& fileName : cmdFiles)
	{
		if (!startWith(getFileName(fileName), filePrefix))
		{
			continue;
		}
		myVector<string> lines;
		openTxtFileLines(fileName, lines);
		for (const string& line : lines)
		{
			if (lineMatch(line))
			{
				needDefineCmds.push_back(getFileNameNoSuffix(fileName, true));
				break;
			}
		}
	}
	return needDefineCmds;
}

string CodeUtility::codeListToString(const myVector<string>& codeList)
{
	string str;
	for (int i = 0; i < codeList.size(); ++i)
	{
		line(str, codeList[i], i != codeList.size() - 1);
	}
	return str;
}