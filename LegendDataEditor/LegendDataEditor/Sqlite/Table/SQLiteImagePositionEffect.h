#ifndef _SQLITE_IMAGE_POSITION_EFFECT_H_
#define _SQLITE_IMAGE_POSITION_EFFECT_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class ImagePositionEffectData : public SQLiteTableData
{
public:
	static string ID;
	static string PREFAB;
	static string FRAME_COUNT;
	static string POS_X;
	static string POS_Y;
	static string SPEED;
	static string LOOP;
public:
	int mID;
	string mPrefab;
	int mFrameCount;
	txVector<int> mPosX;
	txVector<int> mPosY;
	float mSpeed;
	int mLoop;
public:
	ImagePositionEffectData()
	{
		REGISTE_PARAM(mID, ID);
		REGISTE_PARAM(mPrefab, PREFAB);
		REGISTE_PARAM(mFrameCount, FRAME_COUNT);
		REGISTE_PARAM(mPosX, POS_X);
		REGISTE_PARAM(mPosY, POS_Y);
		REGISTE_PARAM(mSpeed, SPEED);
		REGISTE_PARAM(mLoop, LOOP);
	}
};

class SQLiteImagePositionEffect : public SQLiteTable
{
public:
	SQLiteImagePositionEffect(SQLite* sqlite)
		:SQLiteTable("ImagePositionEffect", sqlite){}
	void query(const string& prefab, ImagePositionEffectData& data);
};

#endif