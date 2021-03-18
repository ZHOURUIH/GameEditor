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
void CodeComponent::generateStringDefineComponent(const myVector<string>& componentList, string filePath)
{
	// 头文件
	string header;
	uint count = componentList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(componentList[i]));
	}

	writeFile(filePath + "StringDefineComponent.h", ANSIToUTF8(header.c_str(), true));

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(componentList[i]));
	}

	writeFile(filePath + "StringDefineComponent.cpp", ANSIToUTF8(source.c_str(), true));
}