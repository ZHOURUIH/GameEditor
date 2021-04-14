#ifndef _TD_PLAYER_SKILL_DETAIL_H_
#define _TD_PLAYER_SKILL_DETAIL_H_

#include "SQLiteData.h"

class TDPlayerSkillDetail : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Description;
	static const char* SkillID;
	static const char* Bullet;
	static const char* FireTime;
	static const char* FireEffect;
	static const char* AdditiveEffect;
	static const char* AdditiveEffectTime;
	static const char* FireBuff;
	static const char* FireSound;
	static const char* Range;
	static const char* UseMP;
	static const char* PlayerLevel;
	static const char* LevelExp;
	static const char* LevelUpCost;
	static const char* LevelUpCostName;
	static const char* LevelUpCostCount;
	static const char* FireCostObject;
	static const char* FireCostCount;
	static const char* Param0;
	static const char* Param1;
	static const char* Param2;
	static const char* Param3;
public:
	int mID;
	string mName;
	string mDescription;
	ushort mSkillID;
	myVector<ushort> mBullet;
	myVector<byte> mFireTime;
	ushort mFireEffect;
	ushort mAdditiveEffect;
	float mAdditiveEffectTime;
	myVector<ushort> mFireBuff;
	ushort mFireSound;
	ushort mRange;
	ushort mUseMP;
	ushort mPlayerLevel;
	ushort mLevelExp;
	myVector<ushort> mLevelUpCost;
	myVector<string> mLevelUpCostName;
	myVector<ushort> mLevelUpCostCount;
	ushort mFireCostObject;
	byte mFireCostCount;
	float mParam0;
	float mParam1;
	float mParam2;
	float mParam3;
public:
	TDPlayerSkillDetail()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mDescription, Description);
		registeParam(mSkillID, SkillID);
		registeParam(mBullet, Bullet);
		registeParam(mFireTime, FireTime);
		registeParam(mFireEffect, FireEffect);
		registeParam(mAdditiveEffect, AdditiveEffect);
		registeParam(mAdditiveEffectTime, AdditiveEffectTime);
		registeParam(mFireBuff, FireBuff);
		registeParam(mFireSound, FireSound);
		registeParam(mRange, Range);
		registeParam(mUseMP, UseMP);
		registeParam(mPlayerLevel, PlayerLevel);
		registeParam(mLevelExp, LevelExp);
		registeParam(mLevelUpCost, LevelUpCost);
		registeParam(mLevelUpCostName, LevelUpCostName);
		registeParam(mLevelUpCostCount, LevelUpCostCount);
		registeParam(mFireCostObject, FireCostObject);
		registeParam(mFireCostCount, FireCostCount);
		registeParam(mParam0, Param0);
		registeParam(mParam1, Param1);
		registeParam(mParam2, Param2);
		registeParam(mParam3, Param3);
	}
};

#endif