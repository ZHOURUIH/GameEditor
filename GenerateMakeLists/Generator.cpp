#include "Generator.h"

myMap<string, string> Generator::readConfig(const string& configPath)
{
	myVector<string> lines;
	openTxtFileLines(configPath, lines, false);
	myMap<string, string> configList;
	FOR_VECTOR(lines)
	{
		myVector<string> params;
		split(lines[i].c_str(), "=", params);
		if (params.size() == 2)
		{
			configList.insert(params[0], params[1]);
		}
	}
	END(lines);
	return configList;
}

myVector<string> Generator::generateDirectory(const string& path, const myVector<string>& exceptList)
{
	myVector<string> newExceptList;
	FOR_VECTOR_CONST(exceptList)
	{
		newExceptList.push_back(path + "/" + exceptList[i]);
	}
	myVector<string> pathList;
	findFolders(path, pathList, true);
	removeIgnorePath(pathList, newExceptList);
	return pathList;
}

myVector<string> Generator::generateSourceFile(const string& path, const myVector<string>& exceptDirList, const myVector<string>& exceptFileList)
{
	myVector<string> fileList;
	myVector<string> patterns{ ".cpp" , ".c" };
	findFiles(path, fileList, patterns.data(), patterns.size());
	removeIgnoreFile(fileList, exceptFileList);
	return fileList;
}

void Generator::removeIgnorePath(myVector<string>& pathList, const myVector<string>& ignoreList)
{
	uint ignoreCount = ignoreList.size();
	for (int i = 0; i < (int)pathList.size(); ++i)
	{
		FOR_J(ignoreCount)
		{
			if (startWith(pathList[i], ignoreList[j].c_str()))
			{
				pathList.erase(pathList.begin() + i);
				--i;
				break;
			}
		}
	}
}

void Generator::removeIgnoreFile(myVector<string>& fileList, const myVector<string>& ignoreList)
{
	for (uint i = 0; i < fileList.size(); ++i)
	{
		if (ignoreList.contains(getFileName(fileList[i])))
		{
			fileList.erase(i, 1);
			--i;
		}
	}
}