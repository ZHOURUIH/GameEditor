#ifndef _CODE_STATE_H_
#define _CODE_STATE_H_

#include "CodeUtility.h"

class CodeState : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateStringDefineState(const myVector<string>& stateList, const string& stringDefineFile);
	static void generateStateRegister(const myVector<string>& stateList, const string& filePath);
};

#endif