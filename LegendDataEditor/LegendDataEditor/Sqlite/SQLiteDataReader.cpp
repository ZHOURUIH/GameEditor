#include "SQLiteDataReader.h"

SQLiteDataReader::SQLiteDataReader(sqlite3_stmt* pStmt)
	:m_pStmt(pStmt)
{}

SQLiteDataReader::~SQLiteDataReader()
{
	close();
}

bool SQLiteDataReader::read()
{
	if (m_pStmt == NULL)
	{
		return false;
	}
	if (sqlite3_step(m_pStmt) != SQLITE_ROW)
	{
		return false;
	}
	return true;
}

void SQLiteDataReader::close()
{
	if (m_pStmt != NULL)
	{
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}
}
int SQLiteDataReader::getColumnCount()
{
	return sqlite3_column_count(m_pStmt);
}

std::string SQLiteDataReader::getColumnName(int col)
{
	return std::string(sqlite3_column_name(m_pStmt, col));
}

SQLITE_DATATYPE SQLiteDataReader::getDataType(int col)
{
	return (SQLITE_DATATYPE)sqlite3_column_type(m_pStmt, col);
}

std::string SQLiteDataReader::getString(int col)
{
	return std::string((char*)sqlite3_column_text(m_pStmt, col));
}

int SQLiteDataReader::getInt(int col)
{
	return sqlite3_column_int(m_pStmt, col);
}

float SQLiteDataReader::getFloat(int col)
{
	return (float)sqlite3_column_double(m_pStmt, col);
}

const char* SQLiteDataReader::getBlob(int col, int& length)
{
	length = sqlite3_column_bytes(m_pStmt, col);
	return (const char*)sqlite3_column_blob(m_pStmt, col);
}