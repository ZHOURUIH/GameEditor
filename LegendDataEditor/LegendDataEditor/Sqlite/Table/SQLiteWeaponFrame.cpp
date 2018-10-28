#include "SQLiteWeaponFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

std::string WeaponFrameData::COL_ID = "WeaponID";
std::string WeaponFrameData::COL_LABEL = "Label";
std::string WeaponFrameData::COL_DIRECTION = "Direction";
std::string WeaponFrameData::COL_ACTION = "Action";
std::string WeaponFrameData::COL_FRAME_COUNT = "FrameCount";
std::string WeaponFrameData::COL_POSX = "PosX";
std::string WeaponFrameData::COL_POSY = "PosY";

void SQLiteWeaponFrame::query(int weaponID, txVector<WeaponFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, WeaponFrameData::COL_ID, weaponID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteWeaponFrame::query(int weaponID, int direction, txVector<WeaponFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, WeaponFrameData::COL_ID, weaponID, " and ");
	appendConditionInt(conditionString, WeaponFrameData::COL_DIRECTION, direction, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteWeaponFrame::query(int weaponID, int direction, const std::string& action, txVector<WeaponFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, WeaponFrameData::COL_ID, weaponID, " and ");
	appendConditionInt(conditionString, WeaponFrameData::COL_DIRECTION, direction, " and ");
	appendConditionString(conditionString, WeaponFrameData::COL_ACTION, action, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteWeaponFrame::updateData(const WeaponFrameData& data)
{
	std::string updateString;
	appendUpdateInt(updateString, WeaponFrameData::COL_FRAME_COUNT, data.mFrameCount);
	appendUpdateIntArray(updateString, WeaponFrameData::COL_POSX, data.mPosX);
	appendUpdateIntArray(updateString, WeaponFrameData::COL_POSY, data.mPosY, true);
	std::string conditionString;
	appendConditionInt(conditionString, WeaponFrameData::COL_ID, data.mID, " and ");
	appendConditionInt(conditionString, WeaponFrameData::COL_DIRECTION, data.mDirection, " and ");
	appendConditionString(conditionString, WeaponFrameData::COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}

bool SQLiteWeaponFrame::insert(const WeaponFrameData& data)
{
	std::string valueString;
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mDirection);
	appendValueString(valueString, data.mAction);
	appendValueInt(valueString, data.mFrameCount);
	appendValueIntArray(valueString, data.mPosX);
	appendValueIntArray(valueString, data.mPosY, true);
	return doInsert(valueString);
}