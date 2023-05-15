#include "CodeEvent.h"

void CodeEvent::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "EventSystem/";
	string cppEventFilePath = cppHeaderPath + "Event/";

	string cmdFile = openTxtFile("Event.txt");
	if (cmdFile.length() == 0)
	{
		ERROR("未找文件Event.txt");
		return;
	}
	myVector<string> eventLineList;
	split(cmdFile.c_str(), "\r\n", eventLineList);
	myVector<pair<string, string>> eventList;
	FOR_VECTOR_CONST(eventLineList)
	{
		myVector<string> splitResult;
		split(eventLineList[i].c_str(), "\t", splitResult);
		if (splitResult.size() != 2)
		{
			ERROR("事件文件解析错误:" + eventLineList[i]);
		}
		eventList.push_back(make_pair(splitResult[0], splitResult[1]));
	}
	// 生成StringDefineEvent文件
	generateStringDefineEvent(eventList, cppGameStringDefineFile);
	// 生成EventType.h文件
	generateEventType(eventList, cppHeaderPath);
	FOR_VECTOR_CONST(eventList)
	{
		generateEventFile(eventList[i].first, cppEventFilePath);
	}
}

void CodeEvent::generateStringDefineEvent(const myVector<pair<string, string>>& eventList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// Event"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const auto& item : eventList)
	{
		codeList.insert(++lineStart, stringDeclare(item.first));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

// EventType.h
void CodeEvent::generateEventType(const myVector<pair<string, string>>& eventList, const string& headerPath)
{
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "// 游戏事件定义");
	line(str0, "class EVENT_TYPE");
	line(str0, "{");
	line(str0, "public:");
	// 由于需要添加默认的无效值,所以复制一份列表
	myVector<pair<string, string>> newList;
	newList.push_back(make_pair("None", "// 无效值"));
	newList.addRange(eventList);
	FOR_VECTOR_CONST(newList)
	{
		string type = removeStartString(nameToUpper(newList[i].first), "_EVENT_");
		// 由于None转换出来是以_开头,所以还是需要确保是字母开头
		type = removeStartString(type, "_");
		string str = "\tconst static ushort " + type + " = " + intToString(i) + ";";
		uint tableCount = generateAlignTableCount(str, 48);
		FOR_J(tableCount)
		{
			str += "\t";
		}
		line(str0, str + newList[i].second);
	}
	line(str0, "};", false);

	writeFile(headerPath + "EventType.h", ANSIToUTF8(str0.c_str(), true));
}

void CodeEvent::generateEventFile(const string& eventName, const string& eventPath)
{
	string headerFullPath = eventPath + eventName + ".h";
	if (isFileExist(headerFullPath))
	{
		return;
	}
	string header;
	string typeStr = nameToUpper(eventName.substr(strlen("Event")), false);
	line(header, "#pragma once");
	line(header, "");
	line(header, "#include \"GameEvent.h\"");
	line(header, "");
	line(header, "class " + eventName + " : public GameEvent");
	line(header, "{");
	line(header, "public:");
	line(header, "\t" + eventName + "()");
	line(header, "\t{");
	line(header, "\t\tmType = EVENT_TYPE::" + typeStr + ";");
	line(header, "\t}");
	line(header, "public:");
	line(header, "};", false);

	writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
}