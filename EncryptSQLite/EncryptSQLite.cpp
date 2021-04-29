#include "EncryptSQLite.h"

bool EncryptSQLite::init()
{
	myVector<string> configLines;
	openTxtFileLines("./EncryptConfig.txt", configLines, true);
	FOR_VECTOR(configLines)
	{
		removeAll(configLines[i], ' ');
		removeAll(configLines[i], '\t');
		myVector<string> params;
		split(configLines[i].c_str(), "=", params);
		if (params[0] == "SourcePath")
		{
			mSourcePath = params[1];
		}
		else if (params[0] == "TargetPath")
		{
			mTargetPath = params[1];
		}
	}
	END(configLines);

	mEncryptKey = openTxtFile("./EncryptKey.txt", false);

	if (mSourcePath == "" || mTargetPath == "" || mEncryptKey == "")
	{
		ERROR("配置文件错误");
		return false;
	}
	rightToLeft(mSourcePath);
	rightToLeft(mTargetPath);
	if (mSourcePath[mSourcePath.length() - 1] != '/')
	{
		mSourcePath += "/";
	}
	if (mTargetPath[mTargetPath.length() - 1] != '/')
	{
		mTargetPath += "/";
	}
	return true;
}

void EncryptSQLite::encrypt()
{
	myVector<string> files;
	findFiles(mSourcePath, files, "db");
	FOR_VECTOR(files)
	{
		FileContent file;
		openBinaryFile(files[i], file);
		FOR_J(file.mFileSize)
		{
			file.mBuffer[j] ^= mEncryptKey[j % mEncryptKey.length()];
		}
		// 将加密后的文件拷贝到目标目录
		writeFileSimple(mTargetPath + getFileName(files[i]), file.mBuffer, file.mFileSize);
	}
	END(files);
}