#ifndef _TD_GUILD_H_
#define _TD_GUILD_H_

#include "SQLiteData.h"

class TDGuild : public SQLiteData
{
public:
	static const char* ID;
	static const char* MaxMemberCount;
	static const char* MaxExp;
public:
	int mID;
	ushort mMaxMemberCount;
	int mMaxExp;
public:
	TDGuild()
	{
		registeParam(mID, ID);
		registeParam(mMaxMemberCount, MaxMemberCount);
		registeParam(mMaxExp, MaxExp);
	}
};

#endif