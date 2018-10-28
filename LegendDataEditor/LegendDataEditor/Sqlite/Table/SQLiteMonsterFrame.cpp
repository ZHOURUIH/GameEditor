#include "SQLiteMonsterFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

std::string MonsterFrameData::COL_ID = "MonsterID";
std::string MonsterFrameData::COL_LABEL = "Label";
std::string MonsterFrameData::COL_DIRECTION = "Direction";
std::string MonsterFrameData::COL_ACTION = "Action";
std::string MonsterFrameData::COL_FRAME_COUNT = "FrameCount";
std::string MonsterFrameData::COL_POSX = "PosX";
std::string MonsterFrameData::COL_POSY = "PosY";

void SQLiteMonsterFrame::query(int monsterID, txVector<MonsterFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, MonsterFrameData::COL_ID, monsterID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteMonsterFrame::query(int monsterID, int direction, txVector<MonsterFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, MonsterFrameData::COL_ID, monsterID, " and ");
	appendConditionInt(conditionString, MonsterFrameData::COL_DIRECTION, direction, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteMonsterFrame::query(int monsterID, int direction, const std::string& action, txVector<MonsterFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, MonsterFrameData::COL_ID, monsterID, " and ");
	appendConditionInt(conditionString, MonsterFrameData::COL_DIRECTION, direction, " and ");
	appendConditionString(conditionString, MonsterFrameData::COL_ACTION, action, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteMonsterFrame::updateData(const MonsterFrameData& data)
{
	std::string updateString;
	appendUpdateInt(updateString, MonsterFrameData::COL_FRAME_COUNT, data.mFrameCount);
	appendUpdateIntArray(updateString, MonsterFrameData::COL_FRAME_COUNT, data.mPosX);
	appendUpdateIntArray(updateString, MonsterFrameData::COL_FRAME_COUNT, data.mPosY, true);
	std::string conditionString;
	appendConditionInt(conditionString, MonsterFrameData::COL_ID, data.mID, " and ");
	appendConditionInt(conditionString, MonsterFrameData::COL_DIRECTION, data.mDirection, " and ");
	appendConditionString(conditionString, MonsterFrameData::COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}

bool SQLiteMonsterFrame::insert(const MonsterFrameData& data)
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