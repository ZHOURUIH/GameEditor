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
	// 恢复数据,将行插入进去
	mMainListWindow->addRow(mRow, move(mData));
}