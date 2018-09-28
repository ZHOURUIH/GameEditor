#ifndef _SQLITE_DATA_READER_H_
#define _SQLITE_DATA_READER_H_

#include "ServerDefine.h"

enum SQLITE_DATATYPE
{
	SQLITE_DATATYPE_INTEGER = SQLITE_INTEGER,
	SQLITE_DATATYPE_FLOAT = SQLITE_FLOAT,
	SQLITE_DATATYPE_TEXT = SQLITE_TEXT,
	SQLITE_DATATYPE_BLOB = SQLITE_BLOB,
	SQLITE_DATATYPE_NULL = SQLITE_NULL,
};

class SQLiteDataReader 
{
public:
	SQLiteDataReader(sqlite3_stmt* pStmt);	
	~SQLiteDataReader(); 
public:	
	bool read();// 读取一行数据,需要循环调用来读取多行数据
	void close();
	int getColumnCount();
	std::string getColumnName(int col);
	SQLITE_DATATYPE getDataType(int col);
	std::string getString(int col);
	int getInt(int col);
	float getFloat(int col);
	const char* getBlob(int col, int& length);
private:
	sqlite3_stmt* m_pStmt;
};

#endif