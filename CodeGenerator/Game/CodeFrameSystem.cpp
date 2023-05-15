#include "CodeFrameSystem.h"

void CodeFrameSystem::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "Common/";

	string frameSystemFile = openTxtFile("FrameSystem.txt");
	if (frameSystemFile.length() == 0)
	{
		ERROR("未找文件FrameSystem.txt");
		return;
	}
	myVector<string> lineList;
	split(frameSystemFile.c_str(), "\r\n", lineList);
	myVector<string> classPoolList = findTargetHeaderFile(cppGamePath,
		[](const string& fileName) { return endWith(fileName, "Pool"); },
		[](const string& line) 
		{
			return findSubstr(line, " : public ClassPool<") || 
					findSubstr(line, " : public ClassTypePool<") || 
					findSubstr(line, " : public ClassKeyPool<");
		});
	myVector<string> factoryList = findTargetHeaderFile(cppGamePath, 
		[](const string& fileName) { return endWith(fileName, "FactoryManager"); }, 
		[](const string& line) { return findSubstr(line, " : public FactoryManager<"); });
	lineList.addRange(classPoolList);
	lineList.addRange(factoryList);
	generateFrameSystemRegister(lineList, cppGamePath + "Game/Game.cpp");
	const string gameBaseHeader = cppHeaderPath + "GameBase.h";
	const string gameBaseSource = cppHeaderPath + "GameBase.cpp";
	generateFrameSystemDeclare(lineList, gameBaseHeader);
	generateFrameSystemDefine(lineList, gameBaseSource);
	generateFrameSystemGet(lineList, gameBaseSource);
	generateFrameSystemClear(lineList, gameBaseSource);
}

void CodeFrameSystem::generateFrameSystemRegister(const myVector<string>& frameSystemList, const string& gameCppPath)
{
	// 更新Game.cpp的特定部分代码
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameCppPath, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// FrameSystem Register"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : frameSystemList)
	{
		codeList.insert(++lineStart, "\tregisteSystem<" + item + ">(STR(" + item + "));");
	}
	writeFile(gameCppPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeFrameSystem::generateFrameSystemClear(const myVector<string>& frameSystemList, const string& gameBaseSourceFile)
{
	// 更新GameBase.cpp的特定部分代码
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseSourceFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// FrameSystem Clear"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}
	for (const string& item : frameSystemList)
	{
		codeList.insert(++lineStart, "\tm" + item + " = nullptr;");
	}
	writeFile(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeFrameSystem::generateFrameSystemDeclare(const myVector<string>& frameSystemList, const string& gameBaseHeaderFile)
{
	// 更新GameBase.h的特定部分代码
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseHeaderFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// FrameSystem"); },
		[](const string& codeLine) { return codeLine.length() == 0; }))
	{
		return;
	}
	for (const string& item : frameSystemList)
	{
		codeList.insert(++lineStart, "\tstatic " + item + "* m" + item + ";");
	}
	writeFile(gameBaseHeaderFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeFrameSystem::generateFrameSystemDefine(const myVector<string>& frameSystemList, const string& gameBaseSourceFile)
{
	// 更新GameBase.cpp的特定部分代码
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseSourceFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// FrameSystem Define"); },
		[](const string& codeLine) { return codeLine.length() == 0; }))
	{
		return;
	}

	for (const string& item : frameSystemList)
	{
		codeList.insert(++lineStart, item + "* GameBase::m" + item + ";");
	}
	writeFile(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeFrameSystem::generateFrameSystemGet(const myVector<string>& frameSystemList, const string& gameBaseSourceFile)
{
	// 更新GameBase.cpp的特定部分代码
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseSourceFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// FrameSystem Get"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : frameSystemList)
	{
		codeList.insert(++lineStart, "\tFrameBase::mServerFramework->getSystem(STR(" + item + "), m" + item + ");");
	}
	writeFile(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}