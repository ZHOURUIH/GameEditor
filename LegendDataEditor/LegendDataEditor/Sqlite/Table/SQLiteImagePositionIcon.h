#ifndef _SQLITE_IMAGE_POSITION_ICON_H_
#define _SQLITE_IMAGE_POSITION_ICON_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class ImagePositionIconData : public SQLiteTableData
{
public:
	static string ID;
	static string ATLAS;
	static string IMAGE;
	static string POS_X;
	static string POS_Y;
public:
	int mID;
	string mAtlas;
	string mImage;
	int mPosX;
	int mPosY;
public:
	ImagePositionIconData()
	{
		REGISTE_PARAM(mID, ID);
		REGISTE_PARAM(mAtlas, ATLAS);
		REGISTE_PARAM(mImage, IMAGE);
		REGISTE_PARAM(mPosX, POS_X);
		REGISTE_PARAM(mPosY, POS_Y);
	}
};

class SQLiteImagePositionIcon : public SQLiteTable
{
public:
	SQLiteImagePositionIcon(SQLite* sqlite)
		:SQLiteTable("ImagePositionIcon", sqlite){}
};

#endif