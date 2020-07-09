#ifndef _SQLITE_H_
#define _SQLITE_H_

#include "ServerDefine.h"

class SQLiteDataReader;
class SQLiteSceneMap;
class SQLiteSceneMapFile;
class SQLiteSceneMapTransfer;
class SQLiteNPC;
class SQLiteMonGen;
class SQLiteMonster;
class SQLiteMonsterTemplate;
class SQLiteMagic;
class SQLiteStdItem;
class SQLiteImagePositionAnimation;
class SQLiteImagePositionEffect;
class SQLiteImagePositionIcon;
class SQLiteMonsterCollider;
class SQLiteItemEquip;
class SQLiteItemConsumable;
class SQLiteItemSkillBook;
class SQLiteSkillEffectDirection;
class SQLitePeaceArea;
class SQLiteGoods;
class SQLiteMapEffect;
class SQLite
{
public:
	sqlite3* mSQlite3;
	SQLiteSceneMap* mSQLiteSceneMap;
	SQLiteSceneMapFile* mSQLiteSceneMapFile;
	SQLiteSceneMapTransfer* mSQLiteSceneMapTransfer;
	SQLiteNPC* mSQLiteNPC;
	SQLiteMonGen* mSQLiteMonGen;
	SQLiteMonster* mSQLiteMonster;
	SQLiteMonsterTemplate* mSQLiteMonsterTemplate;
	SQLiteMagic* mSQLiteMagic;
	SQLiteStdItem* mSQLiteStdItem;
	SQLiteImagePositionAnimation* mSQLiteImagePositionAnimation;
	SQLiteImagePositionEffect* mSQLiteImagePositionEffect;
	SQLiteImagePositionIcon* mSQLiteImagePositionIcon;
	SQLiteMonsterCollider* mSQLiteMonsterCollider;
	SQLiteItemEquip* mSQLiteItemEquip;
	SQLiteItemConsumable* mSQLiteItemConsumable;
	SQLiteItemSkillBook* mSQLiteItemSkillBook;
	SQLiteSkillEffectDirection* mSQLiteSkillEffectDirection;
	SQLitePeaceArea* mSQLitePeaceArea;
	SQLiteGoods* mSQLiteGoods;
	SQLiteMapEffect* mSQLiteMapEffect;
public:
	SQLite(const string& dbFileName);
	virtual ~SQLite() { destroy(); }
	void destroy();
	bool executeNonQuery(const string& queryString);
	SQLiteDataReader* executeQuery(const string& queryString);
	void releaseReader(SQLiteDataReader* reader);
};

#endif