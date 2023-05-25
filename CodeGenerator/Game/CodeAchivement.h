#ifndef _CODE_ACHIVEMENT_H_
#define _CODE_ACHIVEMENT_H_

#include "CodeUtility.h"

class CodeAchivement : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateAchivement(const string& path, const string& cppHeaderPath);
	static void generateRegister(const myVector<string>& list, const string& headerPath);
protected:
};

#endif