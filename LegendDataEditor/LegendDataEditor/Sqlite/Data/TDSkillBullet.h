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
	string mDescription;
	ushort mBulletEffect;
	byte mBulletEffectPoint;
	ushort mAdditiveEffect;
	ushort mExplosionEffect;
	ushort mHitEffect;
	myVector<float> mAdditiveProperty;
	ushort mAttack;
	float mAttackPercent;
	float mBulletSpeed;
	Vector2UShort mBulletSize;
	Vector2UShort mBulletCenter;
	bool mRotateBullet;
	ushort mBulletSound;
	float mExistTime;
	float mDestroyTime;
	myVector<ushort> mHitTargetBuff;
	myVector<ushort> mHitSelfBuff;
	myVector<ushort> mWillHitTargetBuff;
	byte mSingleTarget;
	byte mTrackTarget;
	byte mFaceDirection;
	byte mTargetPointType;
	byte mHitPoint;
	byte mBulletPath;
	bool mFaceForward;
	bool mSceneContinuously;
public:
	TDSkillBullet()
	{
		registeParam(mID, ID);
		registeParam(mDescription, Description);
		registeParam(mBulletEffect, BulletEffect);
		registeParam(mBulletEffectPoint, BulletEffectPoint);
		registeParam(mAdditiveEffect, AdditiveEffect);
		registeParam(mExplosionEffect, ExplosionEffect);
		registeParam(mHitEffect, HitEffect);
		registeParam(mAdditiveProperty, AdditiveProperty);
		registeParam(mAttack, Attack);
		registeParam(mAttackPercent, AttackPercent);
		registeParam(mBulletSpeed, BulletSpeed);
		registeParam(mBulletSize, BulletSize);
		registeParam(mBulletCenter, BulletCenter);
		registeParam(mRotateBullet, RotateBullet);
		registeParam(mBulletSound, BulletSound);
		registeParam(mExistTime, ExistTime);
		registeParam(mDestroyTime, DestroyTime);
		registeParam(mHitTargetBuff, HitTargetBuff);
		registeParam(mHitSelfBuff, HitSelfBuff);
		registeParam(mWillHitTargetBuff, WillHitTargetBuff);
		registeParam(mSingleTarget, SingleTarget);
		registeParam(mTrackTarget, TrackTarget);
		registeParam(mFaceDirection, FaceDirection);
		registeParam(mTargetPointType, TargetPointType);
		registeParam(mHitPoint, HitPoint);
		registeParam(mBulletPath, BulletPath);
		registeParam(mFaceForward, FaceForward);
		registeParam(mSceneContinuously, SceneContinuously);
	}
};

#endif