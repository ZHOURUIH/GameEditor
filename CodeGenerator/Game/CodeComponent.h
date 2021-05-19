#ifndef _CODE_COMPONENT_H_
#define _CODE_COMPONENT_H_

#include "CodeUtility.h"

class CodeComponent : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateStringDefineComponent(const myVector<string>& componentList, string filePath);
	static void generateComponentRegister(const myVector<string>& componentList, string filePath);
protected:
};

#endif