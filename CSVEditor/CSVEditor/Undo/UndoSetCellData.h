#pragma once

#include "Undo.h"

class UndoSetCellData : public Undo
{
public:
	void setData(int row, int col, Vector<Vector<string>>&& data);
	void setData(int row, int col, const string& data);
	virtual void undo();
protected:
	int mStartRow = 0;
	int mStartCol = 0;
	Vector<Vector<string>> mData;
};