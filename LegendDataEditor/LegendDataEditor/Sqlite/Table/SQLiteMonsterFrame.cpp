#include "SQLiteMonsterFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

void SQLiteMonsterFrame::query(int monsterID, txVector<MonsterFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, monsterID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteMonsterFrame::query(int monsterID, int direction, txVector<MonsterFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, monsterID, " and ");
	appendConditionInt(conditionString, COL_DIRECTION, direction, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteMonsterFrame::query(int monsterID, int direction, const std::string& action, txVector<MonsterFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, monsterID, " and ");
	appendConditionInt(conditionString, COL_DIRECTION, direction, " and ");
	appendConditionString(conditionString, COL_ACTION, action, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteMonsterFrame::updateData(const MonsterFrameData& data)
{
	std::string updateString;
	appendUpdateInt(updateString, COL_FRAME_COUNT, data.mFrameCount);
	appendUpdateFloatArray(updateString, COL_FRAME_COUNT, data.mPosX);
	appendUpdateFloatArray(updateString, COL_FRAME_COUNT, data.mPosY, true);
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, data.mID, " and ");
	appendConditionInt(conditionString, COL_DIRECTION, data.mDirection, " and ");
	appendConditionString(conditionString, COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}

bool SQLiteMonsterFrame::insert(const MonsterFrameData& data)
{
	std::string valueString;
	appendValueInt(valueString, data.mID);
	appendValueInt(valueString, data.mDirection);
	appendValueString(valueString, data.mAction);
	appendValueInt(valueString, data.mFrameCount);
	appendValueFloatArray(valueString, data.mPosX);
	appendValueFloatArray(valueString, data.mPosY);
	appendValueString(valueString, data.mPrefab, true);
	return doInsert(valueString);
}

void SQLiteMonsterFrame::parseReader(SQLiteDataReader* reader, txVector<MonsterFrameData>& dataList)
{
	while (reader->read())
	{
		MonsterFrameData data;
		data.mID = reader->getInt(getCol(COL_ID));
		data.mDirection = reader->getInt(getCol(COL_DIRECTION));
		data.mAction = reader->getString(getCol(COL_ACTION));
		data.mFrameCount = reader->getInt(getCol(COL_FRAME_COUNT));
		StringUtility::stringToFloatArray(reader->getString(getCol(COL_POSX)), data.mPosX);
		StringUtility::stringToFloatArray(reader->getString(getCol(COL_POSY)), data.mPosY);
		data.mPrefab = reader->getString(getCol(COL_PREFAB));
		dataList.push_back(data);
	}
	mSQLite->releaseReader(reader);
}