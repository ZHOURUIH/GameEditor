#include "CodeSkill.h"

void CodeSkill::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "Character/Component/Skill/";

	string skillFile = openTxtFile("Skill.txt");
	if (skillFile.length() == 0)
	{
		ERROR("未找文件Skill.txt");
		return;
	}
	myVector<string> skillList;
	split(skillFile.c_str(), "\r\n", skillList);
	// 生成StringDefineSkill文件
	generateStringDefineSkill(skillList, cppStringDefinePath);
}

// StringDefineSkill.h和StringDefineSkill.cpp
void CodeSkill::generateStringDefineSkill(const myVector<string>& skillList, const string& filePath)
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
	line(header, "#endif", false);
	writeFile(filePath + "StringDefineSkill.h", ANSIToUTF8(header.c_str(), true));
}