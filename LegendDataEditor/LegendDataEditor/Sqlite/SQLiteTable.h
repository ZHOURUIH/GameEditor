#ifndef _SQLITE_TABLE_H_
#define _SQLITE_TABLE_H_

#include "ServerDefine.h"
#include "SQLiteDataReader.h"
#include "Utility.h"

class SQLiteTable;
class SQLiteTableData
{
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table) = 0;
	virtual void insert(string& valueString) const = 0;
	virtual void update(string& updateString) const = 0;
};

class SQLite;
class SQLiteTable
{
protected:
	string mTableName;
	SQLite* mSQLite;
	txVector<string> COL_NAME;
public:
	SQLiteTable(const string& name, SQLite* sqlite);
	int getCol(const string& colName)
	{
		int count = COL_NAME.size();
		for (int i = 0; i < count; ++i)
		{
			if (COL_NAME[i] == colName)
			{
				return i;
			}
		}
		return -1;
	}
	void registeColumn(const string& colName)
	{
		COL_NAME.push_back(colName);
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
			data.parse(reader, this);
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
			data.parse(reader, this);
			dataList.push_back(data);
		}
		mSQLite->releaseReader(reader);
	}
};

#endif