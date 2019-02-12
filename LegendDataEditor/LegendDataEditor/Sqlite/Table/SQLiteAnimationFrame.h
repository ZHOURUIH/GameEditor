#ifndef _SQLITE_ANIMATION_FRAME_H_
#define _SQLITE_ANIMATION_FRAME_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class AnimationFrameData : public SQLiteTableData
{
public:
	static string COL_ATLAS;
	static string COL_ANIMATION;
	static string COL_FRAME_COUNT;
	static string COL_POSX;
	static string COL_POSY;
public:
	string mAtlas;
	string mAnimation;
	int mFrameCount;
	txVector<int> mPosX;
	txVector<int> mPosY;
public:
	AnimationFrameData()
	{
		REGISTE_PARAM(mAtlas, COL_ATLAS);
		REGISTE_PARAM(mAnimation, COL_ANIMATION);
		REGISTE_PARAM(mFrameCount, COL_FRAME_COUNT);
		REGISTE_PARAM(mPosX, COL_POSX);
		REGISTE_PARAM(mPosY, COL_POSY);
	}
};

class SQLiteAnimationFrame : SQLiteTable
{
public:
	SQLiteAnimationFrame(SQLite* sqlite)
		:SQLiteTable("AnimationFrame", sqlite)
	{
		;
	}
	void query(const string& atlas, const string& animation, txVector<AnimationFrameData>& dataList);
	bool updateData(const AnimationFrameData& data);
	bool insert(const AnimationFrameData& data);
};

#endif