#ifndef _SQLITE_TABLE_H_
#define _SQLITE_TABLE_H_

#include "ServerDefine.h"
#include "SQLiteDataReader.h"
#include "Utility.h"
#include "SQLite.h"

class SQLite;
class SQLiteTable
{
protected:
	string mTableName;
	SQLite* mSQLite;
public:
	SQLiteTable(const string& name, SQLite* sqlite);
	template<typename T>
	void queryAll(txVector<T*>& dataList)
	{
		doSelect(dataList);
	}
	void deleteAll()
	{
		doDelete("");
	}
	template<typename T>
	bool insert(T& data)
	{
		string valueString;
		data.insert(valueString);
		StringUtility::removeLastComma(valueString);
		return doInsert(valueString);
	}
	template<typename T>
	bool update(T& data)
	{
		string updateString;
		data.update(updateString);
		StringUtility::removeLastComma(updateString);
		string conditionString;
		StringUtility::appendConditionInt(conditionString, T::COL_ID, data.mID, "");
		return doUpdate(updateString, conditionString);
	}
	int getMaxID()
	{
		int maxID = 0;
		string queryStr = "SELECT max(ID) FROM " + mTableName;
		SQLiteDataReader* reader = mSQLite->executeQuery(queryStr);
		if (reader != NULL)
		{
			while (reader->read())
			{
				maxID = StringUtility::stringToInt(reader->getString(0));
				break;
			}
			mSQLite->releaseReader(reader);
		}
		return maxID;
	}
protected:
	template<typename T>
	void doSelect(txVector<T*>& dataList, const string& conditionString = "")
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
	bool doDelete(const string& valueString);
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
	void parseReader(SQLiteDataReader* reader, txVector<T*>& dataList)
	{
		if (reader == NULL)
		{
			return;
		}
		while (reader->read())
		{
			T* data = TRACE_NEW(T, data);
			data->parse(reader);
			dataList.push_back(data);
		}
		mSQLite->releaseReader(reader);
	}
};

#endif