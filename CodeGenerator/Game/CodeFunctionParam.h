#ifndef _CODE_FUNCTION_PARAM_H_
#define _CODE_FUNCTION_PARAM_H_

#include "CodeUtility.h"

class CodeFunctionParam : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppRegister(const myVector<string>& list, const string& filePath);
	static void generateStringDefine(const myVector<string>& list, const string& filePath);
protected:
};

#endif