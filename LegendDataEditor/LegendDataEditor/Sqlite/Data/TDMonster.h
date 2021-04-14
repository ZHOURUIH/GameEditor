#ifndef _TD_MONSTER_H_
#define _TD_MONSTER_H_

#include "SQLiteData.h"

class TDMonster : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Description;
	static const char* TemplateID;
	static const char* StrengthType;
	static const char* BehaviourTree;
	static const char* Level;
	static const char* HP;
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
	static const char* HitProbability;
	static const char* EvadeProbability;
	static const char* TrackSpeedRatio;
	static const char* TrackWaitRatio;
	static const char* MoveSpeed;
	static const char* WalkStep;
	static const char* WalkWait;
	static const char* AttackWait;
	static const char* AttackSpeed;
	static const char* SkillCheckRange;
	static const char* ViewRange;
	static const char* MoveRange;
	static const char* Exp;
	static const char* Skill;
	static const char* SkillLevel;
	static const char* SearchTargetDistance;
	static const char* TelentBuff;
	static const char* TalkBubbleGroup;
	static const char* DefaultAction;
	static const char* ReviveCount;
	static const char* SearchTarget;
	static const char* Duty;
	static const char* HPRecover;
	static const char* DisplayIllustrated;
	static const char* AntiTemptation;
public:
	int mID;
	string mName;
	ushort mTemplateID;
	byte mStrengthType;
	byte mBehaviourTree;
	ushort mLevel;
	uint mHP;
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
	float mHitProbability;
	float mEvadeProbability;
	float mTrackSpeedRatio;
	float mTrackWaitRatio;
	ushort mMoveSpeed;
	ushort mWalkStep;
	float mWalkWait;
	float mAttackWait;
	float mAttackSpeed;
	ushort mSkillCheckRange;
	ushort mViewRange;
	ushort mMoveRange;
	uint mExp;
	myVector<ushort> mSkill;
	myVector<byte> mSkillLevel;
	ushort mSearchTargetDistance;
	myVector<ushort> mTelentBuff;
	myVector<ushort> mTalkBubbleGroup;
	byte mDefaultAction;
	byte mReviveCount;
	byte mSearchTarget;
	byte mDuty;
	ushort mHPRecover;
	bool mDisplayIllustrated;
	int mAntiTemptation;
public:
	TDMonster()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParamEmpty(Description);
		registeParam(mTemplateID, TemplateID);
		registeParam(mStrengthType, StrengthType);
		registeParam(mBehaviourTree, BehaviourTree);
		registeParam(mLevel, Level);
		registeParam(mHP, HP);
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
		registeParam(mHitProbability, HitProbability);
		registeParam(mEvadeProbability, EvadeProbability);
		registeParam(mTrackSpeedRatio, TrackSpeedRatio);
		registeParam(mTrackWaitRatio, TrackWaitRatio);
		registeParam(mMoveSpeed, MoveSpeed);
		registeParam(mWalkStep, WalkStep);
		registeParam(mWalkWait, WalkWait);
		registeParam(mAttackWait, AttackWait);
		registeParam(mAttackSpeed, AttackSpeed);
		registeParam(mSkillCheckRange, SkillCheckRange);
		registeParam(mViewRange, ViewRange);
		registeParam(mMoveRange, MoveRange);
		registeParam(mExp, Exp);
		registeParam(mSkill, Skill);
		registeParam(mSkillLevel, SkillLevel);
		registeParam(mSearchTargetDistance, SearchTargetDistance);
		registeParam(mTelentBuff, TelentBuff);
		registeParam(mTalkBubbleGroup, TalkBubbleGroup);
		registeParam(mDefaultAction, DefaultAction);
		registeParam(mReviveCount, ReviveCount);
		registeParam(mSearchTarget, SearchTarget);
		registeParam(mDuty, Duty);
		registeParam(mHPRecover, HPRecover);
		registeParam(mDisplayIllustrated, DisplayIllustrated);
		registeParam(mAntiTemptation, AntiTemptation);
	}
};

#endif