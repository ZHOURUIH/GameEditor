#include "AllHeader.h"

void UndoSwapRow::setData(int row0, int row1)
{
	mRow0 = row0;
	mRow1 = row1;
}

void UndoSwapRow::undo()
{
	// �ٽ���һ��
	mMainListWindow->swapRow(mRow1, mRow0);
}