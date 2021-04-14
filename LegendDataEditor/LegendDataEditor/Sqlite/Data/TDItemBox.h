#ifndef _TD_ITEM_BOX_H_
#define _TD_ITEM_BOX_H_

#include "SQLiteData.h"

class TDItemBox : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Object;
	static const char* Count;
	static const char* AniCount;
public:
	int mID;
	string mName;
	myVector<ushort> mObject;
	myVector<ushort> mCount;
	byte mAniCount;
public:
	TDItemBox()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mObject, Object);
		registeParam(mCount, Count);
		registeParam(mAniCount, AniCount);
	}
};

#endif