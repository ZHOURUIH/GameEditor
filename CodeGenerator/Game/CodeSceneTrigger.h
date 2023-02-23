#ifndef _CODE_SCENE_TRIGGER_H_
#define _CODE_SCENE_TRIGGER_H_

#include "CodeUtility.h"

class CodeSceneTrigger : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateCppRegister(const myVector<string>& triggerList, const myVector<string>& modifierList, const string& filePath);
	static void generateStringDefine(const myVector<string>& list, const string& filePath);
protected:
};

#endif