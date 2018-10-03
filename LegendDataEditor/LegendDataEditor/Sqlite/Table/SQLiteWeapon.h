#ifndef _SQLITE_WEAPON_H_
#define _SQLITE_WEAPON_H_

#include "ServerDefine.h"
#include "SQLiteTable.h"

class WeaponData
{
public:
	std::string mLabel;
	int mID;
	std::string mDesc;
	std::string mOccupation;
	std::string mResource;
};

class SQLiteWeapon : public SQLiteTable
{
	std::string COL_LABLE = "WeaponLabel";
	std::string COL_ID = "WeaponID";
	std::string COL_DESC = "Desc";
	std::string COL_OCCUPATION = "Occupation";
	std::string COL_RESOURCE = "Resource";
public:
	SQLiteWeapon(SQLite* sqlite)
		:SQLiteTable("Weapon", sqlite)
	{
		registeColumn(COL_LABLE);
		registeColumn(COL_ID);
		registeColumn(COL_DESC);
		registeColumn(COL_OCCUPATION);
		registeColumn(COL_RESOURCE);
	}
	void query(int weaponID, WeaponData& data);
	bool insert(const WeaponData& data);
protected:
	void parseReader(SQLiteDataReader* reader, WeaponData& data);
};

#endif