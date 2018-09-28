#include "SQLiteCloth.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

void SQLiteCloth::query(int clothID, ClothData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, clothID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteCloth::insert(const ClothData& data)
{
	std::string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc, true);
	return doInsert(valueString);
}

void SQLiteCloth::parseReader(SQLiteDataReader* reader, ClothData& data)
{
	while (reader->read())
	{
		data.mLabel = reader->getString(getCol(COL_LABLE));
		data.mID = reader->getInt(getCol(COL_ID));
		data.mDesc = reader->getString(getCol(COL_DESC));
		break;
	}
	mSQLite->releaseReader(reader);
}