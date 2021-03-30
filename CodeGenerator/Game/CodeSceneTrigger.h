#ifndef _CODE_SCENE_TRIGGER_H_
#define _CODE_SCENE_TRIGGER_H_

#include "CodeUtility.h"

class CodeSceneTrigger : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppTriggerTotalHeaderFile(const myVector<string>& triggerList, string filePath);
	static void generateCppRegister(const myVector<string>& triggerList, string filePath);
	static void generateStringDefineTrigger(const myVector<string>& triggerList, string filePath);
protected:
};

#endif