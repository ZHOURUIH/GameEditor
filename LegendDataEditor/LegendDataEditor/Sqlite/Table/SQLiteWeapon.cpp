#include "SQLiteWeapon.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

std::string WeaponData::COL_LABLE = "WeaponLabel";
std::string WeaponData::COL_ID = "WeaponID";
std::string WeaponData::COL_DESC = "Desc";
std::string WeaponData::COL_OCCUPATION = "Occupation";
std::string WeaponData::COL_RESOURCE = "Resource";

void SQLiteWeapon::query(int weaponID, WeaponData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, WeaponData::COL_ID, weaponID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteWeapon::insert(const WeaponData& data)
{
	std::string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc);
	appendValueString(valueString, data.mOccupation);
	appendValueString(valueString, data.mResource, true);
	return doInsert(valueString);
}