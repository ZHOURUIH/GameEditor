﻿#ifndef _TD_MAP_TILE_INDEX_H_
#define _TD_MAP_TILE_INDEX_H_

#include "SQLiteData.h"

class TDMapTileIndex : public SQLiteData
{
public:
	static const char* ID;
	static const char* MapFileName;
	static const char* AtlasName;
	static const char* TextureCount;
public:
	int mID;
	string mMapFileName;
	string mAtlasName;
	ushort mTextureCount;
public:
	TDMapTileIndex()
	{
		registeParam(mID, ID);
		registeParam(mMapFileName, MapFileName);
		registeParam(mAtlasName, AtlasName);
		registeParam(mTextureCount, TextureCount);
	}
};

#endif