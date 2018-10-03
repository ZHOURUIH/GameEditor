#ifndef _SQLITE_TABLE_H_
#define _SQLITE_TABLE_H_

#include "ServerDefine.h"

class SQLite;
class SQLiteDataReader;
class SQLiteTable
{
protected:
	std::string mTableName;
	SQLite* mSQLite;
	txVector<std::string> COL_NAME;
public:
	SQLiteTable(const std::string& name, SQLite* sqlite);
	int getCol(const std::string& colName);
	void registeColumn(const std::string& colName);
	bool doUpdate(const std::string& updateString, const std::string& conditionString);
	bool doInsert(const std::string& valueString);
	void appendValueString(std::string& queryStr, const std::string& str, bool isEnd = false);
	void appendValueInt(std::string& queryStr, int value, bool isEnd = false);
	void appendValueIntArray(std::string& queryStr, const txVector<int>& intArray, bool isEnd = false);
	void appendConditionString(std::string& condition, const std::string& col, const std::string& str, const std::string& operate);
	void appendConditionInt(std::string& condition, const std::string& col, int value, const std::string& operate);
	void appendUpdateString(std::string& updateInfo, const std::string& col, const std::string& str, bool isEnd = false);
	void appendUpdateInt(std::string& updateInfo, const std::string& col, int value, bool isEnd = false);
	void appendUpdateIntArray(std::string& updateInfo, const std::string& col, const txVector<int>& intArray, bool isEnd = false);
};

#endif