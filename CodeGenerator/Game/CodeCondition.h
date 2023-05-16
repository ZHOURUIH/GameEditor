#ifndef _CODE_CONDITION_H_
#define _CODE_CONDITION_H_

#include "CodeUtility.h"

class CodeCondition : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateCppRegisterFile(const myVector<pair<string, string>>& conditionList, const string& filePath);
	static void generateCSRegisterFile(const myVector<pair<string, string>>& conditionList, const string& filePath);
};

#endif