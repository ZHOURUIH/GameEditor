#ifndef _TD_ERROR_CODE_H_
#define _TD_ERROR_CODE_H_

#include "SQLiteData.h"

class TDErrorCode : public SQLiteData
{
public:
	static const char* ID;
	static const char* ErrorInfo;
public:
	int mID;
	string mErrorInfo;
public:
	TDErrorCode()
	{
		registeParam(mID, ID);
		registeParam(mErrorInfo, ErrorInfo);
	}
};

#endif