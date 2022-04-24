#include "CodeUnityBuild.h"

void CodeUnityBuild::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	myVector<string> fileList;
	myVector<string> patterns{".cpp"};
	findFiles(cppProjectPath, fileList, patterns.data(), patterns.size());
	FOR_VECTOR(fileList)
	{
		string& fileName = fileList[i];
		// 如果是Dependency中的文件,则需要保留Dependency路径,但是.c是需要排除的文件,所以另外处理
		int index = 0;
		if (findSubstr(fileName, "Dependency/", &index))
		{
			fileName = fileName.substr(index + strlen("Dependency/"));
		}
		else
		{
			fileName = getFileName(fileName);
		}
	}
	END(fileList);
	fileList.remove("main.cpp");
	fileList.remove("UnityBuild.cpp");
	// 生成UnityBuild.cpp文件
	generateCppUnityBuild(fileList, cppProjectPath);
}

// UnityBuild.cpp
void CodeUnityBuild::generateCppUnityBuild(const myVector<string>& fileList, string filePath)
{
	string str0;
	uint count = fileList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + fileList[i] + "\"", i != count - 1);
	}

	writeFile(filePath + "UnityBuild.cpp", ANSIToUTF8(str0.c_str(), true));
}