#include "SQLiteClothFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

void SQLiteClothFrame::query(int clothID, txVector<ClothFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, clothID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteClothFrame::query(int clothID, int direction, txVector<ClothFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, clothID, " and ");
	appendConditionInt(conditionString, COL_DIRECTION, direction, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteClothFrame::query(int clothID, int direction, const std::string& action, txVector<ClothFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, clothID, " and ");
	appendConditionInt(conditionString, COL_DIRECTION, direction, " and ");
	appendConditionString(conditionString, COL_ACTION, action, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteClothFrame::updateData(const ClothFrameData& data)
{
	std::string updateString;
	appendUpdateInt(updateString, COL_FRAME_COUNT, data.mFrameCount);
	appendUpdateFloatArray(updateString, COL_POSX, data.mPosX);
	appendUpdateFloatArray(updateString, COL_POSY, data.mPosY, true);
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, data.mID, " and ");
	appendConditionInt(conditionString, COL_DIRECTION, data.mDirection, " and ");
	appendConditionString(conditionString, COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}
bool SQLiteClothFrame::insert(const ClothFrameData& data)
{
	std::string valueString;
	appendValueInt(valueString, data.mID);
	appendValueInt(valueString, data.mDirection);
	appendValueString(valueString, data.mAction);
	appendValueInt(valueString, data.mFrameCount);
	appendValueFloatArray(valueString, data.mPosX);
	appendValueFloatArray(valueString, data.mPosY, true);
	return doInsert(valueString);
}

bool SQLiteClothFrame::insertOrUpdate(const ClothFrameData& data)
{
	bool ret = false;
	txVector<ClothFrameData> tempList;
	query(data.mID, data.mDirection, data.mAction, tempList);
	if (tempList.size() == 0)
	{
		ret = insert(data);
	}
	else
	{
		ret = updateData(data);
	}
	return ret;
}

void SQLiteClothFrame::parseReader(SQLiteDataReader* reader, txVector<ClothFrameData>& dataList)
{
	while (reader->read())
	{
		ClothFrameData data;
		data.mID = reader->getInt(getCol(COL_ID));
		data.mDirection = reader->getInt(getCol(COL_DIRECTION));
		data.mAction = reader->getString(getCol(COL_ACTION));
		data.mFrameCount = reader->getInt(getCol(COL_FRAME_COUNT));
		StringUtility::stringToFloatArray(reader->getString(getCol(COL_POSX)), data.mPosX);
		StringUtility::stringToFloatArray(reader->getString(getCol(COL_POSY)), data.mPosY);
		dataList.push_back(data);
	}
	mSQLite->releaseReader(reader);
}