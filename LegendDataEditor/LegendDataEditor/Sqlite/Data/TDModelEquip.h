#ifndef _TD_MODEL_EQUIP_H_
#define _TD_MODEL_EQUIP_H_

#include "SQLiteData.h"

class TDModelEquip : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* AttackAnimation;
	static const char* SkillAnimation;
	static const char* RunAnimation;
	static const char* WalkAnimation;
	static const char* DieAnimation;
	static const char* HitAnimation;
	static const char* StandAnimation;
	static const char* DigAnimation;
	static const char* SearchAnimation;
	static const char* JumpAttackAnimation;
	static const char* RushAttackAnimation;
public:
	int mID;
	string mName;
	ushort mAttackAnimation;
	ushort mSkillAnimation;
	ushort mRunAnimation;
	ushort mWalkAnimation;
	ushort mDieAnimation;
	ushort mHitAnimation;
	ushort mStandAnimation;
	ushort mDigAnimation;
	ushort mSearchAnimation;
	ushort mJumpAttackAnimation;
	ushort mRushAttackAnimation;
public:
	TDModelEquip()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mAttackAnimation, AttackAnimation);
		registeParam(mSkillAnimation, SkillAnimation);
		registeParam(mRunAnimation, RunAnimation);
		registeParam(mWalkAnimation, WalkAnimation);
		registeParam(mDieAnimation, DieAnimation);
		registeParam(mHitAnimation, HitAnimation);
		registeParam(mStandAnimation, StandAnimation);
		registeParam(mDigAnimation, DigAnimation);
		registeParam(mSearchAnimation, SearchAnimation);
		registeParam(mJumpAttackAnimation, JumpAttackAnimation);
		registeParam(mRushAttackAnimation, RushAttackAnimation);
	}
};

#endif