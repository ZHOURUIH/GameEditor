#include "CodeState.h"

void CodeState::generate()
{
	string cppHeaderPath = cppGamePath + "Character/Component/StateMachine/";

	string stateFile;
	openTxtFile("State.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("未找文件State.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// 生成StringDefineState文件
	generateStringDefineState(stateList, cppStringDefinePath);
	// 生成StateHeader.h文件
	generateCppStateTotalHeaderFile(stateList, cppHeaderPath);
	// 生成StateRegister.cpp文件
	generateStateRegister(stateList, cppHeaderPath);
}

// StateHeader.h
void CodeState::generateCppStateTotalHeaderFile(const myVector<string>& stateList, string filePath)
{
	string str0;
	line(str0, "#ifndef _STATE_HEADER_H_");
	line(str0, "#define _STATE_HEADER_H_");
	line(str0, "");
	uint count = stateList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + stateList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "StateHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineState.h和StringDefineState.cpp
void CodeState::generateStringDefineState(const myVector<string>& stateList, string filePath)
{
	// 头文件
	string header;
	line(header, "#ifdef _STRING_DEFINE_STATE_H_");
	line(header, "#error \"特殊头文件,只能被StringDefine.h所包含\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_STATE_H_");
	line(header, "");
	uint count = stateList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(stateList[i]));
	}
	line(header, "");
	FOR_I(count)
	{
		line(header, stringDeclare(stateList[i] + "Param"));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefineState.h", ANSIToUTF8(header.c_str(), true));

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(stateList[i]));
	}
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(stateList[i] + "Param"));
	}
	writeFile(filePath + "StringDefineState.cpp", ANSIToUTF8(source.c_str(), true));
}

void CodeState::generateStateRegister(const myVector<string>& stateList, string filePath)
{
	myVector<string> preCodeList;
	myVector<string> endCodeList;
	findCustomCode(filePath + "StateRegister.cpp", preCodeList, endCodeList);
	myVector<string> stateRegisteList;
	FOR_VECTOR_CONST(stateList)
	{
		if (startWith(stateList[i], "StateAction") || startWith(stateList[i], "StateBehaviour"))
		{
			continue;
		}
		stateRegisteList.push_back("STATE_FACTORY(" + stateList[i] + ", CHARACTER_STATE::" + nameToUpper(stateList[i], false) + ");");
	}

	// 生成新的文件
	string source;
	FOR_VECTOR(preCodeList)
	{
		line(source, preCodeList[i]);
	}
	END(preCodeList);
	FOR_VECTOR(stateRegisteList)
	{
		line(source, "\t" + stateRegisteList[i]);
	}
	END(stateRegisteList);
	line(source, "");
	FOR_VECTOR(endCodeList)
	{
		line(source, endCodeList[i]);
	}
	END(endCodeList);
	writeFile(filePath + "StateRegister.cpp", ANSIToUTF8(source.c_str(), true));
}

void CodeState::findCustomCode(const string& fullPath, myVector<string>& preCodeList, myVector<string>& endCodeList)
{
	myVector<string> fileLines;
	openTxtFileLines(fullPath, fileLines);
	int preCodeEnd = -1;
	int endCodeStart = -1;
	FOR_VECTOR(fileLines)
	{
		if (preCodeEnd < 0 && fileLines[i] == "\t// buff状态")
		{
			preCodeEnd = i;
			continue;
		}
		if (endCodeStart < 0 && fileLines[i] == "\t// 行为状态")
		{
			endCodeStart = i;
			break;
		}
	}
	END(fileLines);
	if (preCodeEnd < 0 || endCodeStart < 0)
	{
		cout << "状态注册代码文件解析错误" << endl;
		return;
	}
	FOR_I(fileLinesCount)
	{
		if ((int)i <= preCodeEnd)
		{
			preCodeList.push_back(fileLines[i]);
		}
		else if ((int)i >= endCodeStart)
		{
			endCodeList.push_back(fileLines[i]);
		}
	}
}