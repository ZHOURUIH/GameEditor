#include "SQLite.h"
#include "Utility.h"
#include "SQLiteCloth.h"
#include "SQLiteClothFrame.h"
#include "SQLiteWeapon.h"
#include "SQLiteWeaponFrame.h"
#include "SQLiteMonster.h"
#include "SQLiteMonsterFrame.h"
#include "SQLiteEffect.h"
#include "SQLiteEffectFrame.h"
#include "SQLiteDataReader.h"

SQLite::SQLite(const std::string& dbFileName)
{
	// 打开数据库链接
	int ret = sqlite3_open(dbFileName.c_str(), &mSQlite3);
	if (ret != SQLITE_OK)
	{
		std::string errorInfo = sqlite3_errmsg(mSQlite3);
		return;
	}
	mSQLiteCloth = new SQLiteCloth(this);
	mSQLiteClothFrame = new SQLiteClothFrame(this);
	mSQLiteWeapon = new SQLiteWeapon(this);
	mSQLiteWeaponFrame = new SQLiteWeaponFrame(this);
	mSQLiteMonster = new SQLiteMonster(this);
	mSQLiteMonsterFrame = new SQLiteMonsterFrame(this);
	mSQLiteEffect = new SQLiteEffect(this);
	mSQLiteEffectFrame = new SQLiteEffectFrame(this);
}
void SQLite::destroy()
{
	TRACE_DELETE(mSQLiteCloth);
	TRACE_DELETE(mSQLiteClothFrame);
	TRACE_DELETE(mSQLiteWeapon);
	TRACE_DELETE(mSQLiteWeaponFrame);
	TRACE_DELETE(mSQLiteMonster);
	TRACE_DELETE(mSQLiteMonsterFrame);
	TRACE_DELETE(mSQLiteEffect);
	TRACE_DELETE(mSQLiteEffectFrame);
	sqlite3_close(mSQlite3);
}
bool SQLite::executeNonQuery(const std::string& queryString)
{
	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(mSQlite3, queryString.c_str(), -1, &stmt, NULL) != SQLITE_OK)
	{
		return false;
	}
	sqlite3_step(stmt);
	return (sqlite3_finalize(stmt) == SQLITE_OK) ? true : false;
}

SQLiteDataReader* SQLite::executeQuery(const std::string& queryString)
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