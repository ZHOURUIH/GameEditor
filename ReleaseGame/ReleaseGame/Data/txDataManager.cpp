#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "txDataManager.h"

#include "Utility.h"

template<>txDataManager* txSingleton<txDataManager>::ms_Singleton = 0;

txDataManager::txDataManager()
{
	throwAllToUnusedList();
}

txDataManager::~txDataManager()
{
	dump();
	DestroyAll();
}

int txDataManager::LoadData(const char *name, bool neednull)
{
	int index = -1;
	if(!getOneUnusedIndex(index))
	{
		std::cout << "error : can not get one unused index! name is : " << name << std::endl;
		return -1;
	}

#ifdef LOAD_FROM_ASSETMANAGER
	int nFileLen = ASS_getFileSize(const_cast<char*>(name));
	if(nFileLen <= 0)
	{
		ENGINE_ERROR("error : can not open file in AssetManager, path is : %s", name);
		return -1;
	}
	LOGI("file length is %d, name is : %s", nFileLen, name);

	elems[index].setSize(nFileLen + neednull);
	elems[index].setKey(name);
	throwIndexToUsedList(index);

	char* data = elems[index].getValuePtr();

	memset(data, 0, elems[index].getSize());

	ASS_loadFile((char*)name, nFileLen, data);
#else
	std::string path = Utility::getAvailableResourcePath(std::string(name));
	std::cout << "Load data from " << path << std::endl;

	//打开文件，开始载入
#ifdef _USE_SAFE_API
	FILE* fp = NULL;
	fopen_s(&fp, path.c_str(), "rb");
#else
	FILE* fp = fopen(path.c_str(), "rb");
#endif
	if (fp == NULL)
	{
		// 此处不能使用错误信息宏,因为需要允许某些文件不存在的情况
		std::cout << "can not open file in SD card, path is : " << path <<std::endl;
		return -1;
	}

	//获取文件大小
	fseek(fp, 0, SEEK_END);
	int nFileLen = ftell(fp);
	rewind(fp);

	//空文件
	if (nFileLen <= 0)
	{
		// 此处不能使用错误信息宏,因为需要允许某些文件为空的情况
		std::cout << "file len is 0, path is : " << path << std::endl;
	}

	elems[index].setSize(nFileLen + neednull);
	elems[index].setKey(name);
	throwIndexToUsedList(index);

	void *data = elems[index].getValuePtr();

	memset(data, 0, elems[index].getSize());

	int count = fread(data, 1, nFileLen, fp);
	if (count != nFileLen)
	{
		std::cout << "error : fread count != nFileLen, count is :: " << count <<  nFileLen << ", path is" << path << std::endl;
	}

	fclose(fp);
#endif

	return index;
}

bool txDataManager::isFileExist(std::string fileName)
{
	std::string path = Utility::getAvailableResourcePath(fileName);
	return Utility::isFileExist(path);
}

bool txDataManager::writeFile(const char* name, char* buffer, int bufferSize)
{
#ifndef LOAD_FROM_ASSETMANAGER
	return Utility::writeFile(name, bufferSize, buffer);
#else
	return false;
#endif
}

int txDataManager::NewData(const char *name, int sizeInByte)
{
	int index = -1;

	if(!getOneUnusedIndex(index))
	{
		std::cout << "error : can not find one unused index, in NewData, name is : " << name << std::endl;
		return -1;
	}

	elems[index].setSize(sizeInByte);
	elems[index].setKey(name);
	throwIndexToUsedList(index);

	return index;
}

int txDataManager::GetDataIndex(char *name)
{
	for (int i = 0; i < DATA_SLOTS; ++i)
	{
		if (isIndexUsed(i) && (strcmp(name, (char*) elems[i].getKeyPtr()) == 0))
		{
			return i;
		}
	}

	std::cout << "error : can not find file named : %s" <<  name;
	return -1;
}

txDataElem* txDataManager::GetData(int index)
{
	if(isIndexUsed(index))
	{
		return &(elems[index]);
	}
	else
	{
		std::cout << "error : can not get file, index is : " << index << std::endl;
		return NULL;
	}
}

int txDataManager::GetFileSize(int index)
{
#ifdef LOAD_FROM_ASSETMANAGER
	std::set<short>::iterator iter = mUsedIndexList.find(index);
	if(iter != mUsedIndexList.end())
	{
		return ASS_getFileSize(elems[*iter].getKeyPtr());
	}
	else
	{
		ENGINE_ERROR("error : can not get file size, index is : %d", index);
		return 0;
	}
#else
	if(isIndexUsed(index))
	{
		return (elems[index]).getSize();
	}
	else
	{
		std::cout << "error : can not get file size, index is : %d" << index;
		return 0;
	}
#endif
}

int txDataManager::GetFileSize(const char* filename, int* pStart, int* pLength, int* pFD)
{
#ifdef LOAD_FROM_ASSETMANAGER
	return ASS_getFileSize((char*)filename, pStart, pLength, pFD);
#else
	struct stat info;
	char path[1024];
	std::string mediaPath = Utility::getMediaPath();
#ifdef _USE_SAFE_API
	sprintf_s(path, 1024, "%s/%s", mediaPath.c_str(), filename);
#else
	sprintf(path, "%s/%s", mediaPath.c_str(), filename);
#endif
	
	stat((const char*)path, &info);
	return info.st_size;
#endif
}

void txDataManager::dump()
{
	std::cout << "======================================Begin Data Dump====================================" << std::endl;
	std::set<short>::iterator iter = mUsedIndexList.begin();
	std::set<short>::iterator iterEnd = mUsedIndexList.end();
	for( ; iter != iterEnd; ++iter)
	{
		int index = *iter;
		std::cout << "========" << index << "size=" << elems[index].getSize() << std::endl;
	}
	std::cout << "========                Total inuse : " << mUsedIndexList.size();
	std::cout << "======================================End Data Dump======================================" << std::endl;
}

void txDataManager::DestroyAll()
{
	throwAllToUnusedList();
	for(int i = 0; i < DATA_SLOTS; ++i)
	{
		elems[i].setSize(0);
		elems[i].setKey("");
	}
}

// 获取一个未使用的下标
bool txDataManager::getOneUnusedIndex(int& index)
{
	if(mUnusedIndexList.size() == 0)
	{
		std::cout << "error : there is no more space for data! please expand the reserved memory! txData::getOneUnusedIndex";
		return false;
	}
	index = *mUnusedIndexList.begin();
	return true;
}

// 将未使用列表中的一个下标放入已使用列表中,并且从未使用列表中删除该下标
bool txDataManager::throwIndexToUsedList(int index)
{
	std::set<short>::iterator iter = mUnusedIndexList.find(index);
	if(iter == mUnusedIndexList.end())
	{
		std::cout << "error : can not find index : " << index << "the index is not in unusedlist." << std::endl;
		return false;
	}
	mUnusedIndexList.erase(iter);
	mUsedIndexList.insert(index);
	return true;
}

// 将已使用列表中的一个下标放入未使用列表中,将该下标的数据销毁并且从已使用列表中删除该下标
bool txDataManager::throwIndexToUnusedList(int index)
{
	std::set<short>::iterator iter = mUsedIndexList.find(index);
	if(iter == mUsedIndexList.end())
	{
		std::cout << "error : can not find index : " << index << "the index is not in usedlist." << std::endl;
		return false;
	}
	elems[index].setSize(0);
	elems[index].setKey("");
	mUsedIndexList.erase(iter);
	mUnusedIndexList.insert(index);
	return true;
}

// 将全部下标都放入未使用列表中,并且清空已使用列表
void txDataManager::throwAllToUnusedList()
{
	// 清空已使用列表
	mUsedIndexList.clear();
	// 将所有索引放入未使用列表中
	mUnusedIndexList.clear();
	for(int i = 0; i < DATA_SLOTS; ++i)
	{
		mUnusedIndexList.insert(i);
	}
}
