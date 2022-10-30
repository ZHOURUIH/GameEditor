#ifndef _TD_TILE_IMAGE_BIG_H_
#define _TD_TILE_IMAGE_BIG_H_

#include "SQLiteData.h"

class TDTileImageBig : public SQLiteData
{
public:
	static const char* ID;
	static const char* MapFileName;
	static const char* AtlasName;
	static const char* ImageName;
public:
	int mID;
	string mMapFileName;
	string mAtlasName;
	int mImageName;
public:
	TDTileImageBig()
	{
		registeParam(mID, ID);
		registeParam(mMapFileName, MapFileName);
		registeParam(mAtlasName, AtlasName);
		registeParam(mImageName, ImageName);
	}
};

#endif