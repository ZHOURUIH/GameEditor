#include "CodeFrameSystem.h"

void CodeFrameSystem::generate()
{
	generateFrameSystem(cppGamePath, "Common/GameBase.h", "Game/Game.cpp", "GameBase");
	generateFrameSystem(cppGameCorePath, "Common/GameCoreBase.h", "GameCore/GameCore.cpp", "GameCoreBase");
	generateFrameSystem(cppFramePath, "Common/FrameBase.h", "ServerFramework/ServerFramework.cpp", "FrameBase");
}

void CodeFrameSystem::generateFrameSystem(const string& cppPath, const string& baseFilePathNoSuffix, const string& gameFilePath, const string& baseClassName)
{
	myVector<string> frameSystemList = findTargetHeaderFile(cppPath,
		[](const string& fileName) { return endWith(fileName, "System") || endWith(fileName, "Manager"); },
		[](const string& line)
		{
			return findSubstr(line, " : public FrameComponent") && 
				   findClassName(line) != "FactoryManager" && 
				   findClassName(line) != "ClassPool"&&
				   findClassName(line) != "ClassTypePool"&&
				   findClassName(line) != "ClassBaseTypePool"&&
				   findClassName(line) != "ClassKeyPool";
		});
	myVector<string> classPoolList = findTargetHeaderFile(cppPath,
		[](const string& fileName) { return endWith(fileName, "Pool"); },
		[](const string& line)
		{
			return findSubstr(line, " : public ClassPool<") ||
				   findSubstr(line, " : public ClassTypePool<") ||
				   findSubstr(line, " : public ClassBaseTypePool<") ||
				   findSubstr(line, " : public ClassKeyPool<");
		});
	myVector<string> factoryList = findTargetHeaderFile(cppPath,
		[](const string& fileName) { return endWith(fileName, "FactoryManager"); },
		[](const string& line) { return findSubstr(line, " : public FactoryManager<"); });
	// 暂时只能特殊判断,把ServerFramework加上
	if (baseClassName == "FrameBase")
	{
		frameSystemList.insert(0, "ServerFramework");
	}
	frameSystemList.addRange(classPoolList);
	frameSystemList.addRange(factoryList);
	generateFrameSystemRegister(frameSystemList, cppPath + gameFilePath);
	generateFrameSystemDeclare(frameSystemList, cppPath + baseFilePathNoSuffix);
	const string gameBaseSource = cppPath + replaceSuffix(baseFilePathNoSuffix, ".cpp");
	generateFrameSystemDefine(frameSystemList, gameBaseSource, baseClassName);
	generateFrameSystemGet(frameSystemList, gameBaseSource);
	generateFrameSystemClear(frameSystemList, gameBaseSource);
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
		if (item == "ServerFramework")
		{
			continue;
		}
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#ifdef _MYSQL");
		}
		codeList.insert(++lineStart, "\tregisteSystem<" + item + ">(STR(" + item + "));");
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#endif");
		}
	}
	writeFileIfChanged(gameCppPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
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
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#ifdef _MYSQL");
		}
		codeList.insert(++lineStart, "\tm" + item + " = nullptr;");
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#endif");
		}
	}
	writeFileIfChanged(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeFrameSystem::generateFrameSystemDeclare(const myVector<string>& frameSystemList, const string& gameBaseHeaderFile)
{
	// 更新GameBase.h的特定部分代码
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseHeaderFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// FrameSystem"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "};"); }))
	{
		return;
	}
	for (const string& item : frameSystemList)
	{
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#ifdef _MYSQL");
		}
		codeList.insert(++lineStart, "\tstatic " + item + "* m" + item + ";");
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#endif");
		}
	}
	writeFileIfChanged(gameBaseHeaderFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeFrameSystem::generateFrameSystemDefine(const myVector<string>& frameSystemList, const string& gameBaseSourceFile, const string& baseClassName)
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
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#ifdef _MYSQL");
		}
		codeList.insert(++lineStart, item + "* " + baseClassName + "::m" + item + ";");
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#endif");
		}
	}
	writeFileIfChanged(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
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
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#ifdef _MYSQL");
		}
		if (item == "ServerFramework")
		{
			codeList.insert(++lineStart, "\tmServerFramework = ServerFramework::getSingleton();");
		}
		else
		{
			codeList.insert(++lineStart, "\tFrameBase::mServerFramework->getSystem(STR(" + item + "), m" + item + ");");
		}
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#endif");
		}
	}
	writeFileIfChanged(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}