#ifndef _TD_ITEM_SKILL_BOOK_H_
#define _TD_ITEM_SKILL_BOOK_H_

#include "SQLiteData.h"

class TDItemSkillBook : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* SkillID;
	static const char* Job;
public:
	int mID;
	string mName;
	ushort mSkillID;
	byte mJob;
public:
	TDItemSkillBook()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mSkillID, SkillID);
		registeParam(mJob, Job);
	}
};

#endif