#pragma once

#include "Undo.h"

class GridData;
class ColumnData;
class UndoDeleteColumn : public Undo
{
public:
	~UndoDeleteColumn();
	void setData(int col, Vector<GridData*>&& data, ColumnData* columnData);
	virtual void undo();
protected:
	int mColumn = -1;
	Vector<GridData*> mData;
	ColumnData* mColumnData = nullptr;
};