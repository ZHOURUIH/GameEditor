#ifndef _TD_TILE_IMAGE_OBJECT_H_
#define _TD_TILE_IMAGE_OBJECT_H_

#include "SQLiteData.h"

class TDTileImageObject : public SQLiteData
{
public:
	static const char* ID;
	static const char* FileIndex;
	static const char* AtlasName;
	static const char* ImageName;
	static const char* ImageSizeX;
	static const char* ImageSizeY;
public:
	int mID;
	short mFileIndex;
	string mAtlasName;
	int mImageName;
	short mImageSizeX;
	short mImageSizeY;
public:
	TDTileImageObject()
	{
		registeParam(mID, ID);
		registeParam(mFileIndex, FileIndex);
		registeParam(mAtlasName, AtlasName);
		registeParam(mImageName, ImageName);
		registeParam(mImageSizeX, ImageSizeX);
		registeParam(mImageSizeY, ImageSizeY);
	}
};

#endif