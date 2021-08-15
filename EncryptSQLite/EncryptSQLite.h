#ifndef _ENCRYPT_SQLITE_H_
#define _ENCRYPT_SQLITE_H_

#include "FrameUtility.h"

class EncryptSQLite : public FrameUtility
{
public:
	bool init();
	bool encrypt();
protected:
	string mSourcePath;
	myVector<string> mTargetPath;		// 可能会有多个目标路径需要拷贝
	string mEncryptKey;
};

#endif