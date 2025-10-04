#include "AllHeader.h"

void UndoAddColumn::setData(int col)
{
	mColumn = col;
}

void UndoAddColumn::undo()
{
	// É¾³ýÁÐ
	mMainListWindow->deleteColumn(mColumn);
}