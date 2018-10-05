#include "SQLiteEffect.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

void SQLiteEffect::query(int weaponID, EffectData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, weaponID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteEffect::insert(const EffectData& data)
{
	std::string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc);
	appendValueString(valueString, data.mResource);
	appendValueInt(valueString, data.mDirectionCount, true);
	return doInsert(valueString);
}

void SQLiteEffect::parseReader(SQLiteDataReader* reader, EffectData& data)
{
	while (reader->read())
	{
		data.mLabel = reader->getString(getCol(COL_LABLE));
		data.mID = reader->getInt(getCol(COL_ID));
		data.mDesc = reader->getString(getCol(COL_DESC));
		data.mResource = reader->getString(getCol(COL_RESOURCE));
		data.mDirectionCount = reader->getInt(getCol(COL_DIRECITON_COUNT));
		break;
	}
	mSQLite->releaseReader(reader);
}