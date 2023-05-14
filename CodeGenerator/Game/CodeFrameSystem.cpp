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
	myVector<string> frameSystemList;
	myVector<string> factoryManagerList;
	myVector<string> classPoolList;
	int typeIndex = -1;
	FOR_VECTOR(lineList)
	{
		removeAll(lineList[i], '\t');
		if (lineList[i].length() == 0)
		{
			continue;
		}
		if (lineList[i] == "{")
		{
			if (lineList[i - 1] == "FrameSystem")
			{
				typeIndex = 0;
			}
			else if (lineList[i - 1] == "FactoryManager")
			{
				typeIndex = 1;
			}
			else if (lineList[i - 1] == "ClassPool")
			{
				typeIndex = 2;
			}
			continue;
		}
		if (lineList[i] == "}")
		{
			typeIndex = -1;
			continue;
		}
		if (typeIndex == 0)
		{
			frameSystemList.push_back(lineList[i]);
		}
		else if (typeIndex == 1)
		{
			factoryManagerList.push_back(lineList[i]);
		}
		else if (typeIndex == 2)
		{
			classPoolList.push_back(lineList[i]);
		}
	}
	END(lineList);
	myVector<string> allList;
	for (const string& item : frameSystemList)
	{
		allList.push_back(item);
	}
	for (const string& item : factoryManagerList)
	{
		allList.push_back(item);
	}
	for (const string& item : classPoolList)
	{
		allList.push_back(item);
	}
	generateFrameSystemRegister(allList, cppGamePath + "Game/Game.cpp");
	const string gameBaseHeader = cppHeaderPath + "GameBase.h";
	const string gameBaseSource = cppHeaderPath + "GameBase.cpp";
	generateFrameSystemDeclare(allList, gameBaseHeader);
	generateFrameSystemDefine(allList, gameBaseSource);
	generateFrameSystemGet(allList, gameBaseSource);
	generateFrameSystemClear(allList, gameBaseSource);
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