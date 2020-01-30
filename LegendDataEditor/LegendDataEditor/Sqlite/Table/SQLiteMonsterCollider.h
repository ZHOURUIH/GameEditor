#ifndef _SQLITE_MONSTER_COLLIDER_H_
#define _SQLITE_MONSTER_COLLIDER_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class MonsterColliderData : public SQLiteTableData
{
public:
	static string COL_ATLAS;
	static string COL_ANIMATION;
	static string COL_FRAME_COUNT;
	static string COL_CENTER_X;
	static string COL_CENTER_Y;
	static string COL_WIDTH;
	static string COL_HEIGHT;
public:
	string mAtlas;
	string mAnimation;
	int mFrameCount;
	txVector<int> mCenterX;
	txVector<int> mCenterY;
	txVector<int> mWidth;
	txVector<int> mHeight;
public:
	MonsterColliderData()
	{
		REGISTE_PARAM(mAtlas, COL_ATLAS);
		REGISTE_PARAM(mAnimation, COL_ANIMATION);
		REGISTE_PARAM(mFrameCount, COL_FRAME_COUNT);
		REGISTE_PARAM(mCenterX, COL_CENTER_X);
		REGISTE_PARAM(mCenterY, COL_CENTER_Y);
		REGISTE_PARAM(mWidth, COL_WIDTH);
		REGISTE_PARAM(mHeight, COL_HEIGHT);
	}
};

class SQLiteMonsterCollider : SQLiteTable
{
public:
	SQLiteMonsterCollider(SQLite* sqlite)
		:SQLiteTable("MonsterCollider", sqlite)
	{}
	void query(const string& atlas, const string& animation, txVector<MonsterColliderData*>& dataList);
	bool updateData(const MonsterColliderData& data);
	bool insert(const MonsterColliderData& data);
};

#endif