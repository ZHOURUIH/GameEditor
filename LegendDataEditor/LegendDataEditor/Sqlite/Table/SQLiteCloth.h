#ifndef _SQLITE_CLOTH_H_
#define _SQLITE_CLOTH_H_

#include "SQLiteTable.h"

class ClothData
{
public:
	std::string mLabel;
	int mID;
	std::string mDesc;
	std::string mOccupation;
	std::string mResource;
};

class SQLiteCloth : public SQLiteTable
{
	std::string COL_LABLE = "ClothLabel";
	std::string COL_ID = "ClothID";
	std::string COL_DESC = "Desc";
	std::string COL_OCCUPATION = "Occupation";
	std::string COL_RESOURCE = "Resource";
public:
	SQLiteCloth(SQLite* sqlite)
		:SQLiteTable("Cloth", sqlite)
	{
		registeColumn(COL_LABLE);
		registeColumn(COL_ID);
		registeColumn(COL_DESC);
		registeColumn(COL_OCCUPATION);
		registeColumn(COL_RESOURCE);
	}
	void query(int clothID, ClothData& data);
	bool insert(const ClothData& data);
protected:
	void parseReader(SQLiteDataReader* reader, ClothData& data);
};

#endif