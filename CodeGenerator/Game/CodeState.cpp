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
		codeList.insert(++lineStart, "\tSTATE_FACTORY(" + stateList[i] + ");");
	}

	// �����µ��ļ�
	writeFile(filePath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}