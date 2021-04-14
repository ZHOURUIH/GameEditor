#ifndef _TD_TALK_BUBBLE_GROUP_H_
#define _TD_TALK_BUBBLE_GROUP_H_

#include "SQLiteData.h"

class TDTalkBubbleGroup : public SQLiteData
{
public:
	static const char* ID;
	static const char* Type;
	static const char* OwnerName;
	static const char* Description;
	static const char* TalkBubble;
	static const char* Weight;
	static const char* TalkCD;
	static const char* Probability;
public:
	int mID;
	byte mType;
	myVector<ushort> mTalkBubble;
	myVector<ushort> mWeight;
	float mTalkCD;
	float mProbability;
public:
	TDTalkBubbleGroup()
	{
		registeParam(mID, ID);
		registeParam(mType, Type);
		registeParamEmpty(OwnerName);
		registeParamEmpty(Description);
		registeParam(mTalkBubble, TalkBubble);
		registeParam(mWeight, Weight);
		registeParam(mTalkCD, TalkCD);
		registeParam(mProbability, Probability);
	}
};

#endif