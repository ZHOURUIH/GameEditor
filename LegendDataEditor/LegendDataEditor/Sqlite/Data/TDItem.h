#ifndef _TD_ITEM_H_
#define _TD_ITEM_H_

#include "SQLiteData.h"

class TDItem : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Description;
	static const char* ItemType;
	static const char* Quality;
	static const char* Level;
	static const char* Shape;
	static const char* Icon;
	static const char* Price;
	static const char* CD;
	static const char* CDGroup;
	static const char* MaxCount;
	static const char* Weight;
	static const char* Trade;
public:
	int mID;
	string mName;
	byte mItemType;
	byte mQuality;
	ushort mLevel;
	uint mPrice;
	float mCD;
	short mCDGroup;
	uint mMaxCount;
	float mWeight;
	byte mTrade;
public:
	TDItem()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParamEmpty(Description);
		registeParam(mItemType, ItemType);
		registeParam(mQuality, Quality);
		registeParam(mLevel, Level);
		registeParamEmpty(Shape);
		registeParamEmpty(Icon);
		registeParam(mPrice, Price);
		registeParam(mCD, CD);
		registeParam(mCDGroup, CDGroup);
		registeParam(mMaxCount, MaxCount);
		registeParam(mWeight, Weight);
		registeParam(mTrade, Trade);
	}
};

#endif