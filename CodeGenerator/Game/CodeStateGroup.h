#ifndef _CODE_STATE_GROUP_H_
#define _CODE_STATE_GROUP_H_

#include "CodeUtility.h"

class CodeStateGroup : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateStringDefineStateGroup(const myVector<string>& stateList, const string& filePath);
protected:
};

#endif