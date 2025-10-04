#include "AllHeader.h"

UndoDeleteColumn::~UndoDeleteColumn()
{
	for (GridData* data : mData)
	{
		delete data;
	}
	if (mColumnData != nullptr)
	{
		delete mColumnData;
	}
}

void UndoDeleteColumn::setData(int col, Vector<GridData*>&& data, ColumnData* columnData)
{
	mColumn = col;
	mData = move(data);
	mColumnData = columnData;
}

void UndoDeleteColumn::undo()
{
	// �ָ�����,���в����ȥ
	mMainListWindow->addColumn(mColumn, move(mData), mColumnData);
	mColumnData = nullptr;
}