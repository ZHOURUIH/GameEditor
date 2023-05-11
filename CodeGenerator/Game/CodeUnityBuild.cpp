#include "CodeUnityBuild.h"

void CodeUnityBuild::generate()
{
	if (ServerProjectPath.length() == 0)
	{
		return;
	}

	// 生成UnityBuild.cpp文件
	generateCppUnityBuild(cppGameProjectPath, "UnityBuild.cpp");
	generateCppUnityBuild(cppFrameProjectPath, "UnityBuildFrame.cpp");
}

// UnityBuild.cpp
void CodeUnityBuild::generateCppUnityBuild(const string& filePath, const string& unityBuildFileName)
{
	myVector<string> fileList;
	myVector<string> patterns{ ".cpp" };
	findFiles(filePath, fileList, patterns.data(), patterns.size());
	for(string& fileName : fileList)
	{
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
	fileList.remove(unityBuildFileName);

	string str0;
	uint count = fileList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + fileList[i] + "\"", i != count - 1);
	}
	cout << "生成unity文件:" << filePath << ":" << unityBuildFileName << ",数量:" << count << endl;
	writeFile(filePath + unityBuildFileName, ANSIToUTF8(str0.c_str(), true));
}