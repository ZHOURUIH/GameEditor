#include "CodeClassDeclare.h"

void CodeClassDeclare::generate()
{
	if (cppFramePath.length() == 0)
	{
		return;
	}

	generateCppFrameClassAndHeader(cppFrameProjectPath, cppFramePath + "Common/");
	generateCppGameClassAndHeader(cppGameProjectPath, cppGamePath + "Common/");
	//generateCppBattleCoreClassAndHeader(cppBattleCoreProjectPath, cppBattleCorePath + "Common/");
}

void CodeClassDeclare::generateCppFrameClassAndHeader(const string& path, const string& targetFilePath)
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
		if (findSubstr(fileName, "Dependency/") != NULL)
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
	END(fileList);

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

void CodeClassDeclare::generateCppGameClassAndHeader(const string& path, const string& targetFilePath)
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

		// StringDefine的各个头文件是仅在StringDefine.h中包含的,所以不能在其他地方再次包含,否则linux下编译会报大量警告,相当于所有的源文件都定义了大量的未使用变量
		if (fileNameNoSuffix != headerFileName &&
			(!startWith(fileNameNoSuffix, "StringDefine") || fileNameNoSuffix == "StringDefine") &&
			fileNameNoSuffix != "MySQLInstanceDeclare" &&
			fileNameNoSuffix != "MySQLInstanceClear" &&
			fileNameNoSuffix != "SQLiteInstanceDeclare" &&
			fileNameNoSuffix != "SQLiteInstanceClear" &&
			fileNameNoSuffix != "FrameSystemDeclare" &&
			fileNameNoSuffix != "SQLiteSTLPoolRegister" &&
			fileNameNoSuffix != "FrameSystemGet" &&
			fileNameNoSuffix != "FrameSystemClear" &&
			fileNameNoSuffix != "FrameSystemRegiste")
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
	writeFileIfChanged(targetFilePath + "GameClassDeclare.h", ANSIToUTF8(str1.c_str(), true));

	// GameHeader.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"FrameHeader.h\"");
	uint count1 = gameHeaderList.size();
	FOR_I(count1)
	{
		line(str0, "#include \"" + gameHeaderList[i] + "\"", i != count1 - 1);
	}
	writeFileIfChanged(targetFilePath + headerFileName + ".h", ANSIToUTF8(str0.c_str(), true));
}

void CodeClassDeclare::generateCppBattleCoreClassAndHeader(const string& path, const string& targetFilePath)
{
	const string headerFileName = "BattleCoreHeader";
	myVector<string> battleCoreClassList;
	myVector<string> battleCoreHeaderList;
	myVector<string> fileList;
	findFiles(path, fileList, ".h");
	FOR_VECTOR(fileList)
	{
		const string& fileName = fileList[i];
		string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);

		// StringDefine的各个头文件是仅在StringDefine.h中包含的,所以不能在其他地方再次包含,否则linux下编译会报大量警告,相当于所有的源文件都定义了大量的未使用变量
		if (fileNameNoSuffix != headerFileName &&
			(!startWith(fileNameNoSuffix, "StringDefine") || fileNameNoSuffix == "StringDefine") &&
			fileNameNoSuffix != "MySQLInstanceDeclare" &&
			fileNameNoSuffix != "MySQLInstanceClear" &&
			fileNameNoSuffix != "SQLiteInstanceDeclare" &&
			fileNameNoSuffix != "SQLiteInstanceClear" &&
			fileNameNoSuffix != "FrameSystemDeclare" &&
			fileNameNoSuffix != "SQLiteSTLPoolRegister" &&
			fileNameNoSuffix != "FrameSystemGet" &&
			fileNameNoSuffix != "FrameSystemClear" &&
			fileNameNoSuffix != "FrameSystemRegiste")
		{
			battleCoreHeaderList.push_back(getFileName(fileName));
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
				battleCoreClassList.push_back(className);
			}
		}
	}
	END(fileList);

	// BattleCoreClassDeclare.h
	string str1;
	line(str1, "#pragma once");
	line(str1, "");
	uint count0 = battleCoreClassList.size();
	FOR_I(count0)
	{
		line(str1, "class " + battleCoreClassList[i] + ";", i != count0 - 1);
	}
	writeFileIfChanged(targetFilePath + "BattleCoreClassDeclare.h", ANSIToUTF8(str1.c_str(), true));

	// BattleCoreHeader.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"FrameHeader.h\"");
	uint count1 = battleCoreHeaderList.size();
	FOR_I(count1)
	{
		line(str0, "#include \"" + battleCoreHeaderList[i] + "\"", i != count1 - 1);
	}
	writeFileIfChanged(targetFilePath + headerFileName + ".h", ANSIToUTF8(str0.c_str(), true));
}