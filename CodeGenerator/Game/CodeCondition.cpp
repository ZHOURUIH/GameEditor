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
	}

	if (csHotfixGamePath.length() > 0)
	{
		// cs
		// 生成ConditionRegister.cs文件
		generateCSRegisterFile(conditionPairList, csRegisterPath);
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