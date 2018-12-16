#ifndef _SQLITE_H_
#define _SQLITE_H_

#include "ServerDefine.h"

class SQLiteEquip;
class SQLiteMonster;
class SQLiteEffect;
class SQLiteDataReader;
class SQLiteSceneMap;
class SQLiteNPC;
class SQLiteMonGen;
class SQLiteMonsterInfo;
class SQLiteMagic;
class SQLiteStdItem;
class SQLiteAnimationFrame;
class SQLite
{
public:
	sqlite3* mSQlite3;
	SQLiteEquip* mSQLiteEquip;
	SQLiteMonster* mSQLiteMonster;
	SQLiteEffect* mSQLiteEffect;
	SQLiteSceneMap* mSQLiteSceneMap;
	SQLiteNPC* mSQLiteNPC;
	SQLiteMonGen* mSQLiteMonGen;
	SQLiteMonsterInfo* mSQLiteMonsterInfo;
	SQLiteMagic* mSQLiteMagic;
	SQLiteStdItem* mSQLiteStdItem;
	SQLiteAnimationFrame* mSQLiteAnimationFrame;
public:
	SQLite(const string& dbFileName);
	~SQLite() { destroy(); }
	void destroy();
	bool executeNonQuery(const string& queryString);
	SQLiteDataReader* executeQuery(const string& queryString);
	void releaseReader(SQLiteDataReader* reader);
};

#endif