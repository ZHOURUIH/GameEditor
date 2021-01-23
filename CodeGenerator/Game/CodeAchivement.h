#ifndef _CODE_ACHIVEMENT_H_
#define _CODE_ACHIVEMENT_H_

#include "CodeUtility.h"

class CodeAchivement : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateHeaderFile(const myVector<string>& achivementList, string headerPath);
	static void generateAchivementRegister(const myVector<string>& achivementList, string headerPath);
	static void generateAchivementFile(const string& achivement, string achivementPath);
protected:
	const static string cppHeaderPath;
	const static string cppAchivementFilePath;
};

#endif