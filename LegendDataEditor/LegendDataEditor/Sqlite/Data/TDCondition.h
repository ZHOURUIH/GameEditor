#ifndef _TD_CONDITION_H_
#define _TD_CONDITION_H_

#include "SQLiteData.h"

class TDCondition : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
public:
	int mID;
	string mName;
public:
	TDCondition()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
	}
};

#endif