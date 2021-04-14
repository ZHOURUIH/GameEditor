#ifndef _TD_GOODS_H_
#define _TD_GOODS_H_

#include "SQLiteData.h"

class TDGoods : public SQLiteData
{
public:
	static const char* ID;
	static const char* ShopType;
	static const char* Param;
	static const char* Name;
	static const char* Item;
	static const char* PriceGold;
public:
	int mID;
	byte mShopType;
	ushort mParam;
	ushort mItem;
	uint mPriceGold;
public:
	TDGoods()
	{
		registeParam(mID, ID);
		registeParam(mShopType, ShopType);
		registeParam(mParam, Param);
		registeParamEmpty(Name);
		registeParam(mItem, Item);
		registeParam(mPriceGold, PriceGold);
	}
};

#endif