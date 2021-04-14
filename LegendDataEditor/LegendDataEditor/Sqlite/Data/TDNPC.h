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
	Vector2UShort mPos;
	string mName;
	myVector<ushort> mGoods;
	myVector<byte> mRepairEquip;
	myVector<ushort> mMission;
	myVector<ushort> mTalkBubbleGroup;
	myVector<ushort> mTelentBuff;
public:
	TDNPC()
	{
		registeParam(mID, ID);
		registeParam(mMapID, MapID);
		registeParamEmpty(MapName);
		registeParam(mPos, Pos);
		registeParam(mName, Name);
		registeParamEmpty(Apperance);
		registeParamEmpty(StandAnimation0);
		registeParamEmpty(StandAnimation1);
		registeParam(mGoods, Goods);
		registeParamEmpty(MainText);
		registeParam(mRepairEquip, RepairEquip);
		registeParamEmpty(NoEquipTip);
		registeParamEmpty(NoNeedRepairTip);
		registeParam(mMission, Mission);
		registeParam(mTalkBubbleGroup, TalkBubbleGroup);
		registeParam(mTelentBuff, TelentBuff);
	}
};

#endif