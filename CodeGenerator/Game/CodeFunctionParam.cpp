#include "CodeFunctionParam.h"

void CodeFunctionParam::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "FunctionParam/";

	string file = openTxtFile("FunctionParam.txt");
	if (file.length() == 0)
	{
		ERROR("未找文件FunctionParam.txt");
		return;
	}
	myVector<string> list;
	split(file.c_str(), "\r\n", list);
	// 生成StringDefineFunctionParam文件
	generateStringDefine(list, cppGameStringDefineFile);
	// 生成FunctionParamRegister文件
	generateCppRegister(list, cppHeaderPath);
}

// FunctionParamRegister文件
void CodeFunctionParam::generateCppRegister(const myVector<string>& paramList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "void FunctionParamRegister::registeAll()");
	line(str0, "{");
	for (const string& item : paramList)
	{
		line(str0, "\tGameBase::mFunctionParamFactoryManager->addFactory<" + item + ">(NAME(" + item + "));");
	}
	line(str0, "}", false);

	writeFile(filePath + "FunctionParamRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

void CodeFunctionParam::generateStringDefine(const myVector<string>& paramList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// FcuntionParam"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : paramList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}