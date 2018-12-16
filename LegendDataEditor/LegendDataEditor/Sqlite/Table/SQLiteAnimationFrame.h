#ifndef _SQLITE_ANIMATION_FRAME_H_
#define _SQLITE_ANIMATION_FRAME_H_

#include "SQLiteTable.h"

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
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mAtlas = reader->getString(table->getCol(COL_ATLAS));
		mAnimation = reader->getString(table->getCol(COL_ANIMATION));
		mFrameCount = reader->getInt(table->getCol(COL_FRAME_COUNT));
		StringUtility::stringToIntArray(reader->getString(table->getCol(COL_POSX)), mPosX);
		StringUtility::stringToIntArray(reader->getString(table->getCol(COL_POSY)), mPosY);
	}
	virtual void insert(string& valueString) const
	{
		StringUtility::appendValueString(valueString, mAtlas);
		StringUtility::appendValueString(valueString, mAnimation);
		StringUtility::appendValueInt(valueString, mFrameCount);
		StringUtility::appendValueIntArray(valueString, mPosX);
		StringUtility::appendValueIntArray(valueString, mPosY);
	}
	virtual void update(string& updateString) const
	{
		StringUtility::appendUpdateString(updateString, COL_ATLAS, mAtlas);
		StringUtility::appendUpdateString(updateString, COL_ANIMATION, mAnimation);
		StringUtility::appendUpdateInt(updateString, COL_FRAME_COUNT, mFrameCount);
		StringUtility::appendUpdateIntArray(updateString, COL_POSX, mPosX);
		StringUtility::appendUpdateIntArray(updateString, COL_POSY, mPosY);
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_ATLAS);
		table->registeColumn(COL_ANIMATION);
		table->registeColumn(COL_FRAME_COUNT);
		table->registeColumn(COL_POSX);
		table->registeColumn(COL_POSY);
	}
};

class SQLiteAnimationFrame : SQLiteTable
{
public:
	SQLiteAnimationFrame(SQLite* sqlite)
		:SQLiteTable("AnimationFrame", sqlite)
	{
		AnimationFrameData::registeColumn(this);
	}
	void query(const string& atlas, const string& animation, txVector<AnimationFrameData>& dataList);
	bool updateData(const AnimationFrameData& data);
	bool insert(const AnimationFrameData& data);
};

#endif