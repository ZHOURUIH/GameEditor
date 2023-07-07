#ifndef _CODE_FUNCTION_PARAM_H_
#define _CODE_FUNCTION_PARAM_H_

#include "CodeUtility.h"

class CodeFunctionParam : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppGameRegister(const myVector<string>& paramList, const string& stringDefineFile);
	static void generateCppGameCoreRegister(const myVector<string>& paramList, const string& stringDefineFile);
protected:
};

#endif