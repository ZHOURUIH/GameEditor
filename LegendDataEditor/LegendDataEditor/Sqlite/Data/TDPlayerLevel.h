#ifndef _TD_PLAYER_LEVEL_H_
#define _TD_PLAYER_LEVEL_H_

#include "SQLiteData.h"

class TDPlayerLevel : public SQLiteData
{
public:
	static const char* ID;
	static const char* Exp;
public:
	int mID;
	uint mExp;
public:
	TDPlayerLevel()
	{
		registeParam(mID, ID);
		registeParam(mExp, Exp);
	}
};

#endif