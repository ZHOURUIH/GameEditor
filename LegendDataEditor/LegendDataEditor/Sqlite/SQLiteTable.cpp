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

bool SQLiteTable::doDelete(const string& condition)
{
	if (condition != EMPTY_STRING)
	{
		string str = "DELETE FROM " + mTableName + " WHERE " + condition;
		return mSQLite->executeNonQuery(str);
	}
	else
	{
		string str = "DELETE FROM " + mTableName;
		return mSQLite->executeNonQuery(str);
	}
}