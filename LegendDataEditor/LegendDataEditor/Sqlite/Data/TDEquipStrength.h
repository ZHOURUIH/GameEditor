#ifndef _TD_EQUIP_STRENGTH_H_
#define _TD_EQUIP_STRENGTH_H_

#include "SQLiteData.h"

class TDEquipStrength : public SQLiteData
{
public:
	static const char* ID;
	static const char* SuccessOdds;
	static const char* FailedBrokenOdds;
	static const char* FailedDownLevelOdds;
	static const char* CostItemCount;
	static const char* CostGold;
public:
	int mID;
	int mSuccessOdds;
	int mFailedBrokenOdds;
	int mFailedDownLevelOdds;
	int mCostItemCount;
	int mCostGold;
public:
	TDEquipStrength()
	{
		registeParam(mID, ID);
		registeParam(mSuccessOdds, SuccessOdds);
		registeParam(mFailedBrokenOdds, FailedBrokenOdds);
		registeParam(mFailedDownLevelOdds, FailedDownLevelOdds);
		registeParam(mCostItemCount, CostItemCount);
		registeParam(mCostGold, CostGold);
	}
};

#endif