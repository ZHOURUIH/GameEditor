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
	bool doUpdate(const string& updateString, const string& conditionString);
	bool doInsert(const string& valueString);
	void appendValueString(string& queryStr, const string& str, bool isEnd = false);
	void appendValueInt(string& queryStr, int value, bool isEnd = false);
	void appendValueIntArray(string& queryStr, const txVector<int>& intArray, bool isEnd = false);
	void appendConditionString(string& condition, const string& col, const string& str, const string& operate);
	void appendConditionInt(string& condition, const string& col, int value, const string& operate);
	void appendUpdateString(string& updateInfo, const string& col, const string& str, bool isEnd = false);
	void appendUpdateInt(string& updateInfo, const string& col, int value, bool isEnd = false);
	void appendUpdateIntArray(string& updateInfo, const string& col, const txVector<int>& intArray, bool isEnd = false);
	template<typename T>
	void parseReader(SQLiteDataReader* reader, T& data)
	{
		if (reader->read())
		{
			data.parse(reader, this);
		}
		mSQLite->releaseReader(reader);
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