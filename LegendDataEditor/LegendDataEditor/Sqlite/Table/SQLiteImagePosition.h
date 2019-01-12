#ifndef _SQLITE_IMAGE_POSITION_H_
#define _SQLITE_IMAGE_POSITION_H_

#include "SQLiteTable.h"

class ImagePositionData : public SQLiteTableData
{
public:
	static string COL_ATALS;
	static string COL_IMAGE;
	static string COL_POS_X;
	static string COL_POS_Y;
public:
	string mAtlas;	// 图集名
	string mImage;	// 图片名
	int mPosX;
	int mPosY;
public:
	ImagePositionData()
	{
		REGISTE_PARAM(mAtlas, COL_ATALS);
		REGISTE_PARAM(mImage, COL_IMAGE);
		REGISTE_PARAM(mPosX, COL_POS_X);
		REGISTE_PARAM(mPosY, COL_POS_Y);
	}
};

class SQLiteImagePosition : public SQLiteTable
{
public:
	SQLiteImagePosition(SQLite* sqlite)
		:SQLiteTable("ImagePosition", sqlite)
	{
		;
	}
	void query(const string& originAtlas, const string& image, ImagePositionData& data);
	bool insert(const ImagePositionData& data);
};

#endif