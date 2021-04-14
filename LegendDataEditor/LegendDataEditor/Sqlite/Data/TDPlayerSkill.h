#ifndef _TD_PLAYER_SKILL_H_
#define _TD_PLAYER_SKILL_H_

#include "SQLiteData.h"

class TDPlayerSkill : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Description;
	static const char* ActiveAbility;
	static const char* Icon;
	static const char* Animation;
	static const char* SkillDetail;
	static const char* MaxLevel;
	static const char* FaceTarget;
	static const char* IsDamage;
	static const char* EffectiveCamp;
	static const char* Job;
	static const char* CD;
	static const char* SearchTarget;
	static const char* DamageType;
	static const char* Priority;
public:
	int mID;
	bool mActiveAbility;
	myVector<ushort> mSkillDetail;
	byte mMaxLevel;
	bool mIsDamage;
	byte mEffectiveCamp;
	short mJob;
	float mCD;
	byte mSearchTarget;
	byte mDamageType;
	byte mPriority;
public:
	TDPlayerSkill()
	{
		registeParam(mID, ID);
		registeParamEmpty(Name);
		registeParamEmpty(Description);
		registeParam(mActiveAbility, ActiveAbility);
		registeParamEmpty(Icon);
		registeParamEmpty(Animation);
		registeParam(mSkillDetail, SkillDetail);
		registeParam(mMaxLevel, MaxLevel);
		registeParamEmpty(FaceTarget);
		registeParam(mIsDamage, IsDamage);
		registeParam(mEffectiveCamp, EffectiveCamp);
		registeParam(mJob, Job);
		registeParam(mCD, CD);
		registeParam(mSearchTarget, SearchTarget);
		registeParam(mDamageType, DamageType);
		registeParam(mPriority, Priority);
	}
};

#endif