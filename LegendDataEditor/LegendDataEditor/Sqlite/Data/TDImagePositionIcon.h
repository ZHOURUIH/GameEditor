#ifndef _TD_IMAGE_POSITION_ICON_H_
#define _TD_IMAGE_POSITION_ICON_H_

#include "SQLiteData.h"

class TDImagePositionIcon : public SQLiteData
{
public:
	static const char* ID;
	static const char* Atlas;
	static const char* Image;
	static const char* PosX;
	static const char* PosY;
public:
	int mID;
	string mAtlas;
	string mImage;
	short mPosX;
	short mPosY;
public:
	TDImagePositionIcon()
	{
		registeParam(mID, ID);
		registeParam(mAtlas, Atlas);
		registeParam(mImage, Image);
		registeParam(mPosX, PosX);
		registeParam(mPosY, PosY);
	}
};

#endif