#include "EncryptSQLite.h"

bool EncryptSQLite::init()
{
	myVector<string> configLines;
	openTxtFileLines("./EncryptConfig.txt", configLines, true, true);
	FOR_VECTOR(configLines)
	{
		removeAll(configLines[i], ' ');
		removeAll(configLines[i], '\t');
		myVector<string> params;
		split(configLines[i].c_str(), "=", params);
		if (params[0] == "SourcePath")
		{
			mSourcePath = params[1];
			continue;
		}
		if (startWith(params[0], "TargetPath"))
		{
			mTargetPath.push_back(params[1]);
		}
	}
	END(configLines);

	mEncryptKey = openTxtFile("./EncryptKey.txt", true, false);

	if (mSourcePath == "" || mTargetPath.size() == 0 || mEncryptKey == "")
	{
		ERROR("配置文件错误");
		return false;
	}
	rightToLeft(mSourcePath);
	validPath(mSourcePath);
	FOR_VECTOR(mTargetPath)
	{
		rightToLeft(mTargetPath[i]);
		validPath(mTargetPath[i]);
	}
	END(mTargetPath);
	return true;
}

bool EncryptSQLite::encrypt()
{
	myVector<string> files;
	findFiles(mSourcePath, files, "db");
	FOR_VECTOR(files)
	{
		FileContent file;
		openBinaryFile(files[i], file, true);
		if (file.mBuffer == NULL)
		{
			return false;
		}
		FOR_J(file.mFileSize)
		{
			file.mBuffer[j] ^= mEncryptKey[j % mEncryptKey.length()];
		}
		// 将加密后的文件拷贝到目标目录
		FOR_VECTOR_J(mTargetPath)
		{
			if (mTargetPath[j].length() > 0)
			{
				writeFile(mTargetPath[j] + getFileName(files[i]), file.mBuffer, file.mFileSize);
			}
		}
		END(mTargetPath);
	}
	END(files);
	return true;
}