#include "SQLiteTable.h"
#include "SQLite.h"
#include "Utility.h"

SQLiteTable::SQLiteTable(const std::string& name, SQLite* sqlite)
{
	mTableName = name;
	mSQLite = sqlite;
}

bool SQLiteTable::doUpdate(const std::string& updateString, const std::string& conditionString)
{
	std::string queryStr = "UPDATE " + mTableName + " SET " + updateString + " WHERE " + conditionString;
	return mSQLite->executeNonQuery(queryStr);
}
bool SQLiteTable::doInsert(const std::string& valueString)
{
	std::string queryString = "INSERT INTO " + mTableName + " VALUES (" + valueString + ")";
	return mSQLite->executeNonQuery(queryString);
}
void SQLiteTable::appendValueString(std::string& queryStr, const std::string& str, bool isEnd)
{
	queryStr += "\"" + str + "\"";
	if (!isEnd)
	{
		queryStr += ",";
	}
}
void SQLiteTable::appendValueInt(std::string& queryStr, int value, bool isEnd)
{
	queryStr += StringUtility::intToString(value);
	if (!isEnd)
	{
		queryStr += ",";
	}
}
void SQLiteTable::appendValueIntArray(std::string& queryStr, const txVector<int>& intArray, bool isEnd)
{
	appendValueString(queryStr, StringUtility::intArrayToString(intArray), isEnd);
}
void SQLiteTable::appendConditionString(std::string& condition, const std::string& col, const std::string& str, const std::string& operate)
{
	condition += col + " = " + "\"" + str + "\"" + operate;
}
void SQLiteTable::appendConditionInt(std::string& condition, const std::string& col, int value, const std::string& operate)
{
	condition += col + " = " + StringUtility::intToString(value) + operate;
}
void SQLiteTable::appendUpdateString(std::string& updateInfo, const std::string& col, const std::string& str, bool isEnd)
{
	updateInfo += col + " = " + "\"" + str + "\"";
	if (!isEnd)
	{
		updateInfo += ",";
	}
}
void SQLiteTable::appendUpdateInt(std::string& updateInfo, const std::string& col, int value, bool isEnd)
{
	updateInfo += col + " = " + StringUtility::intToString(value);
	if (!isEnd)
	{
		updateInfo += ",";
	}
}
void SQLiteTable::appendUpdateIntArray(std::string& updateInfo, const std::string& col, const txVector<int>& intArray, bool isEnd)
{
	appendUpdateString(updateInfo, col, StringUtility::intArrayToString(intArray), isEnd);
}