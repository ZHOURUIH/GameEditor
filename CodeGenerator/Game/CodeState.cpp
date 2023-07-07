#include "CodeState.h"

void CodeState::generate()
{
	// Game
	myVector<string> gameBuffList = findTargetHeaderFile(cppGamePath,
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
	myVector<string> gameStateActionList = findTargetHeaderFile(cppGamePath,
		[](const string& fileName) { return startWith(fileName, "StateAction"); },
		[](const string& line) { return findSubstr(line, " : public CharacterStateAction"); });
	myVector<string> gameStateBehaviourList = findTargetHeaderFile(cppGamePath,
		[](const string& fileName) { return startWith(fileName, "StateBehaviour"); },
		[](const string& line) { return findSubstr(line, " : public CharacterGameState"); });
	myVector<string> gameStateList;
	gameStateList.addRange(gameBuffList);
	gameStateList.addRange(gameStateActionList);
	gameStateList.addRange(gameStateBehaviourList);
	// 生成StringDefineState文件
	myVector<string> gameStateWithParamList;
	gameStateWithParamList.addRange(gameStateList);
	for (const string& item : gameStateList)
	{
		gameStateWithParamList.push_back(item + "Param");
	}
	CodeUtility::generateStringDefine(gameStateWithParamList, "// State", "GameStringDefine", cppGameStringDefineHeaderFile, cppGameStringDefineSourceFile);
	// 生成StateRegister.cpp文件
	generateStateRegister(gameBuffList, cppGamePath + "Character/Component/StateMachine/GameStateRegister.cpp", false);

	// GameCore
	myVector<string> coreBuffList = findTargetHeaderFile(cppGameCorePath,
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
	myVector<string> coreStateActionList = findTargetHeaderFile(cppGameCorePath,
		[](const string& fileName) { return startWith(fileName, "StateAction"); },
		[](const string& line) { return findSubstr(line, " : public CharacterStateAction"); });
	myVector<string> coreStateBehaviourList = findTargetHeaderFile(cppGameCorePath,
		[](const string& fileName) { return startWith(fileName, "StateBehaviour"); },
		[](const string& line) { return findSubstr(line, " : public CharacterGameState"); });
	myVector<string> coreStateList;
	coreStateList.addRange(coreBuffList);
	coreStateList.addRange(coreStateActionList);
	coreStateList.addRange(coreStateBehaviourList);
	// 生成StringDefineState文件
	myVector<string> coreStateWithParamList;
	coreStateWithParamList.addRange(coreStateList);
	for (const string& item : coreStateList)
	{
		coreStateWithParamList.push_back(item + "Param");
	}
	CodeUtility::generateStringDefine(coreStateWithParamList, "// State", "GameCoreStringDefine", cppGameCoreStringDefineHeaderFile, cppGameCoreStringDefineSourceFile);
	// 生成StateRegister.cpp文件
	generateStateRegister(coreBuffList, cppGameCorePath + "Character/Component/StateMachine/StateRegister.cpp", true);
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

void CodeState::generateStateRegister(const myVector<string>& stateList, const string& filePath, const bool isGameCore)
{
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(filePath, codeList, lineStart,
		[](const string& codeLine) { return codeLine == "\t// auto generate start"; },
		[](const string& codeLine) { return codeLine == "\t// auto generate end"; }))
	{
		return;
	}
	myVector<string> stateRegisteList;
	FOR_VECTOR_CONST(stateList)
	{
		string enumTypeStr = nameToUpper(removeStartString(stateList[i], "Buff"), false);
		if (isGameCore)
		{
			codeList.insert(++lineStart, "\tCORE_STATE_FACTORY(" + stateList[i] + ", CORE_CHARACTER_STATE::" + enumTypeStr + ");");
		}
		else
		{
			codeList.insert(++lineStart, "\tSTATE_FACTORY(" + stateList[i] + ", CHARACTER_STATE::" + enumTypeStr + ");");
		}
	}

	// 生成新的文件
	writeFile(filePath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}