#ifndef _TD_OBJECT_TILE_IMAGE_H_
#define _TD_OBJECT_TILE_IMAGE_H_

#include "SQLiteData.h"

class TDObjectTileImage : public SQLiteData
{
public:
	static const char* ID;
	static const char* FileIndex;
	static const char* ImageSetIndex;
	static const char* FileName;
	static const char* ImageSizeX;
	static const char* ImageSizeY;
public:
	int mID;
	int mFileIndex;
	int mImageSetIndex;
	int mFileName;
	int mImageSizeX;
	int mImageSizeY;
public:
	TDObjectTileImage()
	{
		registeParam(mID, ID);
		registeParam(mFileIndex, FileIndex);
		registeParam(mImageSetIndex, ImageSetIndex);
		registeParam(mFileName, FileName);
		registeParam(mImageSizeX, ImageSizeX);
		registeParam(mImageSizeY, ImageSizeY);
	}
};

#endif