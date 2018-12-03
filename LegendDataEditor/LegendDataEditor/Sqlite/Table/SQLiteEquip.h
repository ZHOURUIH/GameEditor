#ifndef _SQLITE_CLOTH_H_
#define _SQLITE_CLOTH_H_

#include "SQLiteTable.h"

class EquipData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_LABEL;
	static string COL_EQUIP_TYPE;
	static string COL_DESC;
	static string COL_OCCUPATION;
	static string COL_RESOURCE;
	static string COL_CHAR_PANEL_RESOURCE_ATLAS;
	static string COL_CHAR_PANEL_RESOURCE_NAME;
	static string COL_CHAR_PANEL_RESOURCE_OFFSET;
public:
	int mID;
	string mLabel;
	int mEquipType;
	string mDesc;
	string mOccupation;
	string mResource;
	string mCharPanelResourceAtlas;
	string mCharPanelResourceName;
	string mCharPanelResourceOffset;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mID = reader->getInt(table->getCol(COL_ID));
		mLabel = reader->getString(table->getCol(COL_LABEL));
		mEquipType = reader->getInt(table->getCol(COL_EQUIP_TYPE));
		mDesc = reader->getString(table->getCol(COL_DESC));
		mOccupation = reader->getString(table->getCol(COL_OCCUPATION));
		mResource = reader->getString(table->getCol(COL_RESOURCE));
		mCharPanelResourceAtlas = reader->getString(table->getCol(COL_CHAR_PANEL_RESOURCE_ATLAS));
		mCharPanelResourceName = reader->getString(table->getCol(COL_CHAR_PANEL_RESOURCE_NAME));
		mCharPanelResourceOffset = reader->getString(table->getCol(COL_CHAR_PANEL_RESOURCE_OFFSET));
	}
	virtual void insert(string& valueString) const
	{
		StringUtility::appendValueInt(valueString, mID);
		StringUtility::appendValueString(valueString, mLabel);
		StringUtility::appendValueInt(valueString, mEquipType);
		StringUtility::appendValueString(valueString, mDesc);
		StringUtility::appendValueString(valueString, mOccupation);
		StringUtility::appendValueString(valueString, mResource);
		StringUtility::appendValueString(valueString, mCharPanelResourceAtlas);
		StringUtility::appendValueString(valueString, mCharPanelResourceName);
		StringUtility::appendValueString(valueString, mCharPanelResourceOffset);
	}
	virtual void update(string& updateString) const
	{
		StringUtility::appendUpdateInt(updateString, COL_ID, mID);
		StringUtility::appendUpdateString(updateString, COL_LABEL, mLabel);
		StringUtility::appendUpdateInt(updateString, COL_EQUIP_TYPE, mEquipType);
		StringUtility::appendUpdateString(updateString, COL_DESC, mDesc);
		StringUtility::appendUpdateString(updateString, COL_OCCUPATION, mOccupation);
		StringUtility::appendUpdateString(updateString, COL_RESOURCE, mResource);
		StringUtility::appendUpdateString(updateString, COL_CHAR_PANEL_RESOURCE_ATLAS, mCharPanelResourceAtlas);
		StringUtility::appendUpdateString(updateString, COL_CHAR_PANEL_RESOURCE_NAME, mCharPanelResourceName);
		StringUtility::appendUpdateString(updateString, COL_CHAR_PANEL_RESOURCE_OFFSET, mCharPanelResourceOffset);
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_ID);
		table->registeColumn(COL_LABEL);
		table->registeColumn(COL_EQUIP_TYPE);
		table->registeColumn(COL_DESC);
		table->registeColumn(COL_OCCUPATION);
		table->registeColumn(COL_RESOURCE);
		table->registeColumn(COL_CHAR_PANEL_RESOURCE_ATLAS);
		table->registeColumn(COL_CHAR_PANEL_RESOURCE_NAME);
		table->registeColumn(COL_CHAR_PANEL_RESOURCE_OFFSET);
	}
};

class SQLiteEquip : public SQLiteTable
{
public:
	SQLiteEquip(SQLite* sqlite)
		:SQLiteTable("Equip", sqlite)
	{
		EquipData::registeColumn(this);
	}
	void query(int id, EquipData& data);
	bool insert(const EquipData& data);
};

#endif