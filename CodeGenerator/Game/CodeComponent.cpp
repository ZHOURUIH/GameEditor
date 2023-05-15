#include "CodeComponent.h"

void CodeComponent::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	const string cppRegisterPath = cppGamePath + "/Component/";

	myVector<string> files = findTargetHeaderFile(cppGameProjectPath, 
	[](const string& fileName) 
	{ 
		return startWith(fileName, "COM") && 
			   fileName != "COMCharacterGame" && 
			   fileName != "COMPlayer"; 
	},
	[](const string& line)
	{
		return findSubstr(line, " : public GameComponent") || 
			   findSubstr(line, " : public COMCharacterGame") || 
			   findSubstr(line, " : public COMPlayer") || 
			   findSubstr(line, " : public GameComponent");
	});

	// 生成StringDefine文件
	generateStringDefineComponent(files, cppGameStringDefineFile);
	// ComponentRegister.cpp
	generateComponentRegister(files, cppRegisterPath);
}

void CodeComponent::generateStringDefineComponent(const myVector<string>& componentList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// Component"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : componentList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

// ComponentRegister.cpp
void CodeComponent::generateComponentRegister(const myVector<string>& componentList, const string& filePath)
{
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void ComponentRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR_CONST(componentList)
	{
		line(source, "\tFrameBase::mGameComponentFactoryManager->addFactory<" + componentList[i] + ">(NAME(" + componentList[i] + "));");
	}
	line(source, "}", false);
	writeFile(filePath + "ComponentRegister.cpp", ANSIToUTF8(source.c_str(), true));
}