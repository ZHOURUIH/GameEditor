#ifndef _CODE_STATE_H_
#define _CODE_STATE_H_

#include "CodeUtility.h"

class CodeState : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppStateTotalHeaderFile(const myVector<string>& stateList, const string& filePath);
	static void generateStringDefineState(const myVector<string>& stateList, const string& filePath);
	static void generateStateRegister(const myVector<string>& stateList, const string& filePath);
protected:
	static void findCustomCode(const string& fullPath, myVector<string>& preCodeList, myVector<string>& endCodeList);
};

#endif