#ifndef _SQLITE_H_
#define _SQLITE_H_

#include "ServerDefine.h"

class SQLiteCloth;
class SQLiteClothFrame;
class SQLiteWeapon;
class SQLiteWeaponFrame;
class SQLiteMonster;
class SQLiteMonsterFrame;
class SQLiteEffect;
class SQLiteEffectFrame;
class SQLiteDataReader;
class SQLite
{
public:
	sqlite3* mSQlite3;
	SQLiteCloth* mSQLiteCloth;
	SQLiteClothFrame* mSQLiteClothFrame;
	SQLiteWeapon* mSQLiteWeapon;
	SQLiteWeaponFrame* mSQLiteWeaponFrame;
	SQLiteMonster* mSQLiteMonster;
	SQLiteMonsterFrame* mSQLiteMonsterFrame;
	SQLiteEffect* mSQLiteEffect;
	SQLiteEffectFrame* mSQLiteEffectFrame;
public:
	SQLite(const std::string& dbFileName);
	~SQLite() { destroy(); }
	void destroy();
	bool executeNonQuery(const std::string& queryString);
	SQLiteDataReader* executeQuery(const std::string& queryString);
	void releaseReader(SQLiteDataReader* reader);
};

#endif