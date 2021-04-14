#ifndef _TD_BUFF_H_
#define _TD_BUFF_H_

#include "SQLiteData.h"

class TDBuff : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Description;
	static const char* NotifyPlayer;
	static const char* NotifyOthers;
public:
	int mID;
	string mName;
	string mDescription;
	bool mNotifyPlayer;
	bool mNotifyOthers;
public:
	TDBuff()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mDescription, Description);
		registeParam(mNotifyPlayer, NotifyPlayer);
		registeParam(mNotifyOthers, NotifyOthers);
	}
};

#endif