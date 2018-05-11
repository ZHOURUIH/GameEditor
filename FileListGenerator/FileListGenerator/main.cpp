#include <iostream>
#include "FileUtility.h"
#include "StringUtility.h"
#include "txSerializer.h"
#include "md5/md5.h"
#include <fstream>
#include <sstream>
#include <memory>
#include <iomanip>
#include <exception>

const std::string VERSION = "Version";
const std::string FILELIST = "FileList";
const int MD5_BUFFER_SIZE = 1024 * 8;

class FileInfo
{
public:
	std::string mFileName;
	std::string mMD5;
	int mFileSize;
};

#define REMOVE_MAP_ELEMENT(list, element)\
{\
	txMap<std::string, FileInfo*>::iterator iter = list.find(element); \
	if (iter != list.end())\
	{\
		TRACE_DELETE(iter->second);\
		list.erase(iter);\
	}\
}\

std::string generateFileMD5(const std::string& fileName, char* buffer, const int& bufferSize, int& fileSize)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, fileName.c_str(), "rb");
	if (pFile == NULL)
	{
		return "";
	}
	fseek(pFile, 0, SEEK_END);
	fileSize = ftell(pFile);
	rewind(pFile);
	MD5 md5;
	int times = fileSize / bufferSize;
	for (int i = 0; i < times; ++i)
	{
		// 读取文件一段内存
		fread(buffer, sizeof(char), bufferSize, pFile);
		md5.update(buffer, bufferSize);
	}
	int remainLength = fileSize - bufferSize * times;
	if (remainLength > 0)
	{
		fread(buffer, sizeof(char), remainLength, pFile);
		md5.update(buffer, remainLength);
	}
	fclose(pFile);
	std::string md5Str = md5.finalize().hexdigest();
	StringUtility::strToUpper(md5Str);
	return md5Str;
}

void main()
{
	DWORD beginTime = timeGetTime();
	txVector<std::string> fileList;
	FileUtility::findFiles("./", fileList);
	txMap<std::string, FileInfo*> fileInfoList;
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		FileInfo* info = TRACE_NEW(FileInfo, info);
		std::string newPath = fileList[i].substr(strlen("./"), fileList[i].length() - strlen("./"));
		fileInfoList.insert(newPath, info);
	}
	// 排除版本文件,列表文件,程序自身
	char thisFileName[256];
	GetModuleFileNameA(NULL, thisFileName, sizeof(thisFileName));
	REMOVE_MAP_ELEMENT(fileInfoList, StringUtility::getFileName(thisFileName));
	REMOVE_MAP_ELEMENT(fileInfoList, VERSION);
	REMOVE_MAP_ELEMENT(fileInfoList, FILELIST);

	// 计算文件信息
	char* md5Buffer = TRACE_NEW_ARRAY(char, MD5_BUFFER_SIZE, md5Buffer);
	auto iter = fileInfoList.begin();
	auto iterEnd = fileInfoList.end();
	for (; iter != iterEnd; ++iter)
	{
		int curFileSize = 0;
		// 计算文件MD5码,同时可以获取文件大小
		std::string fileMD5 = generateFileMD5(iter->first, md5Buffer, MD5_BUFFER_SIZE, curFileSize);
		FileInfo* info = iter->second;
		info->mFileName = iter->first;
		info->mFileSize = curFileSize;
		info->mMD5 = fileMD5;
		std::cout << info->mFileName + "\t" + StringUtility::intToString(info->mFileSize) + "\t" + info->mMD5 << std::endl;
	}
	TRACE_DELETE_ARRAY(md5Buffer);

	// 将所有文件信息写入文件
	txSerializer serializer;
	std::string strFileCount = StringUtility::intToString(fileInfoList.size()) + "\r\n";
	serializer.writeBuffer((char*)strFileCount.c_str(), strFileCount.length());
	iter = fileInfoList.begin();
	for (; iter != iterEnd; ++iter)
	{
		FileInfo* info = iter->second;
		std::string str = info->mFileName + "\t" + StringUtility::intToString(info->mFileSize) + "\t" + info->mMD5 + "\r\n";
		serializer.writeBuffer((char*)str.c_str(), str.length());
	}
	FileUtility::writeFile(FILELIST, serializer.getDataSize(), (char*)serializer.getBuffer());
	DWORD endTime = timeGetTime();
	float time = (endTime - beginTime) / 1000.0f;
	std::cout << "文件列表生成完毕,共" << fileInfoList.size() << "个文件, 耗时 : " << time  << "秒" << std::endl;
	system("pause");
}