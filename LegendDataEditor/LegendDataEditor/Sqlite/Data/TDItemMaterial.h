#ifndef _TD_ITEM_MATERIAL_H_
#define _TD_ITEM_MATERIAL_H_

#include "SQLiteData.h"

class TDItemMaterial : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* StdMode;
public:
	int mID;
public:
	TDItemMaterial()
	{
		registeParam(mID, ID);
		registeParamEmpty(Name);
		registeParamEmpty(StdMode);
	}
};

#endif