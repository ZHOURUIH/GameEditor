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
	generateStateRegister(gameBuffList, cppGamePath + "Character/Component/StateMachine/StateRegister.cpp");
	print("完成生成角色状态");
	print("");
}

void CodeState::generateStateRegister(const myVector<string>& stateList, const string& filePath)
{
	myVector<string> newLines;
	FOR_VECTOR(stateList)
	{
		newLines.push_back("\tSTATE_FACTORY(" + stateList[i] + ");");
	}
	replaceFileLines(filePath,
		[](const string& codeLine) { return codeLine == "\t// auto generate start"; },
		[](const string& codeLine) { return codeLine == "\t// auto generate end"; }, newLines);
}