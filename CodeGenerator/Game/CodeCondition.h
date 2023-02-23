#ifndef _CODE_CONDITION_H_
#define _CODE_CONDITION_H_

#include "CodeUtility.h"

class CodeCondition : public CodeUtility
{
public:
	static void generate();
protected:
	// c++
	static void generateCppRegisterFile(const myVector<pair<string, string>>& conditionList, const string& filePath);
	static void generateCppConditionEnum(const myVector<pair<string, string>>& conditionList, const string& filePath);
	static void generateCppConditionFile(const string& conditionName, const string& conditionPath);
	// c#
	static void generateCSRegisterFile(const myVector<pair<string, string>>& conditionList, const string& filePath);
	static void generateCSConditionEnum(const myVector<pair<string, string>>& conditionList, const string& filePath);
	static void generateCSConditionFile(const string& conditionName, const string& conditionPath);
protected:
	static void findCppPreAndEndContent(const string& fullPath, myVector<string>& preContent, myVector<string>& endContent);
	static void findCSPreAndEndContent(const string& fullPath, myVector<string>& preContent, myVector<string>& endContent);
};

#endif