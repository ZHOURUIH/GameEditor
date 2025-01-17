﻿#include "CodeUtility.h"
#include "Utility.h"

string CodeUtility::ServerProjectPath;
string CodeUtility::ClientProjectPath;
myVector<string> CodeUtility::ServerExcludeIncludePath;
string CodeUtility::START_FALG = "#start";

bool CodeUtility::initPath()
{
	myVector<string> configLines = openTxtFileLines("./CodeGenerator_Config.txt");
	if (configLines.size() == 0)
	{
		cout << "未找到配置文件CodeGenerator_Config.txt" << endl;
		return false;
	}
	FOR_VECTOR_CONST(configLines)
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
		else if (params[0] == "CLIENT_PROJECT_PATH")
		{
			ClientProjectPath = params[1];
		}
		else if (params[0] == "SERVER_EXCLUDE_INCLUDE_PATH")
		{
			rightToLeft(params[1]);
			split(params[1].c_str(), ",", ServerExcludeIncludePath);
		}
	}

	if (ServerProjectPath == "" && ClientProjectPath == "")
	{
		return false;
	}

	if (ServerProjectPath.length() > 0)
	{
		rightToLeft(ServerProjectPath);
		validPath(ServerProjectPath);
	}
	if (ClientProjectPath.length() > 0)
	{
		rightToLeft(ClientProjectPath);
		validPath(ClientProjectPath);
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
	myVector<string> memberStrList;
	split(line.c_str(), " ", memberStrList);
	if (memberStrList.size() != 2 && memberStrList.size() != 3)
	{
		ERROR("成员变量行错误:" + line);
		return PacketMember();
	}
	memberInfo.mTypeName = memberStrList[0];
	strReplaceAll(memberInfo.mTypeName, "\t", "");
	memberInfo.mMemberName = memberStrList[1];
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

string CodeUtility::cSharpPushParamString(const PacketMember& memberInfo)
{
	return "pushParam(" + memberInfo.mMemberName + ");";
}

string CodeUtility::cppTypeToCSharpType(const string& cppType)
{
	if (cppType == "char")
	{
		return "sbyte";
	}
	else if (cppType == "ullong" || cppType == "llong")
	{
		return "long";
	}
	else if (cppType == "Vector<char>")
	{
		return "List<sbyte>";
	}
	else if (cppType == "Vector<ullong>" || cppType == "Vector<llong>")
	{
		return "List<long>";
	}
	string csharpType = cppType;
	// 为了避免误判断Vector2等以Vector开头的类型,需要加上<
	replaceAll(csharpType, "Vector<", "List<");
	return csharpType;
}

string CodeUtility::cSharpTypeToWrapType(const string& csharpType)
{
	if (csharpType == "bool")
	{
		return "BOOL";
	}
	if (csharpType == "byte")
	{
		return "BYTE";
	}
	if (csharpType == "List<byte>")
	{
		return "BYTES";
	}
	if (csharpType == "sbyte")
	{
		return "SBYTE";
	}
	if (csharpType == "List<sbyte>")
	{
		return "SBYTES";
	}
	if (csharpType == "short")
	{
		return "SHORT";
	}
	if (csharpType == "List<short>")
	{
		return "SHORTS";
	}
	if (csharpType == "ushort")
	{
		return "USHORT";
	}
	if (csharpType == "List<ushort>")
	{
		return "USHORTS";
	}
	if (csharpType == "int")
	{
		return "INT";
	}
	if (csharpType == "List<int>")
	{
		return "INTS";
	}
	if (csharpType == "uint")
	{
		return "UINT";
	}
	if (csharpType == "List<uint>")
	{
		return "UINTS";
	}
	if (csharpType == "long")
	{
		return "LONG";
	}
	if (csharpType == "List<long>")
	{
		return "LONGS";
	}
	if (csharpType == "float")
	{
		return "FLOAT";
	}
	if (csharpType == "List<float>")
	{
		return "FLOATS";
	}
	if (csharpType == "string")
	{
		return "STRING";
	}
	if (csharpType == "List<string>")
	{
		return "STRINGS";
	}
	if (csharpType == "Vector2")
	{
		return "VECTOR2";
	}
	if (csharpType == "Vector2UShort")
	{
		return "VECTOR2USHORt";
	}
	if (csharpType == "Vector2Int")
	{
		return "VECTOR2INT";
	}
	if (csharpType == "Vector3")
	{
		return "VECTOR3";
	}
	if (csharpType == "Vector4")
	{
		return "VECTOR4";
	}
	// 如果是自定义的参数类型的列表,则是此类型的名字加上_List后缀
	if (startWith(csharpType, "List<"))
	{
		int lastPos;
		findSubstr(csharpType, ">", &lastPos);
		const string elementType = csharpType.substr(strlen("List<"), lastPos - strlen("List<"));
		return elementType + "_List";
	}
	return csharpType;
}

string CodeUtility::cSharpMemberDeclareString(const PacketMember& memberInfo)
{
	// c#里面不用char,使用byte,也没有ullong,使用long
	string typeName = cSharpTypeToWrapType(cppTypeToCSharpType(memberInfo.mTypeName));
	return "public " + typeName + " " + memberInfo.mMemberName + " = new " + typeName + "();";
}

void CodeUtility::parseStructName(const string& line, PacketStruct& structInfo)
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
	structInfo.mHotFix = !tagList.contains("[NoHotFix]");
	bool isGame = tagList.contains("[Game]");
	bool isGameCore = tagList.contains("[GameCore]");
	if (isGame == isGameCore)
	{
		if (isGame)
		{
			ERROR("结构体只能有一个Game或GameCore标签," + line);
		}
		else
		{
			ERROR("结构体缺少一个Game或GameCore标签," + line);
		}
	}
	structInfo.mOwner = isGame ? PACKET_OWNER::GAME : PACKET_OWNER::GAME_CORE;
	// 获取原始的表格名称
	int firstTagPos = -1;
	if (findString(line.c_str(), "[", &firstTagPos))
	{
		structInfo.mStructName = line.substr(0, firstTagPos);
	}
	else
	{
		structInfo.mStructName = line;
	}
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
	bool isGame = tagList.contains("[Game]");
	bool isGameCore = tagList.contains("[GameCore]");
	if (isGame && isGameCore)
	{
		ERROR("消息只能有一个Game或GameCore标签," + line);
	}
	packetInfo.mOwner = isGame ? PACKET_OWNER::GAME : PACKET_OWNER::GAME_CORE;
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
	codeList = openTxtFileLines(fullPath);
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

myVector<string> CodeUtility::findTargetHeaderFile(const string& path, const LineMatchCallback& fileNameMatch, const LineMatchCallback& lineMatch, myMap<string, myVector<string>>* fileContentList)
{
	myVector<string> needDefineCmds;
	myVector<string> cmdFiles;
	findFiles(path, cmdFiles, ".h");
	for (const string& fileName : cmdFiles)
	{
		if (!fileNameMatch(getFileNameNoSuffix(fileName, true)))
		{
			continue;
		}
		myVector<string> lines = openTxtFileLines(fileName);
		for (const string& line : lines)
		{
			if (lineMatch(line))
			{
				needDefineCmds.push_back(getFileNameNoSuffix(fileName, true));
				if (fileContentList != nullptr)
				{
					fileContentList->insert(getFileNameNoSuffix(fileName, true), lines);
				}
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

string CodeUtility::findClassName(const string& line)
{
	if (startWith(line, "class ") && !findSubstr(line, ";"))
	{
		// 截取出:前面的字符串
		int colonPos = (int)line.find(':');
		// 有:,最靠近:的才是类名
		if (colonPos != -1)
		{
			myVector<string> elements;
			split(line.substr(0, colonPos).c_str(), " ", elements);
			if (elements.size() == 0)
			{
				return "";
			}
			return elements[elements.size() - 1];
		}
		// 没有:,则最后一个元素是类名
		else
		{
			myVector<string> elements;
			split(line.c_str(), " ", elements);
			if (elements.size() == 0)
			{
				return "";
			}
			return elements[elements.size() - 1];
		}
	}
	return "";
}

void CodeUtility::generateStringDefine(const myVector<string>& packetList, const string& key, const string className, const string& stringDefineHeaderFile, const string& stringDefineSourceFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeListHeader;
	int lineStartHeader = -1;
	if (!findCustomCode(stringDefineHeaderFile, codeListHeader, lineStartHeader,
		[key](const string& codeLine) { return endWith(codeLine, key); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : packetList)
	{
		codeListHeader.insert(++lineStartHeader, stringDeclare(item));
	}
	writeFile(stringDefineHeaderFile, ANSIToUTF8(codeListToString(codeListHeader).c_str(), true));

	// 更新StringDefine.cpp的特定部分
	myVector<string> codeListSource;
	int lineStartSource = -1;
	if (!findCustomCode(stringDefineSourceFile, codeListSource, lineStartSource,
		[key](const string& codeLine) { return endWith(codeLine, key); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : packetList)
	{
		codeListSource.insert(++lineStartSource, stringDefine(item, className));
	}
	writeFile(stringDefineSourceFile, ANSIToUTF8(codeListToString(codeListSource).c_str(), true));
}