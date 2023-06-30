#pragma once

#include "FrameDefine.h"

class FileContent : public FrameDefine
{
public:
	FileContent()
	{
		mBuffer = NULL;
		mFileSize = 0;
	}
	virtual ~FileContent()
	{
		DELETE_ARRAY(mBuffer);
	}
	void createBuffer(int bufferSize);
public:
	char* mBuffer;
	uint mFileSize;
};