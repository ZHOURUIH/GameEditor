#include "CodeSkill.h"

void CodeSkill::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "Character/Component/Skill/";

	string skillFile;
	openTxtFile("Skill.txt", skillFile);
	if (skillFile.length() == 0)
	{
		ERROR("未找文件Skill.txt");
		return;
	}
	myVector<string> skillList;
	split(skillFile.c_str(), "\r\n", skillList);
	// 生成StringDefineSkill文件
	generateStringDefineSkill(skillList, cppStringDefinePath);
	// 生成CharacterSkillHeader.h文件
	generateCppSkillTotalHeaderFile(skillList, cppHeaderPath);
}

// CharacterSkillHeader.h
void CodeSkill::generateCppSkillTotalHeaderFile(const myVector<string>& skillList, string filePath)
{
	string str0;
	line(str0, "#ifndef _CHARACTER_SKILL_HEADER_H_");
	line(str0, "#define _CHARACTER_SKILL_HEADER_H_");
	line(str0, "");
	uint count = skillList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + skillList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "CharacterSkillHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineSkill.h和StringDefineSkill.cpp
void CodeSkill::generateStringDefineSkill(const myVector<string>& skillList, string filePath)
{
	// 头文件
	string header;
	line(header, "#ifdef _STRING_DEFINE_SKILL_H_");
	line(header, "#error \"特殊头文件,只能被StringDefine.h所包含\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_SKILL_H_");
	line(header, "");
	uint count = skillList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(skillList[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefineSkill.h", ANSIToUTF8(header.c_str(), true));

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(skillList[i]));
	}
	writeFile(filePath + "StringDefineSkill.cpp", ANSIToUTF8(source.c_str(), true));
}