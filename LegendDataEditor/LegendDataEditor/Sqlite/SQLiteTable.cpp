#include "SQLiteTable.h"
#include "SQLite.h"
#include "Utility.h"

void SQLiteTableData::parse(SQLiteDataReader* reader, SQLiteTable* table)
{
	int count = mParameters.size();
	for (int i = 0; i < count; ++i)
	{
		if (mParameters[i].mType == typeid(string).name())
		{
			*((string*)mParameters[i].mPointer) = reader->getString(i);
		}
		else if (mParameters[i].mType == typeid(int).name())
		{
			*((int*)mParameters[i].mPointer) = reader->getInt(i);
		}
		else if (mParameters[i].mType == typeid(txVector<int>).name())
		{
			StringUtility::stringToIntArray(reader->getString(i), *((txVector<int>*)mParameters[i].mPointer));
		}
		else
		{
			assert("SQLite参数类型错误");
		}
	}
}
void SQLiteTableData::insert(string& valueString) const
{
	int count = mParameters.size();
	for (int i = 0; i < count; ++i)
	{
		if (mParameters[i].mType == typeid(string).name())
		{
			StringUtility::appendValueString(valueString, *((string*)mParameters[i].mPointer));
		}
		else if (mParameters[i].mType == typeid(int).name())
		{
			StringUtility::appendValueInt(valueString, *((int*)mParameters[i].mPointer));
		}
		else if (mParameters[i].mType == typeid(txVector<int>).name())
		{
			StringUtility::appendValueIntArray(valueString, *((txVector<int>*)mParameters[i].mPointer));
		}
		else
		{
			assert("SQLite参数类型错误");
		}
	}
}
void SQLiteTableData::update(string& updateString) const
{
	int count = mParameters.size();
	for (int i = 0; i < count; ++i)
	{
		if (mParameters[i].mType == typeid(string).name())
		{
			StringUtility::appendUpdateString(updateString, mParameters[i].mCol, *((string*)mParameters[i].mPointer));
		}
		else if (mParameters[i].mType == typeid(int).name())
		{
			StringUtility::appendUpdateInt(updateString, mParameters[i].mCol, *((int*)mParameters[i].mPointer));
		}
		else if (mParameters[i].mType == typeid(txVector<int>).name())
		{
			StringUtility::appendUpdateIntArray(updateString, mParameters[i].mCol, *((txVector<int>*)mParameters[i].mPointer));
		}
		else
		{
			assert("SQLite参数类型错误");
		}
	}
}

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