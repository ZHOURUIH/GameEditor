#include "CodeState.h"

void CodeState::generate()
{
	print("�������ɽ�ɫ״̬");
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
	// ����StateRegister.cpp�ļ�
	generateStateRegister(gameBuffList, cppGamePath + "Character/Component/StateMachine/StateRegister.cpp");
	print("������ɽ�ɫ״̬");
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