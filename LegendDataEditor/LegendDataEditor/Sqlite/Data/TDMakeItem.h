#ifndef _TD_MAKE_ITEM_H_
#define _TD_MAKE_ITEM_H_

#include "SQLiteData.h"

class TDMakeItem : public SQLiteData
{
public:
	static const char* ID;
	static const char* ItemName;
	static const char* Item;
	static const char* MaterialNameList;
	static const char* MaterialList;
	static const char* CountList;
public:
	int mID;
	ushort mItem;
	myVector<ushort> mMaterialList;
	myVector<byte> mCountList;
public:
	TDMakeItem()
	{
		registeParam(mID, ID);
		registeParamEmpty(ItemName);
		registeParam(mItem, Item);
		registeParamEmpty(MaterialNameList);
		registeParam(mMaterialList, MaterialList);
		registeParam(mCountList, CountList);
	}
};

#endif