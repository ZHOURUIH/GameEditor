#ifndef _SQLITE_TABLE_DATA_H_
#define _SQLITE_TABLE_DATA_H_

#include "ServerDefine.h"

class TableParam
{
public:
	void* mPointer;
	string mCol;
	string mType;
	TableParam(void* pointer, const string& col, const string& type)
	{
		mPointer = pointer;
		mCol = col;
		mType = type;
	}
};

#define REGISTE_PARAM(param, col) mParameters.push_back(TableParam(&param, col, typeid(param).name()));

class SQLiteDataReader;
class SQLiteTableData
{
protected:
	txVector<TableParam> mParameters;
public:
	void parse(SQLiteDataReader* reader);
	void insert(string& valueString) const;
	void update(string& updateString) const;
};

#endif