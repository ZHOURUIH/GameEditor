#include "SQLite.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLiteSceneMap.h"
#include "SQLiteSceneMapFile.h"
#include "SQLiteSceneMapTransfer.h"
#include "SQLiteNPC.h"
#include "SQLiteGoods.h"
#include "SQLiteMonGen.h"
#include "SQLiteMonster.h"
#include "SQLiteMonsterTemplate.h"
#include "SQLiteMagic.h"
#include "SQLiteStdItem.h"
#include "SQLiteImagePositionAnimation.h"
#include "SQLiteImagePositionEffect.h"
#include "SQLiteImagePositionIcon.h"
#include "SQLiteMonsterCollider.h"
#include "SQLiteItemEquip.h"
#include "SQLiteItemConsumable.h"
#include "SQLiteItemSkillBook.h"
#include "SQLiteSkillEffectDirection.h"
#include "SQLitePeaceArea.h"
#include "SQLiteMapEffect.h"

#define NEW_TABLE(type) m##type = TRACE_NEW(type, m##type, this)
#define DELETE_TABLE(type) TRACE_DELETE(m##type)

SQLite::SQLite(const string& dbFileName)
{
	// 打开数据库链接
	int ret = sqlite3_open(dbFileName.c_str(), &mSQlite3);
	if (ret != SQLITE_OK)
	{
		string errorInfo = sqlite3_errmsg(mSQlite3);
		return;
	}
	NEW_TABLE(SQLiteSceneMap);
	NEW_TABLE(SQLiteSceneMapFile);
	NEW_TABLE(SQLiteSceneMapTransfer);
	NEW_TABLE(SQLiteNPC);
	NEW_TABLE(SQLiteMonGen);
	NEW_TABLE(SQLiteMonster);
	NEW_TABLE(SQLiteMonsterTemplate);
	NEW_TABLE(SQLiteMagic);
	NEW_TABLE(SQLiteStdItem);
	NEW_TABLE(SQLiteImagePositionAnimation);
	NEW_TABLE(SQLiteImagePositionEffect);
	NEW_TABLE(SQLiteImagePositionIcon);
	NEW_TABLE(SQLiteMonsterCollider);
	NEW_TABLE(SQLiteItemEquip);
	NEW_TABLE(SQLiteItemConsumable);
	NEW_TABLE(SQLiteItemSkillBook);
	NEW_TABLE(SQLiteSkillEffectDirection);
	NEW_TABLE(SQLitePeaceArea);
	NEW_TABLE(SQLiteGoods);
	NEW_TABLE(SQLiteMapEffect);
}
void SQLite::destroy()
{
	DELETE_TABLE(SQLiteSceneMap);
	DELETE_TABLE(SQLiteSceneMapFile);
	DELETE_TABLE(SQLiteSceneMapTransfer);
	DELETE_TABLE(SQLiteNPC);
	DELETE_TABLE(SQLiteMonGen);
	DELETE_TABLE(SQLiteMonster);
	DELETE_TABLE(SQLiteMonsterTemplate);
	DELETE_TABLE(SQLiteMagic);
	DELETE_TABLE(SQLiteStdItem);
	DELETE_TABLE(SQLiteImagePositionAnimation);
	DELETE_TABLE(SQLiteImagePositionEffect);
	DELETE_TABLE(SQLiteImagePositionIcon);
	DELETE_TABLE(SQLiteMonsterCollider);
	DELETE_TABLE(SQLiteItemEquip);
	DELETE_TABLE(SQLiteItemConsumable);
	DELETE_TABLE(SQLiteItemSkillBook);
	DELETE_TABLE(SQLiteSkillEffectDirection);
	DELETE_TABLE(SQLitePeaceArea);
	DELETE_TABLE(SQLiteGoods);
	DELETE_TABLE(SQLiteMapEffect);
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