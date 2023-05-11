#include "CodeCmd.h"

void CodeCmd::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "CommandSystem/";

	string cmdFile = openTxtFile("Cmd.txt");
	if (cmdFile.length() == 0)
	{
		ERROR("未找文件Cmd.txt");
		return;
	}
	myVector<string> cmdList;
	split(cmdFile.c_str(), "\r\n", cmdList);
	// 生成StringDefineCmd文件
	generateStringDefineCmd(cmdList, cppStringDefinePath);
}

// StringDefineCmd.h和StringDefineCmd.cpp
void CodeCmd::generateStringDefineCmd(const myVector<string>& cmdList, const string& filePath)
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
	line(header, "#endif", false);
	writeFile(filePath + "StringDefineCmd.h", ANSIToUTF8(header.c_str(), true));
}