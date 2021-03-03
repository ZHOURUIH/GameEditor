#include "CodeCmd.h"

void CodeCmd::generate()
{
	string cppHeaderPath = cppGamePath + "CommandSystem/";

	string cmdFile;
	openTxtFile("Cmd.txt", cmdFile);
	if (cmdFile.length() == 0)
	{
		ERROR("未找文件Cmd.txt");
		return;
	}
	myVector<string> cmdList;
	split(cmdFile.c_str(), "\r\n", cmdList);
	// 生成StringDefineCmd文件
	generateStringDefineCmd(cmdList, cppStringDefinePath);
	// 生成CommandHeader.h文件
	generateCppCmdTotalHeaderFile(cmdList, cppHeaderPath);
}

// CommandHeader.h文件
void CodeCmd::generateCppCmdTotalHeaderFile(const myVector<string>& cmdList, string filePath)
{
	string str0;
	line(str0, "#ifndef _COMMAND_HEADER_H_");
	line(str0, "#define _COMMAND_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"CommandHeaderBase.h\"");
	line(str0, "");
	uint count = cmdList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + cmdList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#include \"StringDefine.h\"");
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "CommandHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineCmd.h和StringDefineCmd.cpp
void CodeCmd::generateStringDefineCmd(const myVector<string>& cmdList, string filePath)
{
	// 头文件
	string header;
	uint cmdCount = cmdList.size();
	FOR_I(cmdCount)
	{
		line(header, stringDeclare(cmdList[i]));
	}

	writeFile(filePath + "StringDefineCmd.h", ANSIToUTF8(header.c_str(), true));

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, stringDefine(cmdList[i]));
	}

	writeFile(filePath + "StringDefineCmd.cpp", ANSIToUTF8(source.c_str(), true));
}