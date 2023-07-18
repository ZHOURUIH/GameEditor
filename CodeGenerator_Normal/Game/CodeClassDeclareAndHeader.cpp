#include "CodeClassDeclareAndHeader.h"

void CodeClassDeclareAndHeader::generate()
{
	string path0 = ClientProjectPath + "/Plugins/NetTCP/Source/";
	generateCppGameClassAndHeader(path0, path0 + "Common/", "NetTCPClassDeclare", "NetTCPHeader");

	string path1 = ServerProjectPath + "/Frame";
	generateCppGameClassAndHeader(path1, path1 + "Common/", "FrameClassDeclare", "FrameHeader");

	string path = ServerProjectPath + "/Game";
	generateCppGameClassAndHeader(path, path + "Common/", "GameClassDeclare", "GameHeader");
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