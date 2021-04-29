#include "FileContent.h"
#include "StringUtility.h"

void FileContent::createBuffer(int bufferSize)
{
	mFileSize = bufferSize;
	mBuffer = NEW_ARRAY(char, mFileSize, mBuffer);
	if (mBuffer == NULL)
	{
		ERROR("create file buffer failed!");
	}
}