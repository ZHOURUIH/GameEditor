#include "AllHeader.h"

void UndoAddColumn::setData(int col)
{
	mColumn = col;
}

void UndoAddColumn::undo()
{
	// ɾ����
	mMainListWindow->deleteColumn(mColumn);
}