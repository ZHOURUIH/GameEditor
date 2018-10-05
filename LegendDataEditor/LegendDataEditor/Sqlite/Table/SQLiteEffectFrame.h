#ifndef _SQLITE_EFFECT_FRAME
#define _SQLITE_EFFECT_FRAME

#include "SQLiteTable.h"

class EffectFrameData
{
public:
	int mID;
	std::string mLabel;
	int mDirection;
	std::string mAction;
	int mFrameCount;
	txVector<int> mPosX;
	txVector<int> mPosY;
};

class SQLiteEffectFrame : SQLiteTable
{
	std::string COL_ID = "ID";
	std::string COL_LABEL = "Label";
	std::string COL_DIRECTION = "Direction";
	std::string COL_ACTION = "Action";
	std::string COL_FRAME_COUNT = "FrameCount";
	std::string COL_POSX = "PosX";
	std::string COL_POSY = "PosY";
	
public:
	SQLiteEffectFrame(SQLite* sqlite)
		:SQLiteTable("EffectFrame", sqlite)
	{
		registeColumn(COL_ID);
		registeColumn(COL_LABEL);
		registeColumn(COL_DIRECTION);
		registeColumn(COL_ACTION);
		registeColumn(COL_FRAME_COUNT);
		registeColumn(COL_POSX);
		registeColumn(COL_POSY);
	}
	void query(int ID, txVector<EffectFrameData>& dataList);
	void query(int ID, int direction, txVector<EffectFrameData>& dataList);
	void query(int ID, int direction, const std::string& action, txVector<EffectFrameData>& dataList);
	bool updateData(const EffectFrameData& data);
	bool insert(const EffectFrameData& data);
	bool insertOrUpdate(const EffectFrameData& data);
protected:
	void parseReader(SQLiteDataReader* reader, txVector<EffectFrameData>& dataList);
};

#endif