#ifndef _SQLITE_CLOTH_H_
#define _SQLITE_CLOTH_H_

#include "SQLiteTable.h"

class ClothData
{
public:
	std::string mLabel;
	int mID;
	std::string mDesc;
};

class SQLiteCloth : public SQLiteTable
{
	std::string COL_LABLE = "ClothLabel";
	std::string COL_ID = "ClothID";
	std::string COL_DESC = "Desc";
public:
	SQLiteCloth(SQLite* sqlite)
		:SQLiteTable("Cloth", sqlite)
	{
		registeColumn(COL_LABLE);
		registeColumn(COL_ID);
		registeColumn(COL_DESC);
	}
	void query(int clothID, ClothData& data);
	bool insert(const ClothData& data);
protected:
	void parseReader(SQLiteDataReader* reader, ClothData& data);
};

#endif