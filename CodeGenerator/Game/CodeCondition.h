#ifndef _CODE_CONDITION_H_
#define _CODE_CONDITION_H_

#include "CodeUtility.h"

class CodeCondition : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateHeaderFile(const myVector<pair<string, string>>& conditionList, const string& headerPath);
	static void generateRegisterFile(const myVector<pair<string, string>>& conditionList, const string& filePath);
	static void generateConditionEnum(const myVector<pair<string, string>>& conditionList, const string& filePath);
	static void generateConditionFile(const string& conditionName, const string& conditionPath);
protected:
	static void findPreAndEndContent(const string& fullPath, myVector<string>& preContent, myVector<string>& endContent);
};

#endif