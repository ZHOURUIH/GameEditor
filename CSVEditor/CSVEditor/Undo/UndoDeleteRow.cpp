#include "AllHeader.h"

UndoDeleteRow::~UndoDeleteRow()
{
	for (GridData* data : mData)
	{
		delete data;
	}
}

void UndoDeleteRow::setData(int row, Vector<GridData*>&& data)
{
	mRow = row;
	mData = move(data);
}

void UndoDeleteRow::undo()
{
	// �ָ�����,���в����ȥ
	mMainListWindow->addRow(mRow, move(mData));
}