#include "CodeClassDeclareAndHeader.h"

void CodeClassDeclareAndHeader::generate()
{
	if (!ClientProjectPath.empty())
	{
		string clientPath0 = ClientProjectPath + "/Plugins/NetTCP/Source/";
		generateCppGameClassAndHeader(clientPath0, clientPath0 + "Common/", "FrameClassDeclare", "FrameHeader");

		string clientPath1 = ClientProjectPath + "/Source/Reflame/";
		generateCppGameClassAndHeader(clientPath1, clientPath1 + "Common/", "GameClassDeclare", "GameHeader");
	}

	if (!ServerProjectPath.empty())
	{
		string serverPath0 = ServerProjectPath + "/Frame/";
		generateCppGameClassAndHeader(serverPath0, serverPath0 + "Common/", "FrameClassDeclare", "FrameHeader");

		string serverPath1 = ServerProjectPath + "/Game/";
		generateCppGameClassAndHeader(serverPath1, serverPath1 + "Common/", "GameClassDeclare", "GameHeader");
	}
}

void CodeClassDeclareAndHeader::generateCppGameClassAndHeader(const string& path, const string& targetFilePath, const string& declareFileName, const string& headerFileName)
{
	myVector<string> gameClassList;
	myVector<string> gameHeaderList;
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
		if (getFileNameNoSuffix(fileName, true) != headerFileName)
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
	END(fileList);

	// GameClassDeclare.h
	string str1;
	line(str1, "#pragma once");
	line(str1, "");
	uint count0 = gameClassList.size();
	FOR_I(count0)
	{
		line(str1, "class " + gameClassList[i] + ";", i != count0 - 1);
	}
	writeFileIfChanged(targetFilePath + declareFileName + ".h", ANSIToUTF8(str1.c_str(), true));

	// GameHeader.h
	gameHeaderList.insert(0, "FrameHeader.h");
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	uint count1 = gameHeaderList.size();
	FOR_I(count1)
	{
		line(str0, "#include \"" + gameHeaderList[i] + "\"", i != count1 - 1);
	}
	writeFileIfChanged(targetFilePath + headerFileName + ".h", ANSIToUTF8(str0.c_str(), true));
}