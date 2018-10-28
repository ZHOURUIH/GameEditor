#ifndef _SQLITE_CLOTH_H_
#define _SQLITE_CLOTH_H_

#include "SQLiteTable.h"

class ClothData : public SQLiteTableData
{
public:
	static std::string COL_LABLE;
	static std::string COL_ID;
	static std::string COL_DESC;
	static std::string COL_OCCUPATION;
	static std::string COL_RESOURCE;
	std::string mLabel;
	int mID;
	std::string mDesc;
	std::string mOccupation;
	std::string mResource;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mLabel = reader->getString(table->getCol(COL_LABLE));
		mID = reader->getInt(table->getCol(COL_ID));
		mDesc = reader->getString(table->getCol(COL_DESC));
		mOccupation = reader->getString(table->getCol(COL_OCCUPATION));
		mResource = reader->getString(table->getCol(COL_RESOURCE));
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_LABLE);
		table->registeColumn(COL_ID);
		table->registeColumn(COL_DESC);
		table->registeColumn(COL_OCCUPATION);
		table->registeColumn(COL_RESOURCE);
	}
};

class SQLiteCloth : public SQLiteTable
{
public:
	SQLiteCloth(SQLite* sqlite)
		:SQLiteTable("Cloth", sqlite)
	{
		ClothData::registeColumn(this);
	}
	void query(int clothID, ClothData& data);
	bool insert(const ClothData& data);
};

#endif