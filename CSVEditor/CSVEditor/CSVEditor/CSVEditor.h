#pragma once

#include "UsingSTD.h"
#include "GridData.h"
#include "ColumnData.h"

class CSVEditor
{
public:
	CSVEditor()
	{
	}
	~CSVEditor()
	{
	}
	void openFile(const string& file);
	void closeFile();
	void save();
	const Vector<Vector<GridData*>>& getAllGrid() const { return mAllGrid; }
	const Vector<ColumnData*>& getColumnDataList() const { return mColumnDataList; }
	const string& getTableName() const { return mTableName; }
	OWNER getTableOwner() const { return mOwner; }
	bool isOpened() const { return !mTableName.empty(); }
	bool validate();
	void setCellData(int row, int col, const string& data);
	void setColumnName(int col, const string& name);
	void setColumnOwner(int col, const string& owner);
	void setColumnType(int col, const string& type);
	void setColumnComment(int col, const string& comment);
	void setColumnLinkTable(int col, const string& linkTable);
	void setTableName(const string& name);
	void setTableOwner(const string& owner);
protected:
	string mFilePath;
	Vector<Vector<GridData*>> mAllGrid;
	Vector<ColumnData*> mColumnDataList;
	string mTableName;
	OWNER mOwner = OWNER::NONE;
};