#ifndef _TD_N_P_C_H_
#define _TD_N_P_C_H_

#include "SQLiteData.h"

class TDNPC : public SQLiteData
{
public:
	static const char* ID;
	static const char* MapID;
	static const char* MapName;
	static const char* Pos;
	static const char* Name;
	static const char* Apperance;
	static const char* StandAnimation0;
	static const char* StandAnimation1;
	static const char* Goods;
	static const char* MainText;
	static const char* RepairEquip;
	static const char* NoEquipTip;
	static const char* NoNeedRepairTip;
	static const char* Mission;
	static const char* TalkBubbleGroup;
	static const char* TelentBuff;
public:
	int mID;
	ushort mMapID;
	string mMapName;
	Vector2UShort mPos;
	string mName;
	ushort mApperance;
	ushort mStandAnimation0;
	ushort mStandAnimation1;
	myVector<ushort> mGoods;
	string mMainText;
	myVector<byte> mRepairEquip;
	string mNoEquipTip;
	string mNoNeedRepairTip;
	myVector<ushort> mMission;
	myVector<ushort> mTalkBubbleGroup;
	myVector<ushort> mTelentBuff;
public:
	TDNPC()
	{
		registeParam(mID, ID);
		registeParam(mMapID, MapID);
		registeParam(mMapName, MapName);
		registeParam(mPos, Pos);
		registeParam(mName, Name);
		registeParam(mApperance, Apperance);
		registeParam(mStandAnimation0, StandAnimation0);
		registeParam(mStandAnimation1, StandAnimation1);
		registeParam(mGoods, Goods);
		registeParam(mMainText, MainText);
		registeParam(mRepairEquip, RepairEquip);
		registeParam(mNoEquipTip, NoEquipTip);
		registeParam(mNoNeedRepairTip, NoNeedRepairTip);
		registeParam(mMission, Mission);
		registeParam(mTalkBubbleGroup, TalkBubbleGroup);
		registeParam(mTelentBuff, TelentBuff);
	}
};

#endif