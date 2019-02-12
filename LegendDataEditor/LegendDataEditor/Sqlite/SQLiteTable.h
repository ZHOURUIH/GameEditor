#ifndef _SQLITE_TABLE_H_
#define _SQLITE_TABLE_H_

#include "ServerDefine.h"
#include "SQLiteDataReader.h"
#include "Utility.h"

class SQLite;
class SQLiteTable
{
protected:
	string mTableName;
	SQLite* mSQLite;
public:
	SQLiteTable(const string& name, SQLite* sqlite);
	template<typename T>
	void queryAll(txVector<T>& dataList)
	{
		doSelect(dataList);
	}
protected:
	template<typename T>
	void doSelect(txVector<T>& dataList, const string& conditionString = "")
	{
		string queryStr = "";
		if (conditionString != "")
		{
			queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
		}
		else
		{
			queryStr = "SELECT * FROM " + mTableName;
		}
		parseReader(mSQLite->executeQuery(queryStr), dataList);
	}
	template<typename T>
	bool doSelect(T& data, const string& conditionString = "")
	{
		string queryStr = "";
		if (conditionString != "")
		{
			queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
		}
		else
		{
			queryStr = "SELECT * FROM " + mTableName;
		}
		return parseReader(mSQLite->executeQuery(queryStr), data);
	}
	bool doUpdate(const string& updateString, const string& conditionString);
	bool doInsert(const string& valueString);
	template<typename T>
	bool parseReader(SQLiteDataReader* reader, T& data)
	{
		bool ret = false;
		if (reader->read())
		{
			data.parse(reader);
			ret = true;
		}
		mSQLite->releaseReader(reader);
		return ret;
	}
	template<typename T>
	void parseReader(SQLiteDataReader* reader, txVector<T>& dataList)
	{
		while (reader->read())
		{
			T data;
			data.parse(reader);
			dataList.push_back(data);
		}
		mSQLite->releaseReader(reader);
	}
};

#endif