#include "CodeState.h"

void CodeState::generate()
{
	print("正在生成角色状态");
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
	// 生成StateRegister.cpp文件
	generateStateRegister(coreBuffList, cppGameCorePath + "Character/Component/StateMachine/StateRegister.cpp", true);

	myVector<string> coreStateGroupList = findTargetHeaderFile(cppGameCorePath,
		[](const string& fileName) { return startWith(fileName, "StateGroup"); },
		[](const string& line) { return findSubstr(line, " : public StateGroup"); });
	generateStringDefine(coreStateGroupList, 0, "// StateGroup", cppGameCoreStringDefineHeaderFile);

	print("完成生成角色状态");
	print("");
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
	FOR_VECTOR(stateList)
	{
		if (isGameCore)
		{
			codeList.insert(++lineStart, "\tCORE_STATE_FACTORY(" + stateList[i] + ");");
		}
		else
		{
			codeList.insert(++lineStart, "\tSTATE_FACTORY(" + stateList[i] + ");");
		}
	}

	// 生成新的文件
	writeFile(filePath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}