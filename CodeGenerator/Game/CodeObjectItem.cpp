#include "CodeObjectItem.h"

void CodeObjectItem::generate()
{
	string stateFile;
	openTxtFile("ObjectItem.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("未找文件ObjectItem.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// 生成StringDefine文件
	generateStringDefineObjectItem(stateList, cppStringDefinePath);
}

// StringDefineObjectItem.h和StringDefineObjectItem.cpp
void CodeObjectItem::generateStringDefineObjectItem(const myVector<string>& stateList, string filePath)
{
	// 头文件
	string header;
	uint cmdCount = stateList.size();
	FOR_I(cmdCount)
	{
		line(header, stringDeclare(stateList[i]));
	}

	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineObjectItem.h", header);

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, stringDefine(stateList[i]));
	}

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineObjectItem.cpp", source);
}