#ifndef _TX_DATA_MANAGER_H_
#define _TX_DATA_MANAGER_H_

#include <set>

#include "txDataElem.h"
#include "txSingleton.h"

#define DATA_SLOTS 1024 

class txDataManager : public txSingleton<txDataManager>
{
public:
	txDataManager();
	~txDataManager();
	void init(){}
	void checkError(){}
	int LoadData(const char *name, bool neednull = 0); //返回data的index -1=失败， neednull表示是否要在最后位添0

	bool writeFile(const char* name, char* buffer, int bufferSize);

	int NewData(const char *name, int sizeInByte);

	//返回0=成功 1=失败
	bool DestroyData(int index){ return throwIndexToUnusedList(index); }
	void DestroyAll(); //删除所有

	int GetDataIndex(char *name); //通过数据名称获取数据的index， 返回-1表示未找到数据
	txDataElem *GetData(int index); //通过index返回数据

	void dump(); //输出当前所有elems的情况,debug使用

	int GetFileSize(int index); //根据文件的下标得到文件大小
	int GetFileSize(const char* filename, int* pStart, int* pLength, int* pFD); //根据文件名得到文件大小

	// 参数为media下相对路径或者绝对路径,或者相对于工作目录的相对路径
	bool isFileExist(std::string fileName);
protected:
	// 获取一个未使用的下标
	bool getOneUnusedIndex(int& index);
	// 将未使用列表中的一个下标放入已使用列表中,并且从未使用列表中删除该下标
	bool throwIndexToUsedList(int index);
	// 将已使用列表中的一个下标放入未使用列表中,并且从已使用列表中删除该下标
	bool throwIndexToUnusedList(int index);
	// 下标是否已经使用
	bool isIndexUsed(int index){ return mUsedIndexList.find(index) != mUsedIndexList.end(); }
	// 下标是否未被使用
	bool isIndexUnused(int index){ return mUnusedIndexList.find(index) != mUnusedIndexList.end(); }
	// 将全部下标都放入未使用列表中,并且清空已使用列表
	void throwAllToUnusedList();

protected:
	txDataElem elems[DATA_SLOTS];
	std::set<short> mUsedIndexList; // 已使用的下标列表
	std::set<short> mUnusedIndexList; // 未使用的下标列表
};

#endif
