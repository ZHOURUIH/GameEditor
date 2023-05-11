#include "CodeObjectItem.h"

void CodeObjectItem::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string stateFile = openTxtFile("ObjectItem.txt");
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
void CodeObjectItem::generateStringDefineObjectItem(const myVector<string>& objectItemList, const string& filePath)
{
	// 头文件
	string header;
	line(header, "#ifdef _STRING_DEFINE_OBJECT_ITEM_H_");
	line(header, "#error \"特殊头文件,只能被StringDefine.h所包含\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_OBJECT_ITEM_H_");
	line(header, "");
	uint count = objectItemList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(objectItemList[i]));
	}
	line(header, "");
	line(header, "#endif", false);
	writeFile(filePath + "StringDefineObjectItem.h", ANSIToUTF8(header.c_str(), true));
}