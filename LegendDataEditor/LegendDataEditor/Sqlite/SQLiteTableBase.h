#ifndef _SQLITE_TABLE_BASE_H_
#define _SQLITE_TABLE_BASE_H_

#include "ServerDefine.h"
#include "SQLiteDataReader.h"
#include "ThreadLock.h"
#include "ServerBase.h"

class SQLiteTableBase : public ServerBase
{
protected:
	ThreadLock mThreadLock;
	const char* mTableName;
	sqlite3* mSQlite3;
public:
	void setTableName(const char* tableName) { mTableName = tableName; }
	const char* getTableName() const { return mTableName; }
	void init(const string& fullPath);
	virtual ~SQLiteTableBase();
	bool executeNonQuery(const char* queryString);
protected:
	bool doUpdate(const char* updateString, const char* conditionString);
	bool doInsert(const char* valueString);
	SQLiteDataReader* executeQuery(const char* queryString);
	void releaseReader(SQLiteDataReader* reader);
	// 因为此处要兼容非常规数据表格类型的数据类型(SelectCount),所以不使用T,而是重新定义一个新的模板类型
	template<typename Table>
	bool parseReader(SQLiteDataReader* reader, Table& data)
	{
		bool result = false;
		if (reader->read())
		{
			data.parse(reader);
			result = true;
		}
		releaseReader(reader);
		return result;
	}
	// 因为此处要兼容非常规数据表格类型的数据类型(SelectCount),所以不使用T,而是重新定义一个新的模板类型
	template<typename Table>
	void parseReader(SQLiteDataReader* reader, myVector<Table*>& dataList)
	{
		if (reader == NULL)
		{
			return;
		}
		while (reader->read())
		{
			Table* data = NEW(Table, data);
			data->parse(reader);
			dataList.push_back(data);
		}
		releaseReader(reader);
	}
};

#endif