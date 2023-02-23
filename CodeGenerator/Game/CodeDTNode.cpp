#include "CodeDTNode.h"

void CodeDTNode::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	const string cppHeaderPath = cppGamePath + "/Character/Component/DecisionTree/";

	string file;
	openTxtFile("DTNode.txt", file);
	if (file.length() == 0)
	{
		ERROR("未找文件DTNode.txt");
		return;
	}
	myVector<string> nodeLineList;
	split(file.c_str(), "\r\n", nodeLineList);
	// 生成StringDefineDTNode文件
	generateStringDefine(nodeLineList, cppStringDefinePath);
	// 生成DTNodeRegister.cpp文件
	generateRegisterFile(nodeLineList, cppHeaderPath);
}

// StringDefineDTNode.h和StringDefineDTNode.cpp
void CodeDTNode::generateStringDefine(const myVector<string>& nodeList, const string& stringDefinePath)
{
	// 头文件
	string header;
	line(header, "#ifdef _STRING_DEFINE_DTNODE_H_");
	line(header, "#error \"特殊头文件,只能被StringDefine.h所包含\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_DTNODE_H_");
	line(header, "");
	uint count = nodeList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(nodeList[i]));
	}
	line(header, "");
	line(header, "#endif", false);
	writeFile(stringDefinePath + "StringDefineDTNode.h", ANSIToUTF8(header.c_str(), true));
}

// DTNodeRegister.h和DTNodeRegister.cpp
void CodeDTNode::generateRegisterFile(const myVector<string>& nodeList, const string& headerPath)
{
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void DTNodeRegister::registeAll()");
	line(source, "{");
	const uint count = nodeList.size();
	FOR_I(count)
	{
		line(source, "\tmDTNodeFactoryManager->addFactory<" + nodeList[i] + ">(NAME(" + nodeList[i] + "));");
	}
	line(source, "}", false);

	writeFile(headerPath + "DTNodeRegister.cpp", ANSIToUTF8(source.c_str(), true));
}