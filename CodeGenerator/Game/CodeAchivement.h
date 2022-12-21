#ifndef _CODE_ACHIVEMENT_H_
#define _CODE_ACHIVEMENT_H_

#include "CodeUtility.h"

class CodeAchivement : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateHeaderFile(const myVector<string>& list, const string& headerPath);
	static void generateRegister(const myVector<string>& list, const string& headerPath);
	static void generateAchivementFile(const string& achivement, const string& achivementPath);
protected:
};

#endif