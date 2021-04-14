#ifndef _TD_MISSION_H_
#define _TD_MISSION_H_

#include "SQLiteData.h"

class TDMission : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Type;
	static const char* Level;
	static const char* PreMission;
	static const char* AcceptCondition;
	static const char* AcceptType;
	static const char* MainText;
	static const char* FinishText;
	static const char* SurpriseFinishText;
	static const char* FinishCondition;
	static const char* SaveFinishCondition;
	static const char* SurpriseCondition;
	static const char* SaveSurpriseCondition;
	static const char* RemoveMissionObject;
	static const char* Reward;
	static const char* SurpriseReward;
	static const char* NPC;
	static const char* Talk;
	static const char* FinishTalk;
public:
	int mID;
	byte mType;
	ushort mLevel;
	ushort mPreMission;
	myVector<ushort> mAcceptCondition;
	byte mAcceptType;
	myVector<ushort> mFinishCondition;
	myVector<byte> mSaveFinishCondition;
	myVector<ushort> mSurpriseCondition;
	myVector<byte> mSaveSurpriseCondition;
	bool mRemoveMissionObject;
	ushort mReward;
	ushort mSurpriseReward;
	ushort mNPC;
	myVector<ushort> mTalk;
	myVector<ushort> mFinishTalk;
public:
	TDMission()
	{
		registeParam(mID, ID);
		registeParamEmpty(Name);
		registeParam(mType, Type);
		registeParam(mLevel, Level);
		registeParam(mPreMission, PreMission);
		registeParam(mAcceptCondition, AcceptCondition);
		registeParam(mAcceptType, AcceptType);
		registeParamEmpty(MainText);
		registeParamEmpty(FinishText);
		registeParamEmpty(SurpriseFinishText);
		registeParam(mFinishCondition, FinishCondition);
		registeParam(mSaveFinishCondition, SaveFinishCondition);
		registeParam(mSurpriseCondition, SurpriseCondition);
		registeParam(mSaveSurpriseCondition, SaveSurpriseCondition);
		registeParam(mRemoveMissionObject, RemoveMissionObject);
		registeParam(mReward, Reward);
		registeParam(mSurpriseReward, SurpriseReward);
		registeParam(mNPC, NPC);
		registeParam(mTalk, Talk);
		registeParam(mFinishTalk, FinishTalk);
	}
};

#endif