#include "AllHeader.h"

void UndoAddRow::setData(int row)
{
	mRow = row;
}

void UndoAddRow::undo()
{
	// É¾³ýÐÐ
	mMainListWindow->deleteRow(mRow);
}