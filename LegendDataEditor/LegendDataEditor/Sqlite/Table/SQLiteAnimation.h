#ifndef _SQLITE_ANIMATION_H_
#define _SQLITE_ANIMATION_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class AnimationData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_DESCRIPTION;
	static string COL_ATLAS;
	static string COL_ANIMATION;
	static string COL_FRAME_COUNT;
	static string COL_LOOP;
	static string COL_ANIMATION_SPEED;
	static string COL_DIRCTION_COUNT;
	static string COL_ANIMATION_POSITION;
public:
	int mID;
	string mDescription;
	string mAtlas;
	string mAnimation;
	int mFrameCount;
	int mLoop;
	float mAnimationSpeed;
	int mDirectionCount;
	txVector<int> mAnimationPosition;
public:
	AnimationData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mDescription, COL_DESCRIPTION);
		REGISTE_PARAM(mAtlas, COL_ATLAS);
		REGISTE_PARAM(mAnimation, COL_ANIMATION);
		REGISTE_PARAM(mFrameCount, COL_FRAME_COUNT);
		REGISTE_PARAM(mLoop, COL_LOOP);
		REGISTE_PARAM(mAnimationSpeed, COL_ANIMATION_SPEED);
		REGISTE_PARAM(mDirectionCount, COL_DIRCTION_COUNT);
		REGISTE_PARAM(mAnimationPosition, COL_ANIMATION_POSITION);
	}
};

class SQLiteAnimation : public SQLiteTable
{
public:
	SQLiteAnimation(SQLite* sqlite)
		:SQLiteTable("Animation", sqlite){}
	bool updateData(const AnimationData& data);
};

#endif