#ifndef _SQLITE_WEAPON_FRAME
#define _SQLITE_WEAPON_FRAME

#include "SQLiteTable.h"

class WeaponFrameData
{
public:
	int mID;
	int mDirection;
	std::string mAction;
	int mFrameCount;
	txVector<float> mPosX;
	txVector<float> mPosY;
};

class SQLiteWeaponFrame : SQLiteTable
{
	std::string COL_ID = "WeaponID";
	std::string COL_DIRECTION = "Direction";
	std::string COL_ACTION = "Action";
	std::string COL_FRAME_COUNT = "FrameCount";
	std::string COL_POSX = "PosX";
	std::string COL_POSY = "PosY";
	
public:
	SQLiteWeaponFrame(SQLite* sqlite)
		:SQLiteTable("WeaponFrame", sqlite)
	{
		registeColumn(COL_ID);
		registeColumn(COL_DIRECTION);
		registeColumn(COL_ACTION);
		registeColumn(COL_FRAME_COUNT);
		registeColumn(COL_POSX);
		registeColumn(COL_POSY);
	}
	void query(int weaponID, txVector<WeaponFrameData>& dataList);
	void query(int weaponID, int direction, txVector<WeaponFrameData>& dataList);
	void query(int weaponID, int direction, const std::string& action, txVector<WeaponFrameData>& dataList);
	bool updateData(const WeaponFrameData& data);
	bool insert(const WeaponFrameData& data);
protected:
	void parseReader(SQLiteDataReader* reader, txVector<WeaponFrameData>& dataList);
};

#endif