#include "CodeObjectItem.h"

void CodeObjectItem::generate()
{
	myVector<string> list = findTargetHeaderFile(cppGameCorePath, [](const string& fileName) { return startWith(fileName, "ObjectItem"); },
															  [](const string& line) { return findSubstr(line, " : public ObjectItem"); });
	// 生成StringDefine文件
	CodeUtility::generateStringDefine(list, "// ObjectItem", "GameCoreStringDefine", cppGameCoreStringDefineHeaderFile, cppGameCoreStringDefineSourceFile);
}