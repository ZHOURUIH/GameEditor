#include "CodeStateGroup.h"

void CodeStateGroup::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string stateFile = openTxtFile("StateGroup.txt");
	if (stateFile.length() == 0)
	{
		ERROR("未找文件StateGroup.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// 生成StringDefine文件
	generateStringDefineStateGroup(stateList, cppGameStringDefineFile);
}

void CodeStateGroup::generateStringDefineStateGroup(const myVector<string>& stateList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// StateGroup"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : stateList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}