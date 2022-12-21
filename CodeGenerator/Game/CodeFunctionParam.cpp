#include "CodeFunctionParam.h"

void CodeFunctionParam::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "FunctionParam/";

	string file;
	openTxtFile("FunctionParam.txt", file);
	if (file.length() == 0)
	{
		ERROR("未找文件FunctionParam.txt");
		return;
	}
	myVector<string> list;
	split(file.c_str(), "\r\n", list);
	// 生成StringDefineFunctionParam文件
	generateStringDefine(list, cppStringDefinePath);
	// 生成FunctionParamHeader.h文件
	generateCppTotalHeaderFile(list, cppHeaderPath);
	// 生成FunctionParamRegister文件
	generateCppRegister(list, cppHeaderPath);
}

// FunctionParamHeader.h
void CodeFunctionParam::generateCppTotalHeaderFile(const myVector<string>& list, const string& filePath)
{
	string str0;
	line(str0, "#ifndef _FUNCTION_PARAM_HEADER_H_");
	line(str0, "#define _FUNCTION_PARAM_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"FunctionParam.h\"");
	uint count = list.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + list[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "FunctionParamHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// FunctionParamRegister文件
void CodeFunctionParam::generateCppRegister(const myVector<string>& list, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "void FunctionParamRegister::registeAll()");
	line(str0, "{");
	for (const string& item : list)
	{
		line(str0, "\tmFunctionParamFactoryManager->addFactory<" + item + ">(NAME(" + item + "));");
	}
	line(str0, "}", false);

	writeFile(filePath + "FunctionParamRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineFunctionParam.h和StringDefineFunctionParam.cpp
void CodeFunctionParam::generateStringDefine(const myVector<string>& list, const string& filePath)
{
	// 头文件
	string header;
	line(header, "#ifdef _STRING_DEFINE_FUNCTION_PARAM_H_");
	line(header, "#error \"特殊头文件,只能被StringDefine.h所包含\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_FUNCTION_PARAM_H_");
	line(header, "");
	uint count = list.size();
	FOR_I(count)
	{
		line(header, stringDeclare(list[i]));
	}
	line(header, "");
	line(header, "#endif", false);
	writeFile(filePath + "StringDefineFunctionParam.h", ANSIToUTF8(header.c_str(), true));
}