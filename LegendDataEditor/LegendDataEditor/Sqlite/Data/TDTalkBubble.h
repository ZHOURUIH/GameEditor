#ifndef _TD_TALK_BUBBLE_H_
#define _TD_TALK_BUBBLE_H_

#include "SQLiteData.h"

class TDTalkBubble : public SQLiteData
{
public:
	static const char* ID;
	static const char* Group;
	static const char* OwnerName;
	static const char* Content;
	static const char* ShowTime;
public:
	int mID;
	ushort mGroup;
	float mShowTime;
public:
	TDTalkBubble()
	{
		registeParam(mID, ID);
		registeParam(mGroup, Group);
		registeParamEmpty(OwnerName);
		registeParamEmpty(Content);
		registeParam(mShowTime, ShowTime);
	}
};

#endif