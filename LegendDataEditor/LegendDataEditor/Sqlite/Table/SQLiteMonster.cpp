#include "SQLiteMonster.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

void SQLiteMonster::query(int monsterID, MonsterData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, monsterID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteMonster::insert(const MonsterData& data)
{
	std::string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc);
	appendValueString(valueString, data.mPrefab);
	appendValueString(valueString, data.mResource, true);
	return doInsert(valueString);
}

void SQLiteMonster::parseReader(SQLiteDataReader* reader, MonsterData& data)
{
	while (reader->read())
	{
		data.mLabel = reader->getString(getCol(COL_LABLE));
		data.mID = reader->getInt(getCol(COL_ID));
		data.mDesc = reader->getString(getCol(COL_DESC));
		data.mPrefab = reader->getString(getCol(COL_PREFAB));
		data.mResource = reader->getString(getCol(COL_RESOURCE));
		break;
	}
	mSQLite->releaseReader(reader);
}