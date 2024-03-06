#include "CodeClassDeclareAndHeader.h"

void CodeClassDeclareAndHeader::generate()
{
	print("正在生成类声明");
	generateCppFrameClassAndHeader(ServerFrameProjectPath, cppFramePath + "Common/");
	generateCppGameClassAndHeader(cppGamePath, cppGamePath + "Common/");
	generateCppGameCoreClassAndHeader(cppGameCorePath, cppGameCorePath + "Common/");
	print("完成生成类声明");
	print("");
}

void CodeClassDeclareAndHeader::generateCppFrameClassAndHeader(const string& path, const string& targetFilePath)
{
	const string headerFileName = "FrameHeader";
	myVector<string> frameClassList;
	myVector<string> frameHeaderList;
	myVector<string> fileList;
	findFiles(path, fileList, ".h");
	FOR_VECTOR(fileList)
	{
		const string& fileName = fileList[i];
		// 忽略第三方库的文件
		if (findSubstr(fileName, "Dependency/"))
		{
			continue;
		}
		string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);
		if (fileNameNoSuffix != headerFileName)
		{
			frameHeaderList.push_back(getFileName(fileName));
		}
		myVector<string> fileLines = openTxtFileLines(fileName);
		uint lineCount = fileLines.size();
		FOR_J(lineCount)
		{
			if (findSubstr(j > 0 ? fileLines[j - 1] : EMPTY_STRING, "template"))
			{
				continue;
			}
			string className = findClassName(fileLines[j]);
			if (!className.empty())
			{
				frameClassList.push_back(className);
			}
		}
	}

	// FrameClassDeclare.h
	string str1;
	line(str1, "#pragma once");
	line(str1, "");
	uint count0 = frameClassList.size();
	FOR_I(count0)
	{
		line(str1, "class " + frameClassList[i] + ";", i != count0 - 1);
	}
	writeFileIfChanged(targetFilePath + "FrameClassDeclare.h", ANSIToUTF8(str1.c_str(), true));

	// FrameHeader.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	uint count1 = frameHeaderList.size();
	FOR_I(count1)
	{
		line(str0, "#include \"" + frameHeaderList[i] + "\"", i != count1 - 1);
	}

	writeFileIfChanged(targetFilePath + headerFileName + ".h", ANSIToUTF8(str0.c_str(), true));
}

void CodeClassDeclareAndHeader::generateCppGameClassAndHeader(const string& path, const string& targetFilePath)
{
	const string headerFileName = "GameHeader";
	myVector<string> gameClassList;
	myVector<string> gameHeaderList;
	myVector<string> fileList;
	findFiles(path, fileList, ".h");
	FOR_VECTOR(fileList)
	{
		const string& fileName = fileList[i];
		const string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);

		if (fileNameNoSuffix != headerFileName)
		{
			gameHeaderList.push_back(getFileName(fileName));
		}
		myVector<string> fileLines = openTxtFileLines(fileName);
		uint lineCount = fileLines.size();
		FOR_J(lineCount)
		{
			if (findSubstr(j > 0 ? fileLines[j - 1] : EMPTY_STRING, "template"))
			{
				continue;
			}
			string className = findClassName(fileLines[j]);
			if (!className.empty())
			{
				gameClassList.push_back(className);
			}
		}
	}

	// GameClassDeclare.h
	string str1;
	line(str1, "#pragma once");
	line(str1, "");
	uint count0 = gameClassList.size();
	FOR_I(count0)
	{
		line(str1, "class " + gameClassList[i] + ";", i != count0 - 1);
	}
	writeFileIfChanged(targetFilePath + "GameClassDeclare.h", ANSIToUTF8(str1.c_str(), true));

	// GameHeader.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"GameCoreHeader.h\"");
	uint count1 = gameHeaderList.size();
	FOR_I(count1)
	{
		line(str0, "#include \"" + gameHeaderList[i] + "\"", i != count1 - 1);
	}
	writeFileIfChanged(targetFilePath + headerFileName + ".h", ANSIToUTF8(str0.c_str(), true));
}

void CodeClassDeclareAndHeader::generateCppGameCoreClassAndHeader(const string& path, const string& targetFilePath)
{
	const string headerFileName = "GameCoreHeader";
	myVector<string> gameCoreClassList;
	myVector<string> gameCoreHeaderList;
	myVector<string> fileList;
	findFiles(path, fileList, ".h");
	FOR_VECTOR(fileList)
	{
		const string& fileName = fileList[i];
		string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);

		if (fileNameNoSuffix != headerFileName)
		{
			gameCoreHeaderList.push_back(getFileName(fileName));
		}
		myVector<string> fileLines = openTxtFileLines(fileName);
		uint lineCount = fileLines.size();
		FOR_J(lineCount)
		{
			if (findSubstr(j > 0 ? fileLines[j - 1] : EMPTY_STRING, "template"))
			{
				continue;
			}
			string className = findClassName(fileLines[j]);
			if (!className.empty())
			{
				gameCoreClassList.push_back(className);
			}
		}
	}

	// GameCoreClassDeclare.h
	string str1;
	line(str1, "#pragma once");
	line(str1, "");
	uint count0 = gameCoreClassList.size();
	FOR_I(count0)
	{
		line(str1, "class " + gameCoreClassList[i] + ";", i != count0 - 1);
	}
	writeFileIfChanged(targetFilePath + "GameCoreClassDeclare.h", ANSIToUTF8(str1.c_str(), true));

	// GameCoreHeader.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"FrameHeader.h\"");
	uint count1 = gameCoreHeaderList.size();
	FOR_I(count1)
	{
		line(str0, "#include \"" + gameCoreHeaderList[i] + "\"", i != count1 - 1);
	}
	writeFileIfChanged(targetFilePath + headerFileName + ".h", ANSIToUTF8(str0.c_str(), true));
}