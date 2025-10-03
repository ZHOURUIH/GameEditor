#pragma once

#include "EditorHeader.h"
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
protected:
	string mFilePath;
	Vector<Vector<GridData*>> mAllGrid;
	Vector<ColumnData*> mColumnDataList;
	string mTableName;
	OWNER mOwner = OWNER::NONE;
};