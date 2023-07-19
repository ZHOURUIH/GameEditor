#pragma once

#include "CodeUtility.h"

class CodeFrameSystem : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateFrameSystem(const string& cppPath, const string& baseFilePathNoSuffix, const string& gameFilePath, const string& baseClassName);
	static void generateFrameSystemRegister(const myVector<string>& frameSystemList, const string& filePath);
	static void generateFrameSystemClear(const myVector<string>& frameSystemList, const string& gameBaseSourceFile);
	static void generateFrameSystemDeclare(const myVector<string>& frameSystemList, const string& gameBaseHeaderFile);
	static void generateFrameSystemDefine(const myVector<string>& frameSystemList, const string& gameBaseSourceFile, const string& baseClassName);
	static void generateFrameSystemGet(const myVector<string>& frameSystemList, const string& gameBaseSourceFile);
};