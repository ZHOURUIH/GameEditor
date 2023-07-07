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
	CodeUtility::generateStringDefine(list, "// Skill", "GameCoreStringDefine", cppGameCoreStringDefineHeaderFile, cppGameCoreStringDefineSourceFile);
}