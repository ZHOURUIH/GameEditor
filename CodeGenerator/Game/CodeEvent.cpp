#include "CodeEvent.h"

const string CodeEvent::cppHeaderPath = cppGamePath + "EventSystem/";
const string CodeEvent::cppEventFilePath = cppHeaderPath + "Event/";

void CodeEvent::generateEventCode()
{
	string cmdFile;
	openTxtFile("Event.txt", cmdFile);
	if (cmdFile.length() == 0)
	{
		ERROR("未找文件Event.txt");
		return;
	}
	cmdFile = UTF8ToANSI(cmdFile.c_str(), true);
	myVector<string> eventLineList;
	split(cmdFile.c_str(), "\r\n", eventLineList);
	myVector<pair<string, string>> eventList;
	FOR_VECTOR(eventLineList)
	{
		myVector<string> splitResult;
		split(eventLineList[i].c_str(), "\t", splitResult);
		if (splitResult.size() != 2)
		{
			ERROR("事件文件解析错误:" + eventLineList[i]);
		}
		eventList.push_back(make_pair(splitResult[0], splitResult[1]));
	}
	END(eventLineList);
	// 生成StringDefineEvent文件
	generateStringDefineEvent(eventList, cppStringDefinePath);
	// 生成EventHeader.h文件
	generateHeaderFile(eventList, cppHeaderPath);
	// 生成EventType.h文件
	generateEventType(eventList, cppHeaderPath);
	FOR_VECTOR(eventList)
	{
		generateEventFile(eventList[i].first, cppEventFilePath);
	}
	END(eventList);
}

// EventHeader.h文件
void CodeEvent::generateHeaderFile(const myVector<pair<string, string>>& eventList, string headerPath)
{
	string str0;
	line(str0, "#ifndef _EVENT_HEADER_H_");
	line(str0, "#define _EVENT_HEADER_H_");
	line(str0, "");
	uint count = eventList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + eventList[i].first + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(headerPath + "EventHeader.h", str0);
}

// StringDefineEvent.h和StringDefineEvent.cpp
void CodeEvent::generateStringDefineEvent(const myVector<pair<string, string>>& eventList, string stringDefinePath)
{
	// 头文件
	string header;
	uint cmdCount = eventList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + eventList[i].first + ");");
	}

	header = ANSIToUTF8(header.c_str(), true);
	writeFile(stringDefinePath + "StringDefineEvent.h", header);

	// 源文件
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"EventHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + eventList[i].first + ");");
	}

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(stringDefinePath + "StringDefineEvent.cpp", source);
}

// EventType.h
void CodeEvent::generateEventType(const myVector<pair<string, string>>& eventList, string headerPath)
{
	string str0;
	line(str0, "#ifndef _EVENT_TYPE_H_");
	line(str0, "#define _EVENT_TYPE_H_");
	line(str0, "");
	line(str0, "// 游戏事件定义");
	line(str0, "class EVENT_TYPE");
	line(str0, "{");
	line(str0, "public:");
	int maxTableCount = 12;
	FOR_VECTOR_CONST(eventList)
	{
		string type = nameToUpper(eventList[i].first);
		type = type.substr(strlen("_EVENT_"));
		string str = "\tconst static ushort " + type + " = " + intToString(i + 1) + ";";
		// 第一个减一是要去除字符串中的开头的制表符,第二个减一是因为字符串开头已经有一个制表符了,所以可以少加1个
		int tableCount = ceil((maxTableCount * 4 - (str.length() - 1)) / 4.0f) - 1;
		clampMin(tableCount, 0);
		string tableStr;
		FOR_J((uint)tableCount)
		{
			tableStr += "\t";
		}
		line(str0, str + tableStr + eventList[i].second);
	}
	line(str0, "};");
	line(str0, "");
	line(str0, "#endif", false);

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(headerPath + "EventType.h", str0);
}

void CodeEvent::generateEventFile(const string& eventName, string eventPath)
{
	string headerFullPath = eventPath + eventName + ".h";
	if (isFileExist(headerFullPath))
	{
		return;
	}
	string header;
	string marcoName = nameToUpper(eventName) + "_H_";
	string typeStr = nameToUpper(eventName);
	typeStr = typeStr.substr(strlen("_EVENT_"));
	line(header, "#ifndef " + marcoName);
	line(header, "#define " + marcoName);
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
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	header = ANSIToUTF8(header.c_str(), true);
	writeFile(headerFullPath, header);
}