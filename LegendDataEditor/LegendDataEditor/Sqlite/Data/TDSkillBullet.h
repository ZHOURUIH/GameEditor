#ifndef _TD_SKILL_BULLET_H_
#define _TD_SKILL_BULLET_H_

#include "SQLiteData.h"

class TDSkillBullet : public SQLiteData
{
public:
	static const char* ID;
	static const char* Description;
	static const char* BulletEffect;
	static const char* BulletEffectPoint;
	static const char* AdditiveEffect;
	static const char* ExplosionEffect;
	static const char* HitEffect;
	static const char* AdditiveProperty;
	static const char* Attack;
	static const char* AttackPercent;
	static const char* BulletSpeed;
	static const char* BulletSize;
	static const char* BulletCenter;
	static const char* RotateBullet;
	static const char* BulletSound;
	static const char* ExistTime;
	static const char* DestroyTime;
	static const char* HitTargetBuff;
	static const char* HitSelfBuff;
	static const char* WillHitTargetBuff;
	static const char* SingleTarget;
	static const char* TrackTarget;
	static const char* FaceDirection;
	static const char* TargetPointType;
	static const char* HitPoint;
	static const char* BulletPath;
	static const char* FaceForward;
	static const char* SceneContinuously;
public:
	int mID;
	myVector<float> mAdditiveProperty;
	ushort mAttack;
	float mAttackPercent;
	float mExistTime;
	float mDestroyTime;
	myVector<ushort> mHitTargetBuff;
	myVector<ushort> mHitSelfBuff;
	myVector<ushort> mWillHitTargetBuff;
	byte mSingleTarget;
	byte mTrackTarget;
	byte mTargetPointType;
public:
	TDSkillBullet()
	{
		registeParam(mID, ID);
		registeParamEmpty(Description);
		registeParamEmpty(BulletEffect);
		registeParamEmpty(BulletEffectPoint);
		registeParamEmpty(AdditiveEffect);
		registeParamEmpty(ExplosionEffect);
		registeParamEmpty(HitEffect);
		registeParam(mAdditiveProperty, AdditiveProperty);
		registeParam(mAttack, Attack);
		registeParam(mAttackPercent, AttackPercent);
		registeParamEmpty(BulletSpeed);
		registeParamEmpty(BulletSize);
		registeParamEmpty(BulletCenter);
		registeParamEmpty(RotateBullet);
		registeParamEmpty(BulletSound);
		registeParam(mExistTime, ExistTime);
		registeParam(mDestroyTime, DestroyTime);
		registeParam(mHitTargetBuff, HitTargetBuff);
		registeParam(mHitSelfBuff, HitSelfBuff);
		registeParam(mWillHitTargetBuff, WillHitTargetBuff);
		registeParam(mSingleTarget, SingleTarget);
		registeParam(mTrackTarget, TrackTarget);
		registeParamEmpty(FaceDirection);
		registeParam(mTargetPointType, TargetPointType);
		registeParamEmpty(HitPoint);
		registeParamEmpty(BulletPath);
		registeParamEmpty(FaceForward);
		registeParamEmpty(SceneContinuously);
	}
};

#endif