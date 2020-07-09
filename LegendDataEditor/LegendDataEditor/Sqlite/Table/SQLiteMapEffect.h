#ifndef _SQLITE_MAP_EFFECT_H_
#define _SQLITE_MAP_EFFECT_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class MapEffectData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_MAP_FILE_ID;
	static string COL_TILE_POSITION;
	static string COL_EFFECT_FILE_INDEX;
	static string COL_EFFECT_INDEX;
	static string COL_POSITION_OFFSET;
	static string COL_SPEED;
	static string COL_BLEND_TYPE;
public:
	int mID;
	int mMapFileID;
	Vector2i mTilePosition;
	int mEffectFileIndex;
	int mEffectIndex;
	Vector2i mPositionOffset;
	float mSpeed;
	int mBlendType;
public:
	MapEffectData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mMapFileID, COL_MAP_FILE_ID);
		REGISTE_PARAM(mTilePosition, COL_TILE_POSITION);
		REGISTE_PARAM(mEffectFileIndex, COL_EFFECT_FILE_INDEX);
		REGISTE_PARAM(mEffectIndex, COL_EFFECT_INDEX);
		REGISTE_PARAM(mPositionOffset, COL_POSITION_OFFSET);
		REGISTE_PARAM(mSpeed, COL_SPEED);
		REGISTE_PARAM(mBlendType, COL_BLEND_TYPE);
	}
};

class SQLiteMapEffect : public SQLiteTable
{
public:
	SQLiteMapEffect(SQLite* sqlite)
		:SQLiteTable("MapEffect", sqlite){}
	void query(int id, MapEffectData& data);
	bool insert(const MapEffectData& data);
	bool update(const MapEffectData& data);
};

#endif