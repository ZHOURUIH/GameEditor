#include "CodeSceneTrigger.h"

void CodeSceneTrigger::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "SceneTriggerManager/";

	string file = openTxtFile("SceneTrigger.txt");
	if (file.length() == 0)
	{
		ERROR("未找文件SceneTrigger.txt");
		return;
	}
	myVector<string> lineList;
	split(file.c_str(), "\r\n", lineList);
	bool findTriggerFlag = false;
	int triggerStart = -1;
	int triggerEnd = -1;
	FOR_VECTOR(lineList)
	{
		if (lineList[i] == "Trigger")
		{
			findTriggerFlag = true;
			continue;
		}
		if (!findTriggerFlag)
		{
			continue;
		}
		removeAll(lineList[i], '\t');
		if (lineList[i] == "{")
		{
			triggerStart = i + 1;
		}
		else if (lineList[i] == "}")
		{
			triggerEnd = i - 1;
			break;
		}
	}
	END(lineList);

	bool findModifierFlag = false;
	int modifierStart = -1;
	int modifierEnd = -1;
	FOR_I(lineListCount)
	{
		if (lineList[i] == "Modifier")
		{
			findModifierFlag = true;
			continue;
		}
		if (!findModifierFlag)
		{
			continue;
		}
		removeAll(lineList[i], '\t');
		if (lineList[i] == "{")
		{
			modifierStart = i + 1;
		}
		else if (lineList[i] == "}")
		{
			modifierEnd = i - 1;
			break;
		}
	}
	int triggerCount = triggerEnd - triggerStart + 1;
	myVector<string> triggerList;
	FOR_I((uint)triggerCount)
	{
		triggerList.push_back(lineList[triggerStart + i]);
	}
	int modifierCount = modifierEnd - modifierStart + 1;
	myVector<string> modifierList;
	FOR_I((uint)modifierCount)
	{
		modifierList.push_back(lineList[modifierStart + i]);
	}
	// 生成StringDefineSceneTrigger文件
	generateStringDefine(triggerList, cppGameStringDefineFile);
	// 生成SceneTriggerRegister文件
	generateCppRegister(triggerList, modifierList, cppHeaderPath);
}

// SceneTriggerRegister文件
void CodeSceneTrigger::generateCppRegister(const myVector<string>& triggerList, const myVector<string>& modifierList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "void SceneTriggerRegister::registeAll()");
	line(str0, "{");
	// 触发器
	FOR_VECTOR_CONST(triggerList)
	{
		string enumName = nameToUpper(triggerList[i].substr(strlen("SceneTrigger")));
		enumName.erase(0, 1);
		line(str0, "\tGameBase::mSceneTriggerFactoryManager->addFactory<" + triggerList[i] + ">(SCENE_TRIGGER::" + enumName + ");");
	}
	line(str0, "");
	// 触发器的修改器
	FOR_VECTOR_CONST(modifierList)
	{
		line(str0, "\tGameBase::mTriggerModifierFactoryManager->addFactory<" + modifierList[i] + ">(" + intToString(getLastNumber(modifierList[i])) + ");");
	}
	line(str0, "}", false);

	writeFile(filePath + "SceneTriggerRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

void CodeSceneTrigger::generateStringDefine(const myVector<string>& list, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return findSubstr(codeLine, "// SceneTrigger"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : list)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}