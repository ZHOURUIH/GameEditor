#ifndef _SELECT_COUNT_H_
#define _SELECT_COUNT_H_

#include "ServerDefine.h"
#include "SQLiteDataReader.h"

class SelectCount
{
public:
	int mRowCount;
public:
	SelectCount()
	{
		mRowCount = 0;
	}
	void parse(SQLiteDataReader* reader)
	{
		mRowCount = reader->getInt(0);
	}
};

#endif