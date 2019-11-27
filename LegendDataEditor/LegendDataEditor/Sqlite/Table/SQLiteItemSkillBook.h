#ifndef _SQLITE_ITEM_SKILL_BOOK_H_
#define _SQLITE_ITEM_SKILL_BOOK_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class ItemSkillBookData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_NAME;
	static string COL_DESCRIPTION;
	static string COL_SKILL_ID;
	static string COL_JOB;
	static string COL_LEVEL;
	static string COL_ICON_ATLAS;
	static string COL_ICON;
	static string COL_PRICE;
	static string COL_WEIGHT;
public:
	int mID;
	string mName;
	string mDescription;
	int mSkillID;
	int mJob;
	int mLevel;
	string mIconAtlas;
	string mIcon;
	int mPrice;
	int mWeight;
public:
	ItemSkillBookData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mDescription, COL_DESCRIPTION);
		REGISTE_PARAM(mSkillID, COL_SKILL_ID);
		REGISTE_PARAM(mJob, COL_JOB);
		REGISTE_PARAM(mLevel, COL_LEVEL);
		REGISTE_PARAM(mIconAtlas, COL_ICON_ATLAS);
		REGISTE_PARAM(mIcon, COL_ICON);
		REGISTE_PARAM(mPrice, COL_PRICE);
		REGISTE_PARAM(mWeight, COL_WEIGHT);
	}
};

class SQLiteItemSkillBook : public SQLiteTable
{
public:
	SQLiteItemSkillBook(SQLite* sqlite)
		:SQLiteTable("ItemSkillBook", sqlite)
	{}
	bool query(int id, ItemSkillBookData& data);
	bool query(const string& name, ItemSkillBookData& data);
	bool insert(const ItemSkillBookData& data);
	bool update(const ItemSkillBookData& data);
};

#endif