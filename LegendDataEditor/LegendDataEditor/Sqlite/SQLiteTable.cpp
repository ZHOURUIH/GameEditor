#include "SQLiteTable.h"
#include "SQLite.h"
#include "Utility.h"

SQLiteTable::SQLiteTable(const string& name, SQLite* sqlite)
{
	mTableName = name;
	mSQLite = sqlite;
}

bool SQLiteTable::doUpdate(const string& updateString, const string& conditionString)
{
	string queryStr = "UPDATE " + mTableName + " SET " + updateString + " WHERE " + conditionString;
	return mSQLite->executeNonQuery(queryStr);
}
bool SQLiteTable::doInsert(const string& valueString)
{
	string queryString = "INSERT INTO " + mTableName + " VALUES (" + valueString + ")";
	return mSQLite->executeNonQuery(queryString);
}
void SQLiteTable::appendValueString(string& queryStr, const string& str, bool isEnd)
{
	queryStr += "\"" + str + "\"";
	if (!isEnd)
	{
		queryStr += ",";
	}
}
void SQLiteTable::appendValueInt(string& queryStr, int value, bool isEnd)
{
	queryStr += StringUtility::intToString(value);
	if (!isEnd)
	{
		queryStr += ",";
	}
}
void SQLiteTable::appendValueIntArray(string& queryStr, const txVector<int>& intArray, bool isEnd)
{
	appendValueString(queryStr, StringUtility::intArrayToString(intArray), isEnd);
}
void SQLiteTable::appendConditionString(string& condition, const string& col, const string& str, const string& operate)
{
	condition += col + " = " + "\"" + str + "\"" + operate;
}
void SQLiteTable::appendConditionInt(string& condition, const string& col, int value, const string& operate)
{
	condition += col + " = " + StringUtility::intToString(value) + operate;
}
void SQLiteTable::appendUpdateString(string& updateInfo, const string& col, const string& str, bool isEnd)
{
	updateInfo += col + " = " + "\"" + str + "\"";
	if (!isEnd)
	{
		updateInfo += ",";
	}
}
void SQLiteTable::appendUpdateInt(string& updateInfo, const string& col, int value, bool isEnd)
{
	updateInfo += col + " = " + StringUtility::intToString(value);
	if (!isEnd)
	{
		updateInfo += ",";
	}
}
void SQLiteTable::appendUpdateIntArray(string& updateInfo, const string& col, const txVector<int>& intArray, bool isEnd)
{
	appendUpdateString(updateInfo, col, StringUtility::intArrayToString(intArray), isEnd);
}