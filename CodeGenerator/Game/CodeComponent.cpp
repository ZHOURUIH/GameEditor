#include "CodeComponent.h"

void CodeComponent::generate()
{
	string stateFile;
	openTxtFile("Component.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("未找文件Component.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// 生成StringDefine文件
	generateStringDefineComponent(stateList, cppStringDefinePath);
}

// StringDefineComponent.h和StringDefineComponent.cpp
void CodeComponent::generateStringDefineComponent(const myVector<string>& stateList, string filePath)
{
	// 头文件
	string header;
	uint cmdCount = stateList.size();
	FOR_I(cmdCount)
	{
		line(header, stringDeclare(stateList[i]));
	}

	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineComponent.h", header);

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, stringDefine(stateList[i]));
	}

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineComponent.cpp", source);
}