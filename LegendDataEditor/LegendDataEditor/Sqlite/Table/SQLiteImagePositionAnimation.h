#ifndef _SQLITE_IMAGE_POSITION_ANIMATION_H_
#define _SQLITE_IMAGE_POSITION_ANIMATION_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class ImagePositionAnimationData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_ATLAS;
	static string COL_ANIMATION;
	static string COL_FRAME_COUNT;
	static string COL_POSX;
	static string COL_POSY;
public:
	int mID;
	string mAtlas;
	string mAnimation;
	int mFrameCount;
	txVector<int> mPosX;
	txVector<int> mPosY;
public:
	ImagePositionAnimationData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mAtlas, COL_ATLAS);
		REGISTE_PARAM(mAnimation, COL_ANIMATION);
		REGISTE_PARAM(mFrameCount, COL_FRAME_COUNT);
		REGISTE_PARAM(mPosX, COL_POSX);
		REGISTE_PARAM(mPosY, COL_POSY);
	}
};

class SQLiteImagePositionAnimation : public SQLiteTable
{
public:
	SQLiteImagePositionAnimation(SQLite* sqlite)
		:SQLiteTable("ImagePositionAnimation", sqlite){}
	void query(const string& atlas, const string& animation, ImagePositionAnimationData& data);
	bool updateData(const ImagePositionAnimationData& data);
};

#endif