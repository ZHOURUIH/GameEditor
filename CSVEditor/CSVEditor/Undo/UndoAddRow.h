#pragma once

#include "Undo.h"

class UndoAddRow : public Undo
{
public:
	void setData(int row);
	virtual void undo();
protected:
	int mRow = -1;
};