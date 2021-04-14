#ifndef _TD_ITEM_EQUIP_H_
#define _TD_ITEM_EQUIP_H_

#include "SQLiteData.h"

class TDItemEquip : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* DisplayFormat;
	static const char* EquipType;
	static const char* Job;
	static const char* Sex;
	static const char* AniCount;
	static const char* DuraMax;
	static const char* MinPhysicsDefence;
	static const char* MaxPhysicsDefence;
	static const char* MinMagicDefence;
	static const char* MaxMagicDefence;
	static const char* MinPhysicsAttack;
	static const char* MaxPhysicsAttack;
	static const char* MinMagicAttack;
	static const char* MaxMagicAttack;
	static const char* MinSorceryAttack;
	static const char* MaxSorceryAttack;
	static const char* MaxHP;
	static const char* MaxMP;
	static const char* AttackSpeed;
	static const char* MoveSpeed;
	static const char* Lucky;
	static const char* HitProbability;
	static const char* EvadeProbability;
	static const char* CriticalProbability;
	static const char* CriticalDamageRate;
	static const char* HPRecover;
	static const char* MPRecover;
	static const char* ExtraBuff;
	static const char* Wing;
	static const char* WingBind;
	static const char* Model;
	static const char* WeaponEffect;
	static const char* PhysicsDecrease;
	static const char* MagicDecrease;
	static const char* SorceryDecrease;
	static const char* DigMineSpeed;
	static const char* DisplayIllustrated;
	static const char* SkillAttackIncrease;
public:
	int mID;
	string mName;
	string mDisplayFormat;
	byte mEquipType;
	byte mJob;
	byte mSex;
	byte mAniCount;
	ushort mDuraMax;
	ushort mMinPhysicsDefence;
	ushort mMaxPhysicsDefence;
	ushort mMinMagicDefence;
	ushort mMaxMagicDefence;
	ushort mMinPhysicsAttack;
	ushort mMaxPhysicsAttack;
	ushort mMinMagicAttack;
	ushort mMaxMagicAttack;
	ushort mMinSorceryAttack;
	ushort mMaxSorceryAttack;
	ushort mMaxHP;
	ushort mMaxMP;
	float mAttackSpeed;
	float mMoveSpeed;
	char mLucky;
	float mHitProbability;
	float mEvadeProbability;
	float mCriticalProbability;
	float mCriticalDamageRate;
	ushort mHPRecover;
	ushort mMPRecover;
	myVector<ushort> mExtraBuff;
	ushort mWing;
	bool mWingBind;
	ushort mModel;
	ushort mWeaponEffect;
	float mPhysicsDecrease;
	float mMagicDecrease;
	float mSorceryDecrease;
	float mDigMineSpeed;
	bool mDisplayIllustrated;
	float mSkillAttackIncrease;
public:
	TDItemEquip()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mDisplayFormat, DisplayFormat);
		registeParam(mEquipType, EquipType);
		registeParam(mJob, Job);
		registeParam(mSex, Sex);
		registeParam(mAniCount, AniCount);
		registeParam(mDuraMax, DuraMax);
		registeParam(mMinPhysicsDefence, MinPhysicsDefence);
		registeParam(mMaxPhysicsDefence, MaxPhysicsDefence);
		registeParam(mMinMagicDefence, MinMagicDefence);
		registeParam(mMaxMagicDefence, MaxMagicDefence);
		registeParam(mMinPhysicsAttack, MinPhysicsAttack);
		registeParam(mMaxPhysicsAttack, MaxPhysicsAttack);
		registeParam(mMinMagicAttack, MinMagicAttack);
		registeParam(mMaxMagicAttack, MaxMagicAttack);
		registeParam(mMinSorceryAttack, MinSorceryAttack);
		registeParam(mMaxSorceryAttack, MaxSorceryAttack);
		registeParam(mMaxHP, MaxHP);
		registeParam(mMaxMP, MaxMP);
		registeParam(mAttackSpeed, AttackSpeed);
		registeParam(mMoveSpeed, MoveSpeed);
		registeParam(mLucky, Lucky);
		registeParam(mHitProbability, HitProbability);
		registeParam(mEvadeProbability, EvadeProbability);
		registeParam(mCriticalProbability, CriticalProbability);
		registeParam(mCriticalDamageRate, CriticalDamageRate);
		registeParam(mHPRecover, HPRecover);
		registeParam(mMPRecover, MPRecover);
		registeParam(mExtraBuff, ExtraBuff);
		registeParam(mWing, Wing);
		registeParam(mWingBind, WingBind);
		registeParam(mModel, Model);
		registeParam(mWeaponEffect, WeaponEffect);
		registeParam(mPhysicsDecrease, PhysicsDecrease);
		registeParam(mMagicDecrease, MagicDecrease);
		registeParam(mSorceryDecrease, SorceryDecrease);
		registeParam(mDigMineSpeed, DigMineSpeed);
		registeParam(mDisplayIllustrated, DisplayIllustrated);
		registeParam(mSkillAttackIncrease, SkillAttackIncrease);
	}
};

#endif