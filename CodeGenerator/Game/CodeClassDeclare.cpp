#include "CodeClassDeclare.h"

void CodeClassDeclare::generate()
{
	string cppFrameDeclarePath = cppFramePath + "Common/";
	string cppGameDeclarePath = cppGamePath + "Common/";

	myVector<string> frameClassList;
	myVector<string> gameClassList;
	myVector<string> frameHeaderList;
	myVector<string> gameHeaderList;
	myVector<string> fileList;
	myVector<string> patterns{".h"};
	findFiles(cppProjectPath, fileList, patterns.data(), patterns.size());
	FOR_VECTOR(fileList)
	{
		const string& fileName = fileList[i];
		// 忽略第三方库的文件
		if (findSubstr(fileName, "Dependency/") != NULL)
		{
			continue;
		}
		bool isFrameOrGame = true;
		if (findSubstr(fileName, "/Frame/") != NULL)
		{
			isFrameOrGame = true;
		}
		else if (findSubstr(fileName, "/Game/") != NULL)
		{
			isFrameOrGame = false;
		}
		else
		{
			continue;
		}
		string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);
		if (isFrameOrGame)
		{
			if (fileNameNoSuffix != "FrameHeader")
			{
				frameHeaderList.push_back(fileNameNoSuffix);
			}
		}
		else
		{
			if (fileNameNoSuffix != "GameHeader")
			{
				gameHeaderList.push_back(fileNameNoSuffix);
			}
		}
		string fileContent;
		openTxtFile(fileName, fileContent);
		myVector<string> fileLines;
		split(fileContent.c_str(), "\r\n", fileLines);
		uint lineCount = fileLines.size();
		FOR_J(lineCount)
		{
			if (startWith(fileLines[j], "class ") && 
				findSubstr(fileLines[j], ";") == NULL && 
				j > 0 && 
				findSubstr(fileLines[j - 1], "template") == NULL)
			{
				int nameStartIndex = strlen("class ");
				int index = 0;
				findSubstr(fileLines[j], " ", &index, nameStartIndex);
				if (isFrameOrGame)
				{
					frameClassList.push_back(fileLines[j].substr(nameStartIndex, index - nameStartIndex));
				}
				else
				{
					gameClassList.push_back(fileLines[j].substr(nameStartIndex, index - nameStartIndex));
				}
			}
		}
	}
	END(fileList);

	generateCppFrameClassDeclare(frameClassList, cppFrameDeclarePath);
	generateCppGameClassDeclare(gameClassList, cppGameDeclarePath);
	generateCppFrameHeader(frameHeaderList, cppFrameDeclarePath);
	generateCppGameHeader(gameHeaderList, cppGameDeclarePath);
}

// FrameClassDeclare.cpp
void CodeClassDeclare::generateCppFrameClassDeclare(const myVector<string>& classList, string filePath)
{
	string str0;
	line(str0, "#ifndef _FRAME_CLASS_DECLARE_H_");
	line(str0, "#define _FRAME_CLASS_DECLARE_H_");
	line(str0, "");
	uint count = classList.size();
	FOR_I(count)
	{
		line(str0, "class " + classList[i] + ";");
	}
	line(str0, "");
	line(str0, "#endif");

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "FrameClassDeclare.h", str0);
}

// GameClassDeclare.cpp
void CodeClassDeclare::generateCppGameClassDeclare(const myVector<string>& classList, string filePath)
{
	string str0;
	line(str0, "#ifndef _GAME_CLASS_DECLARE_H_");
	line(str0, "#define _GAME_CLASS_DECLARE_H_");
	line(str0, "");
	uint count = classList.size();
	FOR_I(count)
	{
		line(str0, "class " + classList[i] + ";");
	}
	line(str0, "");
	line(str0, "#endif");

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "GameClassDeclare.h", str0);
}

// FrameHeader.h
void CodeClassDeclare::generateCppFrameHeader(const myVector<string>& headerList, string filePath)
{
	string str0;
	line(str0, "#ifndef _FRAME_HEADER_H_");
	line(str0, "#define _FRAME_HEADER_H_");
	line(str0, "");
	uint count = headerList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + headerList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif");

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "FrameHeader.h", str0);
}

// GameHeader.h
void CodeClassDeclare::generateCppGameHeader(const myVector<string>& headerList, string filePath)
{
	string str0;
	line(str0, "#ifndef _GAME_HEADER_H_");
	line(str0, "#define _GAME_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"FrameHeader.h\"");
	uint count = headerList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + headerList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif");

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "GameHeader.h", str0);
}