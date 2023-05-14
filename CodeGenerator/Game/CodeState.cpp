#include "CodeState.h"

void CodeState::generate()
{
	string stateFile = openTxtFile("State.txt");
	if (stateFile.length() == 0)
	{
		ERROR("未找文件State.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// 生成StringDefineState文件
	generateStringDefineState(stateList, cppGameStringDefineFile);
	// 生成StateRegister.cpp文件
	generateStateRegister(stateList, cppGamePath + "Character/Component/StateMachine/StateRegister.cpp");
}

void CodeState::generateStringDefineState(const myVector<string>& stateList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// State"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : stateList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	for (const string& item : stateList)
	{
		codeList.insert(++lineStart, stringDeclare(item + "Param"));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeState::generateStateRegister(const myVector<string>& stateList, const string& filePath)
{
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(filePath, codeList, lineStart,
		[](const string& codeLine) { return codeLine == "\t// buff状态"; },
		[](const string& codeLine) { return codeLine == "\t// 行为状态"; }))
	{
		return;
	}
	myVector<string> stateRegisteList;
	FOR_VECTOR_CONST(stateList)
	{
		if (startWith(stateList[i], "StateAction") || startWith(stateList[i], "StateBehaviour"))
		{
			continue;
		}
		codeList.insert(++lineStart, "\tSTATE_FACTORY(" + stateList[i] + ", CHARACTER_STATE::" + nameToUpper(stateList[i], false) + ");");
	}
	codeList.insert(++lineStart, "");

	// 生成新的文件
	writeFile(filePath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}