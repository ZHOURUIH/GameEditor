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
void CodeObjectItem::generateStringDefineObjectItem(const myVector<string>& objectItemList, string filePath)
{
	// 头文件
	string header;
	uint count = objectItemList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(objectItemList[i]));
	}
	writeFile(filePath + "StringDefineObjectItem.h", ANSIToUTF8(header.c_str(), true));

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(objectItemList[i]));
	}
	writeFile(filePath + "StringDefineObjectItem.cpp", ANSIToUTF8(source.c_str(), true));
}