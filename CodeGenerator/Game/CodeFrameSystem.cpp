#include "CodeFrameSystem.h"

void CodeFrameSystem::generate()
{
	print("�������ɿ�����");
	generateFrameSystem(cppGamePath, "Common/GameBase.h", "Game/Game.cpp", "GameBase", "");
	generateFrameSystem(cppFramePath, "Common/FrameBase.h", "ServerFramework/ServerFramework.cpp", "FrameBase", "MICRO_LEGEND_FRAME_API ");
	print("������ɿ�����");
	print("");
}

void CodeFrameSystem::generateFrameSystem(const string& cppPath, const string& baseFilePathNoSuffix, const string& gameFilePath, const string& baseClassName, const string& exportMacro)
{
	myVector<string> frameSystemList = findTargetHeaderFile(cppPath,
		[](const string& fileName) { return endWith(fileName, "System") || endWith(fileName, "Manager"); },
		[](const string& line)
		{
			return findSubstr(line, " : public FrameComponent") && 
				   findClassName(line) != "FactoryManager" && 
				   findClassName(line) != "ClassPool" &&
				   findClassName(line) != "ClassPoolThread" &&
				   findClassName(line) != "ClassTypePool" &&
				   findClassName(line) != "ClassTypePoolThread" &&
				   findClassName(line) != "ClassBaseTypePool" &&
				   findClassName(line) != "ClassBaseTypePoolThread" &&
				   findClassName(line) != "ClassKeyPool" && 
				   findClassName(line) != "ClassKeyPoolThread" &&
				   findClassName(line) != "ArrayPool" &&
				   findClassName(line) != "ArrayPoolThread";
		});
	myVector<string> classPoolList = findTargetHeaderFile(cppPath,
		[](const string& fileName) { return endWith(fileName, "Pool") || endWith(fileName, "PoolThread"); },
		[](const string& line)
		{
			return findSubstr(line, " : public ClassPool<") ||
				   findSubstr(line, " : public ClassPoolThread<") ||
				   findSubstr(line, " : public ClassTypePool<") ||
				   findSubstr(line, " : public ClassTypePoolThread<") ||
				   findSubstr(line, " : public ClassBaseTypePool<") ||
				   findSubstr(line, " : public ClassBaseTypePoolThread<") ||
				   findSubstr(line, " : public ClassKeyPool<") ||
				   findSubstr(line, " : public ClassKeyPoolThread<") ||
				   findSubstr(line, " : public PodPoolThread<") ||
				   findSubstr(line, " ArrayPool ") ||
				   findSubstr(line, " ArrayPoolThread ");
		});
	myVector<string> factoryList = findTargetHeaderFile(cppPath,
		[](const string& fileName) { return endWith(fileName, "FactoryManager"); },
		[](const string& line) { return findSubstr(line, " : public FactoryManager<"); });
	// ��ʱֻ�������ж�,��ServerFramework����
	if (baseClassName == "FrameBase")
	{
		frameSystemList.insert(0, "ServerFramework");
	}
	frameSystemList.addRange(classPoolList);
	frameSystemList.addRange(factoryList);
	generateFrameSystemRegister(frameSystemList, cppPath + gameFilePath);
	generateFrameSystemDeclare(frameSystemList, cppPath + baseFilePathNoSuffix, exportMacro);
	const string gameBaseSource = cppPath + replaceSuffix(baseFilePathNoSuffix, ".cpp");
	generateFrameSystemDefine(frameSystemList, gameBaseSource);
	generateFrameSystemGet(frameSystemList, gameBaseSource);
	generateFrameSystemClear(frameSystemList, gameBaseSource);
}

void CodeFrameSystem::generateFrameSystemRegister(const myVector<string>& frameSystemList, const string& gameCppPath)
{
	// ����Game.cpp���ض����ִ���
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
	// ����GameBase.cpp���ض����ִ���
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
		codeList.insert(++lineStart, "\t\tm" + item + " = nullptr;");
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#endif");
		}
	}
	writeFileIfChanged(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeFrameSystem::generateFrameSystemDeclare(const myVector<string>& frameSystemList, const string& gameBaseHeaderFile, const string& exportMacro)
{
	// ����GameBase.h���ض����ִ���
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
		codeList.insert(++lineStart, "\t" + exportMacro + "extern " + item + "* m" + item + ";");
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#endif");
		}
	}
	writeFileIfChanged(gameBaseHeaderFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeFrameSystem::generateFrameSystemDefine(const myVector<string>& frameSystemList, const string& gameBaseSourceFile)
{
	// ����GameBase.cpp���ض����ִ���
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
		codeList.insert(++lineStart, "\t" + item + "* m" + item + ";");
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#endif");
		}
	}
	writeFileIfChanged(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeFrameSystem::generateFrameSystemGet(const myVector<string>& frameSystemList, const string& gameBaseSourceFile)
{
	// ����GameBase.cpp���ض����ִ���
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
			codeList.insert(++lineStart, "\t\tmServerFramework = ServerFramework::getSingleton();");
		}
		else
		{
			codeList.insert(++lineStart, "\t\tmServerFramework->getSystem(STR(" + item + "), m" + item + ");");
		}
		if (startWith(item, "MySQL"))
		{
			codeList.insert(++lineStart, "#endif");
		}
	}
	writeFileIfChanged(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}