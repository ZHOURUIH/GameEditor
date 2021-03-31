#ifndef _CODE_SCENE_TRIGGER_H_
#define _CODE_SCENE_TRIGGER_H_

#include "CodeUtility.h"

class CodeSceneTrigger : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppTotalHeaderFile(const myVector<string>& list, string filePath);
	static void generateCppRegister(const myVector<string>& triggerList, const myVector<string>& modifierList, string filePath);
	static void generateStringDefine(const myVector<string>& list, string filePath);
protected:
};

#endif