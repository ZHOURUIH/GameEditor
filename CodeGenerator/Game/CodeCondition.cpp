#include "CodeCondition.h"

void CodeCondition::generate()
{
	string cppHeaderPath = cppGamePath + "ConditionManager/";
	string cppConditionFilePath = cppHeaderPath + "Condition/";
	string cppConditionEnumPath = cppGamePath + "Common/";
	string csRegisterPath = csHotfixGamePath + "ConditionManager/";
	string csConditionFilePath = csRegisterPath + "Condition/";
	string csConditionEnumPath = csHotfixGamePath + "Common/";

	string conditionFile;
	openTxtFile("Condition.txt", conditionFile);
	if (conditionFile.length() == 0)
	{
		ERROR("未找文件Condition.txt");
		return;
	}
	myVector<string> conditionLineList;
	split(conditionFile.c_str(), "\r\n", conditionLineList);
	myVector<pair<string, string>> conditionList;
	FOR_VECTOR(conditionLineList)
	{
		myVector<string> splitResult;
		split(conditionLineList[i].c_str(), "\t", splitResult);
		if (splitResult.size() != 2)
		{
			ERROR("条件文件解析错误:" + conditionLineList[i]);
		}
		conditionList.push_back(make_pair(splitResult[0], splitResult[1]));
	}
	END(conditionLineList);

	if (cppGamePath.length() > 0)
	{
		// c++
		// 生成ConditionRegister.h文件
		generateCppRegisterFile(conditionList, cppHeaderPath);
		// 生成CONDITION枚举
		generateCppConditionEnum(conditionList, cppConditionEnumPath);
		FOR_VECTOR(conditionList)
		{
			generateCppConditionFile(conditionList[i].first, cppConditionFilePath);
		}
		END(conditionList);
	}

	if (csHotfixGamePath.length() > 0)
	{
		// cs
		// 生成ConditionRegister.cs文件
		generateCSRegisterFile(conditionList, csRegisterPath);
		// 生成CONDITION枚举
		generateCSConditionEnum(conditionList, csConditionEnumPath);
		FOR_VECTOR(conditionList)
		{
			generateCSConditionFile(conditionList[i].first, csConditionFilePath);
		}
		END(conditionList);
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
		line(str0, "\tmConditionFactoryManager->addFactory<" + conditionName + ">(CONDITION::" + conditionEnum + ");");
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

// Condition.h和Condition.cpp
void CodeCondition::generateCppConditionFile(const string& conditionName, const string& conditionPath)
{
	string headerFullPath = conditionPath + conditionName + ".h";
	if (!isFileExist(headerFullPath))
	{
		string header;
		string typeStr = nameToUpper(conditionName.substr(strlen("Condition")), false);
		line(header, "#pragma once");
		line(header, "");
		line(header, "#include \"Condition.h\"");
		line(header, "");
		line(header, "class " + conditionName + " : public Condition");
		line(header, "{");
		line(header, "\tBASE_CLASS(Condition);");
		line(header, "public:");
		line(header, "\t" + conditionName + "()");
		line(header, "\t{");
		line(header, "\t}");
		line(header, "\tvoid setCharacter(CharacterGame* character) override;");
		line(header, "\tvoid resetProperty() override");
		line(header, "\t{");
		line(header, "\t\tbase::resetProperty();");
		line(header, "\t}");
		line(header, "\tvoid setParam0(const string& param) override");
		line(header, "\t{");
		line(header, "\t}");
		line(header, "protected:");
		line(header, "};", false);

		writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
	}

	string sourceFullPath = conditionPath + conditionName + ".cpp";
	if (!isFileExist(sourceFullPath))
	{
		string source;
		line(source, "#include \"GameHeader.h\"");
		line(source, "");
		line(source, "void " + conditionName + "::setCharacter(CharacterGame* character)");
		line(source, "{");
		line(source, "\tbase::setCharacter(character);");
		line(source, "}", false);
		writeFile(sourceFullPath, ANSIToUTF8(source.c_str(), true));
	}
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

void CodeCondition::generateCSConditionFile(const string& conditionName, const string& conditionPath)
{
	string csFullPath = conditionPath + conditionName + ".cs";
	if (!isFileExist(csFullPath))
	{
		string str;
		line(str, "using UnityEngine;");
		line(str, "using System;");
		line(str, "using System.Collections.Generic;");
		line(str, "");
		line(str, "public class " + conditionName + " : Condition");
		line(str, "{");
		line(str, "\tpublic override void init(TDConditionDetail data, CharacterGame character)");
		line(str, "\t{");
		line(str, "\t\tbase.init(data, character);");
		line(str, "\t}");
		line(str, "\tpublic override void resetProperty()");
		line(str, "\t{");
		line(str, "\t\tbase.resetProperty();");
		line(str, "\t}");
		line(str, "\tpublic override void setParam0(string param) {}");
		line(str, "\tpublic override void setValue(int value){}");
		line(str, "}", false);
		writeFile(csFullPath, ANSIToUTF8(str.c_str(), true));
	}
}

void CodeCondition::findCppPreAndEndContent(const string& fullPath, myVector<string>& preContent, myVector<string>& endContent)
{
	// 0表示正在查找前段部分的代码
	// 1表示正在查找条件枚举的代码
	// 2表示正在查找后段部分的代码
	int state = 0;
	myVector<string> allLines;
	openTxtFileLines(fullPath, allLines);
	FOR_VECTOR(allLines)
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
	END(allLines);
}

void CodeCondition::findCSPreAndEndContent(const string& fullPath, myVector<string>& preContent, myVector<string>& endContent)
{
	// 0表示正在查找前段部分的代码
	// 1表示正在查找条件枚举的代码
	// 2表示正在查找后段部分的代码
	int state = 0;
	myVector<string> allLines;
	openTxtFileLines(fullPath, allLines);
	FOR_VECTOR(allLines)
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
	END(allLines);
}