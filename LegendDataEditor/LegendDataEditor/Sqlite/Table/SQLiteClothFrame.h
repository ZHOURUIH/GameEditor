#ifndef _SQLITE_CLOTH_FRAME_H_
#define _SQLITE_CLOTH_FRAME_H_

#include "SQLiteTable.h"

class ClothFrameData
{
public:
	int mID;
	int mDirection;
	std::string mAction;
	int mFrameCount;
	txVector<float> mPosX;
	txVector<float> mPosY;
};

class SQLiteClothFrame : public SQLiteTable
{
	std::string COL_ID = "ClothID";
	std::string COL_DIRECTION = "Direction";
	std::string COL_ACTION = "Action";
	std::string COL_FRAME_COUNT = "FrameCount";
	std::string COL_POSX = "PosX";
	std::string COL_POSY = "PosY";
public:
	SQLiteClothFrame(SQLite* sqlite)
		:SQLiteTable("ClothFrame", sqlite)
	{
		registeColumn(COL_ID);
		registeColumn(COL_DIRECTION);
		registeColumn(COL_ACTION);
		registeColumn(COL_FRAME_COUNT);
		registeColumn(COL_POSX);
		registeColumn(COL_POSY);
	}
	void query(int clothID, txVector<ClothFrameData>& dataList);
	void query(int clothID, int direction, txVector<ClothFrameData>& dataList);
	void query(int clothID, int direction, const std::string& action, txVector<ClothFrameData>& dataList);
	bool updateData(const ClothFrameData& data);
	bool insert(const ClothFrameData& data);
	bool insertOrUpdate(const ClothFrameData& data);
	//------------------------------------------------------------------------------------------------------------------------
protected:
	void parseReader(SQLiteDataReader* reader, txVector<ClothFrameData>& dataList);
};

#endif