#include "CodeFunctionParam.h"

void CodeFunctionParam::generate()
{
	// Game
	myVector<string> listGame = findTargetHeaderFile(cppGamePath, 
		[](const string& fileName) { return startWith(fileName, "Param"); },
		[](const string& line) { return findSubstr(line, " : public FunctionParam"); });
	// 生成StringDefineFunctionParam文件
	generateStringDefine(listGame, cppGameStringDefineFile);
	// 生成FunctionParamRegister文件
	generateCppGameRegister(listGame, cppGamePath + "FunctionParam/");

	// GameCore
	myVector<string> listGameCore = findTargetHeaderFile(cppGameCorePath, 
		[](const string& fileName) { return startWith(fileName, "Param"); },
		[](const string& line) { return findSubstr(line, " : public FunctionParam"); });
	// 生成StringDefineFunctionParam文件
	generateStringDefine(listGameCore, cppGameCoreStringDefineFile);
	// 生成FunctionParamRegister文件
	generateCppGameCoreRegister(listGameCore, cppGameCorePath + "FunctionParam/");
}

// FunctionParamRegister文件
void CodeFunctionParam::generateCppGameRegister(const myVector<string>& paramList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "void GameFunctionParamRegister::registeAll()");
	line(str0, "{");
	for (const string& item : paramList)
	{
		line(str0, "\tGameCoreBase::mFunctionParamFactoryManager->addFactory<" + item + ">(GAME_NAME(" + item + "));");
	}
	line(str0, "}", false);

	writeFile(filePath + "GameFunctionParamRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

void CodeFunctionParam::generateCppGameCoreRegister(const myVector<string>& paramList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameCoreHeader.h\"");
	line(str0, "");
	line(str0, "void GameCoreFunctionParamRegister::registeAll()");
	line(str0, "{");
	for (const string& item : paramList)
	{
		line(str0, "\tGameCoreBase::mFunctionParamFactoryManager->addFactory<" + item + ">(GAME_CORE_NAME(" + item + "));");
	}
	line(str0, "}", false);

	writeFile(filePath + "GameCoreFunctionParamRegister.cpp", ANSIToUTF8(str0.c_str(), true));
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