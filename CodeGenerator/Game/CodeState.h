#ifndef _CODE_STATE_H_
#define _CODE_STATE_H_

#include "CodeUtility.h"

class CodeState : public CodeUtility
{
public:
	static void generateStateCode(string filePath, string headerPath);
protected:
	//c++
	static void generateCppStateTotalHeaderFile(const myVector<string>& stateList, string filePath);
	static void generateStringDefineState(const myVector<string>& stateList, string filePath);
};

#endif