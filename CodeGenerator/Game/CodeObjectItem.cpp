#include "CodeObjectItem.h"

void CodeObjectItem::generate()
{
	myVector<string> list = findTargetHeaderFile(cppGameCorePath, [](const string& fileName) { return startWith(fileName, "ObjectItem"); },
															  [](const string& line) { return findSubstr(line, " : public ObjectItem"); });
	// 生成StringDefine文件
	generateStringDefineObjectItem(list, cppGameCoreStringDefineFile);
}

void CodeObjectItem::generateStringDefineObjectItem(const myVector<string>& objectItemList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// ObjectItem"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : objectItemList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}