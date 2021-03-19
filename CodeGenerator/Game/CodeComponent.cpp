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
	line(header, "#ifdef _STRING_DEFINE_COMPONENT_H_");
	line(header, "#error \"特殊头文件,只能被StringDefine.h所包含\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_COMPONENT_H_");
	line(header, "");
	uint count = componentList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(componentList[i]));
	}
	line(header, "");
	line(header, "#endif");
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