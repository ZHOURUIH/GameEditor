#include "AllHeader.h"

void UndoAddRow::setData(int row)
{
	mRow = row;
}

void UndoAddRow::undo()
{
	// ɾ����
	mMainListWindow->deleteRow(mRow);
}