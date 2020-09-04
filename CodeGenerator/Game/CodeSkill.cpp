#include "CodeSkill.h"

void CodeSkill::generateSkillCode(string filePath, string headerPath)
{
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
	generateStringDefineSkill(skillList, filePath);
	// 生成CharacterSkillHeader.h文件
	generateCppSkillTotalHeaderFile(skillList, headerPath);
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
	line(str0, "#include \"StateInterfaceHeader.h\"");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "CharacterSkillHeader.h", str0);
}

// StringDefineSkill.h和StringDefineSkill.cpp
void CodeSkill::generateStringDefineSkill(const myVector<string>& skillList, string filePath)
{
	// 头文件
	string header;
	uint cmdCount = skillList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + skillList[i] + ");");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineSkill.h", header);

	// 源文件
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"CharacterSkillHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + skillList[i] + ");");
	}
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineSkill.cpp", source);
}