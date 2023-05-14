#include "CodeObjectItem.h"

void CodeObjectItem::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string stateFile = openTxtFile("ObjectItem.txt");
	if (stateFile.length() == 0)
	{
		ERROR("未找文件ObjectItem.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// 生成StringDefine文件
	generateStringDefineObjectItem(stateList, cppGameStringDefineFile);
}

void CodeObjectItem::generateStringDefineObjectItem(const myVector<string>& objectItemList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return findSubstr(codeLine, "// ObjectItem"); },
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