#include "CodeCondition.h"

void CodeCondition::generate()
{
	string cppHeaderPath = cppGamePath + "ConditionManager/";
	string cppCondiitonFilePath = cppHeaderPath + "Condition/";
	string cppCondiitonEnumPath = cppGamePath + "Common/";

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
	// 生成ConditionHeader.h文件
	generateHeaderFile(conditionList, cppHeaderPath);
	// 生成ConditionRegister.h文件
	generateRegisterFile(conditionList, cppHeaderPath);
	// 生成CONDITION枚举
	generateConditionEnum(conditionList, cppCondiitonEnumPath);
	FOR_VECTOR(conditionList)
	{
		generateConditionFile(conditionList[i].first, cppCondiitonFilePath);
	}
	END(conditionList);
}

// ConditionHeader.h文件
void CodeCondition::generateHeaderFile(const myVector<pair<string, string>>& conditionList, const string& headerPath)
{
	string str0;
	line(str0, "#ifndef _CONDITION_HEADER_H_");
	line(str0, "#define _CONDITION_HEADER_H_");
	line(str0, "");
	uint count = conditionList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + conditionList[i].first + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(headerPath + "ConditionHeader.h", ANSIToUTF8(str0.c_str(), true));
}

void CodeCondition::generateRegisterFile(const myVector<pair<string, string>>& conditionList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "#define CONDITION_FACTORY(classType, type) mConditionFactoryManager->addFactory<classType>(type);");
	line(str0, "");
	line(str0, "void ConditionRegister::registeAll()");
	line(str0, "{");
	uint count = conditionList.size();
	FOR_I(count)
	{
		const string& conditionName = conditionList[i].first;
		string conditionEnum = nameToUpper(conditionName.substr(strlen("Condition")), false);
		line(str0, "\tCONDITION_FACTORY(" + conditionList[i].first + ", CONDITION::" + conditionEnum + ");");
	}
	line(str0, "}", false);

	writeFile(filePath + "ConditionRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

// CONDITION枚举
void CodeCondition::generateConditionEnum(const myVector<pair<string, string>>& conditionList, const string& filePath)
{
	myVector<string> preContent;
	myVector<string> endContent;
	findPreAndEndContent(filePath + "GameEnum.h", preContent, endContent);
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

void CodeCondition::generateConditionFile(const string& conditionName, const string& conditionPath)
{
	string headerFullPath = conditionPath + conditionName + ".h";
	if (!isFileExist(headerFullPath))
	{
		string header;
		string marcoName = nameToUpper(conditionName) + "_H_";
		string typeStr = nameToUpper(conditionName.substr(strlen("Condition")), false);
		line(header, "#ifndef " + marcoName);
		line(header, "#define " + marcoName);
		line(header, "");
		line(header, "#include \"Condition.h\"");
		line(header, "");
		line(header, "class " + conditionName + " : public Condition");
		line(header, "{");
		line(header, "BASE_CLASS(Condition)");
		line(header, "public:");
		line(header, "\t" + conditionName + "()");
		line(header, "\t{");
		line(header, "\t}");
		line(header, "\tvoid setCharacter(CharacterGame * character) override;");
		line(header, "\tvoid resetProperty() override");
		line(header, "\t{");
		line(header, "\t\tbase::resetProperty();");
		line(header, "\t}");
		line(header, "\tvoid setParam0(const string & param) override");
		line(header, "\t{");
		line(header, "\t}");
		line(header, "protected:");
		line(header, "};");
		line(header, "");
		line(header, "#endif", false);

		writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
	}

	string sourceFullPath = conditionPath + conditionName + ".cpp";
	if (!isFileExist(sourceFullPath))
	{
		string source;
		line(source, "#include \"GameHeader.h\"");
		line(source, "");
		line(source, "void " + conditionName + "::setCharacter(CharacterGame * character)");
		line(source, "{");
		line(source, "}");
		writeFile(sourceFullPath, ANSIToUTF8(source.c_str(), true));
	}
}

void CodeCondition::findPreAndEndContent(const string& fullPath, myVector<string>& preContent, myVector<string>& endContent)
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