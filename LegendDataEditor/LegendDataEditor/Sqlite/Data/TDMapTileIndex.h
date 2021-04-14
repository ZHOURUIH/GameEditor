#ifndef _TD_MAP_TILE_INDEX_H_
#define _TD_MAP_TILE_INDEX_H_

#include "SQLiteData.h"

class TDMapTileIndex : public SQLiteData
{
public:
	static const char* ID;
	static const char* FileIndex;
	static const char* SetIndex;
	static const char* TextureCount;
public:
	int mID;
	short mFileIndex;
	byte mSetIndex;
	ushort mTextureCount;
public:
	TDMapTileIndex()
	{
		registeParam(mID, ID);
		registeParam(mFileIndex, FileIndex);
		registeParam(mSetIndex, SetIndex);
		registeParam(mTextureCount, TextureCount);
	}
};

#endif