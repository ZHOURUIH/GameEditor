#include "FileList.h"
#include "FileInfo.h"
#include "Serializer.h"

string FileList::VERSION = "Version";
string FileList::FILELIST = "FileList";
string FileList::GAME_PDB = "GamePDB.bytes";

void FileList::generateFileList()
{
	DWORD beginTime = timeGetTime();
	myVector<string> fileList;
	findFiles("./", fileList);
	myMap<string, FileInfo*> fileInfoList;
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		string newPath = removeStartString(fileList[i], "./");
		if (endWith(newPath, ".meta"))
		{
			continue;
		}
		fileInfoList.insert(newPath, NULL);
	}
	// 排除版本文件,列表文件,程序自身
	char thisFileName[256];
	GetModuleFileNameA(NULL, thisFileName, sizeof(thisFileName));
	destroyMapElement(fileInfoList, getFileName(thisFileName));
	destroyMapElement(fileInfoList, VERSION);
	destroyMapElement(fileInfoList, FILELIST);
	destroyMapElement(fileInfoList, GAME_PDB);

	// 计算文件信息
	// 将所有文件信息写入文件
	Serializer serializer;
	string strFileCount = intToString(fileInfoList.size()) + "\r\n";
	serializer.writeBuffer((char*)strFileCount.c_str(), (uint)strFileCount.length());
	FOREACH(iter, fileInfoList)
	{
		int curFileSize = 0;
		// 计算文件MD5码,同时可以获取文件大小
		string fileMD5 = generateFileMD5(iter->first, curFileSize);
		string str = iter->first + "\t" + intToString(curFileSize) + "\t" + fileMD5 + "\r\n";
		serializer.writeBuffer((char*)str.c_str(), str.length());
		cout << str;
	}

	writeFile(FILELIST, (char*)serializer.getBuffer(), serializer.getDataSize());
	DWORD endTime = timeGetTime();
	float time = (endTime - beginTime) / 1000.0f;
	cout << "文件列表生成完毕,共" << fileInfoList.size() << "个文件, 耗时 : " << time << "秒" << endl;
}

void FileList::destroyMapElement(myMap<string, FileInfo*>& list, const string& element)
{
	auto iter = list.find(element);
	if (iter != list.end())
	{
		DELETE(iter->second);
		list.erase(iter);
	}
}