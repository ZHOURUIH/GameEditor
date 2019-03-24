#ifndef _SQLITE_H_
#define _SQLITE_H_

#include "ServerDefine.h"

class SQLiteDataReader;
class SQLiteSceneMap;
class SQLiteNPC;
class SQLiteMonGen;
class SQLiteMonsterInfo;
class SQLiteMagic;
class SQLiteStdItem;
class SQLiteAnimationFrame;
class SQLiteImagePosition;
class SQLiteMonsterCollider;
class SQLite
{
public:
	sqlite3* mSQlite3;
	SQLiteSceneMap* mSQLiteSceneMap;
	SQLiteNPC* mSQLiteNPC;
	SQLiteMonGen* mSQLiteMonGen;
	SQLiteMonsterInfo* mSQLiteMonsterInfo;
	SQLiteMagic* mSQLiteMagic;
	SQLiteStdItem* mSQLiteStdItem;
	SQLiteAnimationFrame* mSQLiteAnimationFrame;
	SQLiteImagePosition* mSQLiteImagePosition;
	SQLiteMonsterCollider* mSQLiteMonsterCollider;
public:
	SQLite(const string& dbFileName);
	virtual ~SQLite() { destroy(); }
	void destroy();
	bool executeNonQuery(const string& queryString);
	SQLiteDataReader* executeQuery(const string& queryString);
	void releaseReader(SQLiteDataReader* reader);
};

#endif