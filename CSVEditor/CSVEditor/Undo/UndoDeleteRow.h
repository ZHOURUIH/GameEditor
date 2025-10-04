#pragma once

#include "Undo.h"

class GridData;
class ColumnData;
class UndoDeleteRow : public Undo
{
public:
	~UndoDeleteRow();
	void setData(int row, Vector<GridData*>&& data);
	virtual void undo();
protected:
	int mRow = -1;
	Vector<GridData*> mData;
};