#ifndef _SQLITE_EFFECT_H_
#define _SQLITE_EFFECT_H_

#include "ServerDefine.h"
#include "SQLiteTable.h"

class EffectData
{
public:
	std::string mLabel;
	int mID;
	std::string mDesc;
	std::string mResource;
};

class SQLiteEffect : public SQLiteTable
{
	std::string COL_LABLE = "Label";
	std::string COL_ID = "ID";
	std::string COL_DESC = "Desc";
	std::string COL_RESOURCE = "Resource";
public:
	SQLiteEffect(SQLite* sqlite)
		:SQLiteTable("Effect", sqlite)
	{
		registeColumn(COL_LABLE);
		registeColumn(COL_ID);
		registeColumn(COL_DESC);
		registeColumn(COL_RESOURCE);
	}
	void query(int weaponID, EffectData& data);
	bool insert(const EffectData& data);
protected:
	void parseReader(SQLiteDataReader* reader, EffectData& data);
};

#endif