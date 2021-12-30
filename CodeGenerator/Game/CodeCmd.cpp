#include "CodeCmd.h"

void CodeCmd::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

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
	line(header, "#ifdef _STRING_DEFINE_CMD_H_");
	line(header, "#error \"特殊头文件,只能被StringDefine.h所包含\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_CMD_H_");
	line(header, "");
	uint count = cmdList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(cmdList[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefineCmd.h", ANSIToUTF8(header.c_str(), true));
}