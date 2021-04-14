#ifndef _TD_MAP_EFFECT_H_
#define _TD_MAP_EFFECT_H_

#include "SQLiteData.h"

class TDMapEffect : public SQLiteData
{
public:
	static const char* ID;
	static const char* MapFileID;
	static const char* TilePosition;
	static const char* EffectFileIndex;
	static const char* EffectIndex;
	static const char* PositionOffset;
	static const char* Speed;
	static const char* BlendType;
public:
	int mID;
	ushort mMapFileID;
	Vector2Int mTilePosition;
	byte mEffectFileIndex;
	int mEffectIndex;
	Vector2Int mPositionOffset;
	float mSpeed;
	byte mBlendType;
public:
	TDMapEffect()
	{
		registeParam(mID, ID);
		registeParam(mMapFileID, MapFileID);
		registeParam(mTilePosition, TilePosition);
		registeParam(mEffectFileIndex, EffectFileIndex);
		registeParam(mEffectIndex, EffectIndex);
		registeParam(mPositionOffset, PositionOffset);
		registeParam(mSpeed, Speed);
		registeParam(mBlendType, BlendType);
	}
};

#endif