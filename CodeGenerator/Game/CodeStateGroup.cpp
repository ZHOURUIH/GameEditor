#include "CodeStateGroup.h"

void CodeStateGroup::generate()
{
	myVector<string> list = findTargetHeaderFile(cppGameCorePath, [](const string& fileName) { return startWith(fileName, "StateGroup"); },
															  [](const string& line) { return findSubstr(line, " : public StateGroup"); });
	// 生成StringDefine文件
	CodeUtility::generateStringDefine(list, "// StateGroup", "GameCoreStringDefine", cppGameCoreStringDefineHeaderFile, cppGameCoreStringDefineSourceFile);
}