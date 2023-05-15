#include "CodeCmd.h"

void CodeCmd::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	myVector<string> needDefineCmds = findTargetHeaderFile(cppGameProjectPath, 
									[](const string& fileName) { return startWith(fileName, "Cmd"); },
									[](const string& line) { return findSubstr(line, " : public GameCommand"); });
	generateStringDefineCmd(needDefineCmds, cppGameStringDefineFile);
}

void CodeCmd::generateStringDefineCmd(const myVector<string>& cmdList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// Cmd"); },
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