#include "SQLiteWeapon.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

void SQLiteWeapon::query(int weaponID, WeaponData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, weaponID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteWeapon::insert(const WeaponData& data)
{
	std::string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc, true);
	return doInsert(valueString);
}

void SQLiteWeapon::parseReader(SQLiteDataReader* reader, WeaponData& data)
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