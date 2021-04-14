#include "SQLiteTableBase.h"

void SQLiteTableBase::init(const string& fullPath)
{
	int ret = sqlite3_open(fullPath.c_str(), &mSQlite3);
	if (ret != SQLITE_OK)
	{
		//ERROR("can not open sqlite, info : " + string(sqlite3_errmsg(mSQlite3)));
	}
}
SQLiteTableBase::~SQLiteTableBase()
{
	if (mSQlite3 != NULL)
	{
		sqlite3_close(mSQlite3);
		mSQlite3 = NULL;
	}
}
bool SQLiteTableBase::doUpdate(const char* updateString, const char* conditionString)
{
	array<char, 1024> queryStr{ 0 };
	STRCAT6(queryStr, "UPDATE ", mTableName, " SET ", updateString, " WHERE ", conditionString);
	return executeNonQuery(queryStr.data());
}
bool SQLiteTableBase::doInsert(const char* valueString)
{
	array<char, 1024> queryStr{ 0 };
	STRCAT5(queryStr, "INSERT INTO ", mTableName, " VALUES (", valueString, ")");
	return executeNonQuery(queryStr.data());
}
bool SQLiteTableBase::executeNonQuery(const char* queryString)
{
	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(mSQlite3, queryString, -1, &stmt, NULL) != SQLITE_OK)
	{
		return false;
	}
	sqlite3_step(stmt);
	return (sqlite3_finalize(stmt) == SQLITE_OK) ? true : false;
}
SQLiteDataReader* SQLiteTableBase::executeQuery(const char* queryString)
{
	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(mSQlite3, queryString, -1, &stmt, NULL) != SQLITE_OK)
	{
		return NULL;
	}
	SQLiteDataReader* reader = NEW(SQLiteDataReader, reader, stmt);
	return reader;
}
void SQLiteTableBase::releaseReader(SQLiteDataReader* reader)
{
	DELETE(reader);
}