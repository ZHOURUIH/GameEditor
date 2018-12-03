#ifndef _SQLITE_H_
#define _SQLITE_H_

#include "ServerDefine.h"

class SQLiteEquip;
class SQLiteEquipFrame;
class SQLiteMonster;
class SQLiteMonsterFrame;
class SQLiteEffect;
class SQLiteEffectFrame;
class SQLiteDataReader;
class SQLiteSceneMap;
class SQLiteNPC;
class SQLiteNPCFrame;
class SQLiteMonGen;
class SQLiteMonsterInfo;
class SQLite
{
public:
	sqlite3* mSQlite3;
	SQLiteEquip* mSQLiteEquip;
	SQLiteEquipFrame* mSQLiteEquipFrame;
	SQLiteMonster* mSQLiteMonster;
	SQLiteMonsterFrame* mSQLiteMonsterFrame;
	SQLiteEffect* mSQLiteEffect;
	SQLiteEffectFrame* mSQLiteEffectFrame;
	SQLiteSceneMap* mSQLiteSceneMap;
	SQLiteNPC* mSQLiteNPC;
	SQLiteNPCFrame* mSQLiteNPCFrame;
	SQLiteMonGen* mSQLiteMonGen;
	SQLiteMonsterInfo* mSQLiteMonsterInfo;
public:
	SQLite(const string& dbFileName);
	~SQLite() { destroy(); }
	void destroy();
	bool executeNonQuery(const string& queryString);
	SQLiteDataReader* executeQuery(const string& queryString);
	void releaseReader(SQLiteDataReader* reader);
};

#endif