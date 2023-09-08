#include "CodeComponent.h"

void CodeComponent::generate()
{
	print("正在生成组件代码");
	// Game
	const string cppGameRegisterPath = cppGamePath + "/Component/";
	myVector<string> gameComFiles = findTargetHeaderFile(cppGamePath, 
	[](const string& fileName) 
	{ 
		return startWith(fileName, "COM") && 
			   fileName != "COMCharacterGame" && 
			   fileName != "COMPlayer" && 
			   fileName != "COMNPC"; 
	},
	[](const string& line)
	{
		return findSubstr(line, " : public GameComponent") || 
			   findSubstr(line, " : public COMCharacterGame") || 
			   findSubstr(line, " : public COMPlayer") || 
			   findSubstr(line, " : public COMNPC") || 
			   findSubstr(line, " : public GameComponent");
	});
	// 生成StringDefine文件
	CodeUtility::generateStringDefine(gameComFiles, 20000, "// Component", cppGameStringDefineHeaderFile);
	// ComponentRegister.cpp
	generateGameComponentRegister(gameComFiles, cppGameRegisterPath);

	// GameCore
	const string cppGameCoreRegisterPath = cppGameCorePath + "/Component/";
	myVector<string> gameCoreComFiles = findTargetHeaderFile(cppGameCorePath,
	[](const string& fileName)
	{
		return startWith(fileName, "COM") &&
			fileName != "COMCharacterGame" &&
			fileName != "COMPlayer" &&
			fileName != "COMNPC";
	},
	[](const string& line)
	{
		return findSubstr(line, " : public GameComponent") ||
			findSubstr(line, " : public COMCharacterGame") ||
			findSubstr(line, " : public COMPlayer") ||
			findSubstr(line, " : public COMNPC") ||
			findSubstr(line, " : public GameComponent");
	});
	// 生成StringDefine文件
	CodeUtility::generateStringDefine(gameCoreComFiles, 10000, "// Component", cppGameCoreStringDefineHeaderFile);
	// ComponentRegister.cpp
	generateGameCoreComponentRegister(gameCoreComFiles, cppGameCoreRegisterPath);
	print("完成生成组件代码");
	print("");
}

// GameComponentRegister.cpp
void CodeComponent::generateGameComponentRegister(const myVector<string>& componentList, const string& filePath)
{
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void GameComponentRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR(componentList)
	{
		line(source, "\tFrameBase::mGameComponentFactoryManager->addFactory<" + componentList[i] + ">();");
	}
	line(source, "}", false);
	writeFile(filePath + "GameComponentRegister.cpp", ANSIToUTF8(source.c_str(), true));
}

// GameCoreComponentRegister.cpp
void CodeComponent::generateGameCoreComponentRegister(const myVector<string>& componentList, const string& filePath)
{
	string source;
	line(source, "#include \"GameCoreHeader.h\"");
	line(source, "");
	line(source, "void GameCoreComponentRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR(componentList)
	{
		line(source, "\tFrameBase::mGameComponentFactoryManager->addFactory<" + componentList[i] + ">();");
	}
	line(source, "}", false);
	writeFile(filePath + "GameCoreComponentRegister.cpp", ANSIToUTF8(source.c_str(), true));
}