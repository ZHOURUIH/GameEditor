#ifndef _FILE_H_
#define _FILE_H_

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "txSerializer.h"

class File : public EditorCoreBase
{
public :
	File()
	{
		mFileData = NULL;
		mTotalSize = 0;
		mCurSize = 0;
		mBigFileTempSuffix = ".temp";
	}
	virtual ~File()
	{
		destroy();
	}
	void destroy();
	void startWrite(const std::string& fileName = "");
	bool finishWrite(const std::string& md5);	// md5不为空表示需要检查文件md5,为空则不检查
	void writeFile(char* data, int length);
	void setFileName(const std::string& fileName) { mFileName = fileName; }
	const std::string& getFileName() { return mFileName; }
	const int& getTotalSize() { return mTotalSize; }
	void setTotalSize(const int& size) 
	{
		if (size < 0)
		{
			return;
		}
		mTotalSize = size;
		if (mTotalSize >= BIG_FILE_SIZE)
		{
			mIsBigFile = true;
		}
	}
	const int& getCurSize() { return mCurSize; }
	void setCurSize(const int& size) { mCurSize = size; }
	char* getFileData();
protected:
	std::string mFileName;
	std::string mBigFileTempSuffix;
	txSerializer* mFileData;
	int mTotalSize;	// 文件总大小
	int mCurSize;	// 当前已下载大小
	bool mIsBigFile;
};

#endif