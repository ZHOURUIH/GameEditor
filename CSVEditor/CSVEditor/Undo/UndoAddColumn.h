#pragma once

#include "Undo.h"

class UndoAddColumn : public Undo
{
public:
	void setData(int col);
	virtual void undo();
protected:
	int mColumn = -1;
};