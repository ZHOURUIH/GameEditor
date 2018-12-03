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