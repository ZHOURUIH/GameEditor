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
	string mTargetPath;
	string mEncryptKey;
};

#endif