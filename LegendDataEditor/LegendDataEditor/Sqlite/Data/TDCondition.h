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
public:
	TDCondition()
	{
		registeParam(mID, ID);
		registeParamEmpty(Name);
	}
};

#endif