#ifndef _CODE_SKILL_H_
#define _CODE_SKILL_H_

#include "CodeUtility.h"

class CodeSkill : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppSkillTotalHeaderFile(const myVector<string>& skillList, const string& filePath);
	static void generateStringDefineSkill(const myVector<string>& skillList, const string& filePath);
protected:
};

#endif