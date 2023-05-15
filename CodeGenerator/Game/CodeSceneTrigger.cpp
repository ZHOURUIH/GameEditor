#include "CodeSceneTrigger.h"

void CodeSceneTrigger::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "SceneTriggerManager/";

	myVector<string> triggerList = findTargetHeaderFile(cppGamePath, [](const string& fileName) { return startWith(fileName, "SceneTrigger"); },
																	 [](const string& line) { return findSubstr(line, " : public SceneTrigger"); });
	myVector<string> modifierList = findTargetHeaderFile(cppGamePath, [](const string& fileName) { return startWith(fileName, "TriggerModifier"); },
																	  [](const string& line) { return findSubstr(line, " : public TriggerModifier"); });
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
		[](const string& codeLine) { return endWith(codeLine, "// SceneTrigger"); },
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