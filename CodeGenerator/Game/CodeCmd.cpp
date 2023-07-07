#include "CodeCmd.h"

void CodeCmd::generate()
{
	myVector<string> needDefineGameCmds = findTargetHeaderFile(cppGamePath,
		[](const string& fileName) { return startWith(fileName, "Cmd"); },
		[](const string& line) { return findSubstr(line, " : public GameCommand"); });
	CodeUtility::generateStringDefine(needDefineGameCmds, "// Cmd", "GameStringDefine", cppGameStringDefineHeaderFile, cppGameStringDefineSourceFile);

	myVector<string> needDefineGameCoreCmds = findTargetHeaderFile(cppGameCorePath,
		[](const string& fileName) { return startWith(fileName, "Cmd"); },
		[](const string& line) { return findSubstr(line, " : public GameCommand"); });
	CodeUtility::generateStringDefine(needDefineGameCoreCmds, "// Cmd", "GameCoreStringDefine", cppGameCoreStringDefineHeaderFile, cppGameCoreStringDefineSourceFile);
}