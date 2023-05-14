#ifndef _CODE_DT_NODE_H_
#define _CODE_DT_NODE_H_

#include "CodeUtility.h"

class CodeDTNode : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateStringDefine(const myVector<string>& nodeList, const string& stringDefineFile);
	static void generateRegisterFile(const myVector<string>& nodeList, const string& headerPath);
protected:
};

#endif