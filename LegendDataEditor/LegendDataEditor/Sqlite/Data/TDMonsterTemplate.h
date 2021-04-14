#ifndef _TD_MONSTER_TEMPLATE_H_
#define _TD_MONSTER_TEMPLATE_H_

#include "SQLiteData.h"

class TDMonsterTemplate : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Race;
	static const char* RaceImage;
	static const char* Model;
	static const char* Apperance;
	static const char* MonsterRadius;
	static const char* MonsterRace;
	static const char* CoolEye;
	static const char* DieAnimation;
	static const char* StandAnimation;
	static const char* WalkAnimation;
	static const char* HitAnimation;
	static const char* BornAnimation;
	static const char* DestroyAnimation;
	static const char* RebornAnimation;
	static const char* DestroyEffect;
	static const char* BornEffect;
	static const char* DestroyTime;
public:
	int mID;
	string mName;
	byte mRace;
	byte mRaceImage;
	string mModel;
	ushort mApperance;
	byte mMonsterRadius;
	byte mMonsterRace;
	bool mCoolEye;
	ushort mDieAnimation;
	ushort mStandAnimation;
	ushort mWalkAnimation;
	ushort mHitAnimation;
	ushort mBornAnimation;
	ushort mDestroyAnimation;
	ushort mRebornAnimation;
	ushort mDestroyEffect;
	ushort mBornEffect;
	float mDestroyTime;
public:
	TDMonsterTemplate()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mRace, Race);
		registeParam(mRaceImage, RaceImage);
		registeParam(mModel, Model);
		registeParam(mApperance, Apperance);
		registeParam(mMonsterRadius, MonsterRadius);
		registeParam(mMonsterRace, MonsterRace);
		registeParam(mCoolEye, CoolEye);
		registeParam(mDieAnimation, DieAnimation);
		registeParam(mStandAnimation, StandAnimation);
		registeParam(mWalkAnimation, WalkAnimation);
		registeParam(mHitAnimation, HitAnimation);
		registeParam(mBornAnimation, BornAnimation);
		registeParam(mDestroyAnimation, DestroyAnimation);
		registeParam(mRebornAnimation, RebornAnimation);
		registeParam(mDestroyEffect, DestroyEffect);
		registeParam(mBornEffect, BornEffect);
		registeParam(mDestroyTime, DestroyTime);
	}
};

#endif