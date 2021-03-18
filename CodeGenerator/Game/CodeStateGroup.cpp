#include "CodeStateGroup.h"

void CodeStateGroup::generate()
{
	string stateFile;
	openTxtFile("StateGroup.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("未找文件StateGroup.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// 生成StringDefine文件
	generateStringDefineStateGroup(stateList, cppStringDefinePath);
}

// StringDefineStateGroup.h和StringDefineStateGroup.cpp
void CodeStateGroup::generateStringDefineStateGroup(const myVector<string>& stateList, string filePath)
{
	// 头文件
	string header;
	uint count = stateList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(stateList[i]));
	}
	writeFile(filePath + "StringDefineStateGroup.h", ANSIToUTF8(header.c_str(), true));

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(stateList[i]));
	}
	writeFile(filePath + "StringDefineStateGroup.cpp", ANSIToUTF8(source.c_str(), true));
}