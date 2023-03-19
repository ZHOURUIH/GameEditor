#ifndef _TD_TILE_IMAGE_MID_H_
#define _TD_TILE_IMAGE_MID_H_

#include "SQLiteData.h"

class TDTileImageMid : public SQLiteData
{
public:
	static const char* ID;
	static const char* AtlasName;
	static const char* ImageName;
	static const char* MapFileID;
public:
	int mID;
	string mAtlasName;
	int mImageName;
	int mMapFileID;
public:
	TDTileImageMid()
	{
		registeParam(mID, ID);
		registeParam(mAtlasName, AtlasName);
		registeParam(mImageName, ImageName);
		registeParam(mMapFileID, MapFileID);
	}
};

#endif