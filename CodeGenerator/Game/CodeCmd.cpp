#include "CodeCmd.h"

void CodeCmd::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cmdFile = openTxtFile("Cmd.txt");
	if (cmdFile.length() == 0)
	{
		ERROR("未找文件Cmd.txt");
		return;
	}
	// 此处可直接扫描代码文件
	myVector<string> cmdList;
	split(cmdFile.c_str(), "\r\n", cmdList);
	generateStringDefineCmd(cmdList, cppGameStringDefineFile);
}

void CodeCmd::generateStringDefineCmd(const myVector<string>& cmdList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return findSubstr(codeLine, "// Cmd"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : cmdList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}