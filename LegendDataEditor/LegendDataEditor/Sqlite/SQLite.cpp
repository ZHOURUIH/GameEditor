#include "SQLite.h"
#include "Utility.h"
#include "SQLiteEquip.h"
#include "SQLiteEquipFrame.h"
#include "SQLiteMonster.h"
#include "SQLiteMonsterFrame.h"
#include "SQLiteEffect.h"
#include "SQLiteEffectFrame.h"
#include "SQLiteDataReader.h"
#include "SQLiteSceneMap.h"
#include "SQLiteNPC.h"
#include "SQLiteNPCFrame.h"

SQLite::SQLite(const string& dbFileName)
{
	// 打开数据库链接
	int ret = sqlite3_open(dbFileName.c_str(), &mSQlite3);
	if (ret != SQLITE_OK)
	{
		string errorInfo = sqlite3_errmsg(mSQlite3);
		return;
	}
	mSQLiteEquip = TRACE_NEW(SQLiteEquip, mSQLiteEquip, this);
	mSQLiteEquipFrame = TRACE_NEW(SQLiteEquipFrame, mSQLiteEquipFrame, this);
	mSQLiteMonster = TRACE_NEW(SQLiteMonster, mSQLiteMonster, this);
	mSQLiteMonsterFrame = TRACE_NEW(SQLiteMonsterFrame, mSQLiteMonsterFrame, this);
	mSQLiteEffect = TRACE_NEW(SQLiteEffect, mSQLiteEffect, this);
	mSQLiteEffectFrame = TRACE_NEW(SQLiteEffectFrame, mSQLiteEffectFrame, this);
	mSQLiteSceneMap = TRACE_NEW(SQLiteSceneMap, mSQLiteSceneMap, this);
	mSQLiteNPC = TRACE_NEW(SQLiteNPC, mSQLiteNPC, this);
	mSQLiteNPCFrame = TRACE_NEW(SQLiteNPCFrame, mSQLiteNPCFrame, this);
}
void SQLite::destroy()
{
	TRACE_DELETE(mSQLiteEquip);
	TRACE_DELETE(mSQLiteEquipFrame);
	TRACE_DELETE(mSQLiteMonster);
	TRACE_DELETE(mSQLiteMonsterFrame);
	TRACE_DELETE(mSQLiteEffect);
	TRACE_DELETE(mSQLiteEffectFrame);
	TRACE_DELETE(mSQLiteSceneMap);
	TRACE_DELETE(mSQLiteNPC);
	sqlite3_close(mSQlite3);
}
bool SQLite::executeNonQuery(const string& queryString)
{
	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(mSQlite3, queryString.c_str(), -1, &stmt, NULL) != SQLITE_OK)
	{
		return false;
	}
	sqlite3_step(stmt);
	return (sqlite3_finalize(stmt) == SQLITE_OK) ? true : false;
}

SQLiteDataReader* SQLite::executeQuery(const string& queryString)
{
	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(mSQlite3, queryString.c_str(), -1, &stmt, NULL) != SQLITE_OK)
	{
		return NULL;
	}
	SQLiteDataReader* reader = TRACE_NEW(SQLiteDataReader, reader, stmt);
	return reader;
}

void SQLite::releaseReader(SQLiteDataReader* reader)
{
	TRACE_DELETE(reader);
}