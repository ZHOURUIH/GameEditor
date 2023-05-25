#include "CodeState.h"

void CodeState::generate()
{
	myVector<string> buffList = findTargetHeaderFile(cppGameCorePath,
		[](const string& fileName) { return startWith(fileName, "Buff"); },
		[](const string& line) 
		{
			return findSubstr(line, " : public CharacterBuff") || 
				   findSubstr(line, " : public StrengthIncreaseBuff") || 
				   findSubstr(line, " : public EquipStrengthLevelActiveBuff") || 
				   findSubstr(line, " : public PlayerLevelIncreaseBuff") || 
				   findSubstr(line, " : public RangeCharacterBuff") || 
				   findSubstr(line, " : public RangePlayerCountMakeProperty") || 
				   findSubstr(line, " : public CharacterBuffTrigger");
		});
	myVector<string> stateActionList = findTargetHeaderFile(cppGameCorePath,
		[](const string& fileName) { return startWith(fileName, "StateAction"); },
		[](const string& line) { return findSubstr(line, " : public CharacterStateAction"); });
	myVector<string> stateBehaviourList = findTargetHeaderFile(cppGameCorePath,
		[](const string& fileName) { return startWith(fileName, "StateBehaviour"); },
		[](const string& line) { return findSubstr(line, " : public CharacterGameState"); });
	myVector<string> stateList;
	stateList.addRange(buffList);
	stateList.addRange(stateActionList);
	stateList.addRange(stateBehaviourList);
	// 生成StringDefineState文件
	generateStringDefineState(stateList, cppGameCoreStringDefineFile);
	// 生成StateRegister.cpp文件
	generateStateRegister(stateList, cppGameCorePath + "Character/Component/StateMachine/StateRegister.cpp");
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
		string enumTypeStr = nameToUpper(removeStartString(stateList[i], "Buff"), false);
		codeList.insert(++lineStart, "\tSTATE_FACTORY(" + stateList[i] + ", CHARACTER_STATE::" + enumTypeStr + ");");
	}
	codeList.insert(++lineStart, "");

	// 生成新的文件
	writeFile(filePath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}