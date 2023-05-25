#include "CodeSkill.h"

void CodeSkill::generate()
{
	string cppHeaderPath = cppGameCorePath + "Character/Component/Skill/";

	myVector<string> list = findTargetHeaderFile(cppGameCorePath,
		[](const string& fileName) 
		{
			return startWith(fileName, "PlayerSkill_") || 
				   startWith(fileName, "MonsterSkill_") || 
				   startWith(fileName, "EnvironmentSkill_"); 
		},
		[](const string& line) 
		{
			return findSubstr(line, " : public PlayerSkill") || 
				   findSubstr(line, " : public MonsterSkill") || 
				   findSubstr(line, " : public EnvironmentSkill");
		});
	// 生成StringDefineSkill文件
	generateStringDefineSkill(list, cppGameCoreStringDefineFile);
}

void CodeSkill::generateStringDefineSkill(const myVector<string>& skillList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// Skill"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : skillList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}