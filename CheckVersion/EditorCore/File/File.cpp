#include "File.h"
#include "FileUtility.h"

void File::destroy()
{
	TRACE_DELETE(mFileData);
}

void File::startWrite(const std::string& fileName)
{
	mFileName = fileName;
	mFileData = TRACE_NEW(txSerializer, mFileData);
}

bool File::finishWrite()
{
	if (mIsBigFile)
	{
		FileUtility::writeFile(mFileName + mBigFileTempSuffix, mFileData->getDataSize(), mFileData->getBuffer(), true);
		TRACE_DELETE(mFileData);
		// 删除原文件,然后将临时文件改名为正常文件名
		FileUtility::deleteFile(mFileName);
		bool ret = FileUtility::renameFile(mFileName + mBigFileTempSuffix, mFileName);
		if (!ret)
		{
			return false;
		}
	}
	else
	{
		FileUtility::writeFile(mFileName, mFileData->getDataSize(), mFileData->getBuffer());
		TRACE_DELETE(mFileData);
	}
	return true;
}

void File::writeFile(char* data, int length)
{
	mFileData->writeBuffer(data, length);
	// 如果是大文件,缓存满了以后需要写入临时文件
	if (mIsBigFile && mFileData->getDataSize() >= BIG_FILE_SIZE)
	{
		FileUtility::writeFile(mFileName + mBigFileTempSuffix, mFileData->getDataSize(), mFileData->getBuffer(), true);
		TRACE_DELETE(mFileData);
		mFileData = TRACE_NEW(txSerializer, mFileData);
	}
}

char* File::getFileData()
{
	if (mFileData == NULL)
	{
		return NULL;
	}
	return mFileData->getBuffer();
}