#include "CodeUnityBuild.h"

void CodeUnityBuild::generate()
{
	string path0 = ServerProjectPath + "Frame/";
	string path1 = ServerProjectPath + "Game/";
	generateCppUnityBuild(path0, "UnityBuildFrame.cpp");
	generateCppUnityBuild(path1, "UnityBuildGame.cpp");
}

// UnityBuild.cpp
void CodeUnityBuild::generateCppUnityBuild(const string& filePath, const string& unityBuildFileName)
{
	myVector<string> fileList;
	myVector<string> patterns{ ".cpp" };
	findFiles(filePath, fileList, patterns.data(), patterns.size());
	for (string& fileName : fileList)
	{
		fileName = getFileName(fileName);
	}
	fileList.remove(unityBuildFileName);

	string str0;
	uint count = fileList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + fileList[i] + "\"", i != count - 1);
	}
	writeFileIfChanged(filePath + unityBuildFileName, ANSIToUTF8(str0.c_str(), true));
}