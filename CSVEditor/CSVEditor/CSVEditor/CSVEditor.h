#pragma once

#include "FrameHeader.h"
#include "EditorEnum.h"
#include "GridData.h"
#include "ColumnData.h"
#include "EditorCallback.h"

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
	const string& getFilePath() const { return mFilePath; }
	OWNER getTableOwner() const { return mOwner; }
	bool isOpened() const { return !mTableName.empty(); }
	void setDirty(bool dirty) { mDirty = dirty; CALL(mDirtyCallback); }
	bool isDirty() const { return mDirty; }
	void setDirtyCallback(const Action& callback) { mDirtyCallback = callback; }
	bool validate();
	void setCellDataAuto(int row, int col, const string& value);
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
	bool mDirty = false;
	Action mDirtyCallback;
};