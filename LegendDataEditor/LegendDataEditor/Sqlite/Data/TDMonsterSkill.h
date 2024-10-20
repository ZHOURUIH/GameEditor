﻿#ifndef _TD_MONSTER_SKILL_H_
#define _TD_MONSTER_SKILL_H_

#include "SQLiteData.h"

class TDMonsterSkill : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Description;
	static const char* ActiveAbility;
	static const char* Animation;
	static const char* FireEffect;
	static const char* AdditiveEffect;
	static const char* AdditiveEffectTime;
	static const char* FireSound;
	static const char* FaceTarget;
	static const char* IsDamage;
	static const char* EffectiveCamp;
	static const char* Range;
	static const char* FireTime;
	static const char* Bullet;
	static const char* CD;
	static const char* DamageType;
public:
	int mID;
	string mName;
	string mDescription;
	bool mActiveAbility;
	ushort mAnimation;
	ushort mFireEffect;
	ushort mAdditiveEffect;
	float mAdditiveEffectTime;
	ushort mFireSound;
	bool mFaceTarget;
	bool mIsDamage;
	byte mEffectiveCamp;
	float mRange;
	myVector<byte> mFireTime;
	myVector<ushort> mBullet;
	float mCD;
	byte mDamageType;
public:
	TDMonsterSkill()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mDescription, Description);
		registeParam(mActiveAbility, ActiveAbility);
		registeParam(mAnimation, Animation);
		registeParam(mFireEffect, FireEffect);
		registeParam(mAdditiveEffect, AdditiveEffect);
		registeParam(mAdditiveEffectTime, AdditiveEffectTime);
		registeParam(mFireSound, FireSound);
		registeParam(mFaceTarget, FaceTarget);
		registeParam(mIsDamage, IsDamage);
		registeParam(mEffectiveCamp, EffectiveCamp);
		registeParam(mRange, Range);
		registeParam(mFireTime, FireTime);
		registeParam(mBullet, Bullet);
		registeParam(mCD, CD);
		registeParam(mDamageType, DamageType);
	}
};

#endif