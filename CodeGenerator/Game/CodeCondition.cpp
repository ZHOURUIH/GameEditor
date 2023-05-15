#include "CodeCondition.h"

void CodeCondition::generate()
{
	string cppHeaderPath = cppGamePath + "ConditionManager/";
	string cppConditionFilePath = cppHeaderPath + "Condition/";
	string cppConditionEnumPath = cppGamePath + "Common/";
	string csRegisterPath = csHotfixGamePath + "ConditionManager/";
	string csConditionFilePath = csRegisterPath + "Condition/";
	string csConditionEnumPath = csHotfixGamePath + "Common/";

	myMap<string, myVector<string>> fileContentMap;
	myVector<string> conditionFileList = findTargetHeaderFile(cppGamePath, 
		[](const string& fileName) { return startWith(fileName, "Condition"); },
		[](const string& line) { return findSubstr(line, " : public Condition"); }, &fileContentMap);

	myVector<pair<string, string>> conditionPairList;
	for (const string& fileName : conditionFileList)
	{
		if (!fileContentMap.contains(fileName))
		{
			cout << "找不到条件文件内容:" << fileName << endl;
			return;
		}
		const myVector<string>& lines = fileContentMap[fileName];
		FOR_VECTOR_CONST(lines)
		{
			if (findSubstr(lines[i], " : public Condition") && i > 0)
			{
				conditionPairList.push_back(make_pair(fileName, lines[i - 1]));
				break;
			}
		}
	}
	if (cppGamePath.length() > 0)
	{
		// c++
		// 生成ConditionRegister.h文件
		generateCppRegisterFile(conditionPairList, cppHeaderPath);
		// 生成CONDITION枚举
		generateCppConditionEnum(conditionPairList, cppConditionEnumPath);
	}

	if (csHotfixGamePath.length() > 0)
	{
		// cs
		// 生成ConditionRegister.cs文件
		generateCSRegisterFile(conditionPairList, csRegisterPath);
		// 生成CONDITION枚举
		generateCSConditionEnum(conditionPairList, csConditionEnumPath);
	}
}

// ConditionRegister.h文件
void CodeCondition::generateCppRegisterFile(const myVector<pair<string, string>>& conditionList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "void ConditionRegister::registeAll()");
	line(str0, "{");
	uint count = conditionList.size();
	FOR_I(count)
	{
		const string& conditionName = conditionList[i].first;
		string conditionEnum = nameToUpper(conditionName.substr(strlen("Condition")), false);
		line(str0, "\tGameBase::mConditionFactoryManager->addFactory<" + conditionName + ">(CONDITION::" + conditionEnum + ");");
	}
	line(str0, "}", false);

	writeFile(filePath + "ConditionRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

// CONDITION枚举
void CodeCondition::generateCppConditionEnum(const myVector<pair<string, string>>& conditionList, const string& filePath)
{
	myVector<string> preContent;
	myVector<string> endContent;
	findCppPreAndEndContent(filePath + "GameEnum.h", preContent, endContent);
	string str0;
	FOR_VECTOR(preContent)
	{
		line(str0, preContent[i]);
	}
	END(preContent);
	
	line(str0, "\tNONE,");
	FOR_VECTOR_CONST(conditionList)
	{
		string conditionEnum = nameToUpper(conditionList[i].first.substr(strlen("Condition")), false);
		string lineStr = "\t" + conditionEnum + ",";
		uint needTableCount = generateAlignTableCount(lineStr, 24);
		FOR_J(needTableCount)
		{
			lineStr += "\t";
		}
		lineStr += conditionList[i].second;
		line(str0, lineStr);
	}
	line(str0, "\tMAX,");

	FOR_VECTOR(endContent)
	{
		line(str0, endContent[i], i != endContent.size() - 1);
	}
	END(endContent);

	writeFile(filePath + "GameEnum.h", ANSIToUTF8(str0.c_str(), true));
}

void CodeCondition::generateCSRegisterFile(const myVector<pair<string, string>>& conditionList, const string& filePath)
{
	string str0;
	line(str0, "using System;");
	line(str0, "using static GBR;");
	line(str0, "");
	line(str0, "public class ConditionRegister");
	line(str0, "{");
	line(str0, "\tpublic static void registeAll()");
	line(str0, "\t{");
	uint count = conditionList.size();
	FOR_I(count)
	{
		const string& conditionName = conditionList[i].first;
		string conditionEnum = nameToUpper(conditionName.substr(strlen("Condition")), false);
		line(str0, "\t\tregiste<" + conditionName + ">(CONDITION." + conditionEnum + ");");
	}
	line(str0, "");
	line(str0, "\t\tmConditionManager.checkConditionTypeCount((int)CONDITION.MAX - 1);");
	line(str0, "\t}");
	line(str0, "\t//------------------------------------------------------------------------------------------------------------------------------");
	line(str0, "\tprotected static void registe<T>(CONDITION type) where T : Condition");
	line(str0, "\t{");
	line(str0, "\t\tmConditionManager.registe(typeof(T), type);");
	line(str0, "\t}");
	line(str0, "}", false);

	writeFile(filePath + "ConditionRegister.cs", ANSIToUTF8(str0.c_str(), true));
}

void CodeCondition::generateCSConditionEnum(const myVector<pair<string, string>>& conditionList, const string& filePath)
{
	myVector<string> preContent;
	myVector<string> endContent;
	findCSPreAndEndContent(filePath + "GameEnum.cs", preContent, endContent);
	string str0;
	FOR_VECTOR(preContent)
	{
		line(str0, preContent[i]);
	}
	END(preContent);

	line(str0, "\tNONE,");
	FOR_VECTOR_CONST(conditionList)
	{
		string conditionEnum = nameToUpper(conditionList[i].first.substr(strlen("Condition")), false);
		string lineStr = "\t" + conditionEnum + ",";
		uint needTableCount = generateAlignTableCount(lineStr, 24);
		FOR_J(needTableCount)
		{
			lineStr += "\t";
		}
		lineStr += conditionList[i].second;
		line(str0, lineStr);
	}
	line(str0, "\tMAX,");

	FOR_VECTOR(endContent)
	{
		line(str0, endContent[i], i != endContent.size() - 1);
	}
	END(endContent);

	writeFile(filePath + "GameEnum.cs", ANSIToUTF8(str0.c_str(), true));
}

void CodeCondition::findCppPreAndEndContent(const string& fullPath, myVector<string>& preContent, myVector<string>& endContent)
{
	// 0表示正在查找前段部分的代码
	// 1表示正在查找条件枚举的代码
	// 2表示正在查找后段部分的代码
	int state = 0;
	myVector<string> allLines = openTxtFileLines(fullPath);
	FOR_VECTOR_CONST(allLines)
	{
		if (state == 0)
		{
			preContent.push_back(allLines[i]);
		}
		else if (state == 2)
		{
			endContent.push_back(allLines[i]);
		}
		if (i > 0 && allLines[i - 1] == "enum class CONDITION : byte")
		{
			state = 1;
			continue;
		}
		if (state == 1 && i + 1 < allLines.size() && allLines[i + 1] == "};")
		{
			state = 2;
			continue;
		}
	}
}

void CodeCondition::findCSPreAndEndContent(const string& fullPath, myVector<string>& preContent, myVector<string>& endContent)
{
	// 0表示正在查找前段部分的代码
	// 1表示正在查找条件枚举的代码
	// 2表示正在查找后段部分的代码
	int state = 0;
	myVector<string> allLines = openTxtFileLines(fullPath);
	FOR_VECTOR_CONST(allLines)
	{
		if (state == 0)
		{
			preContent.push_back(allLines[i]);
		}
		else if (state == 2)
		{
			endContent.push_back(allLines[i]);
		}
		if (i > 0 && allLines[i - 1] == "public enum CONDITION : byte")
		{
			state = 1;
			continue;
		}
		if (state == 1 && i + 1 < allLines.size() && allLines[i + 1] == "};")
		{
			state = 2;
			continue;
		}
	}
}