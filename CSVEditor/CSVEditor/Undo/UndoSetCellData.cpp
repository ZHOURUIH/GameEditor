#include "AllHeader.h"

void UndoSetCellData::setData(int row, int col, Vector<Vector<string>>&& data)
{
	mStartRow = row;
	mStartCol = col;
	mData = move(data);
}

void UndoSetCellData::setData(int row, int col, const string& data)
{
	mStartRow = row;
	mStartCol = col;
	mData.push_back(Vector<string>{ data });
}

void UndoSetCellData::undo()
{
	// »Ö¸´Êý¾Ý
	FOR_VECTOR(mData)
	{
		const auto& cols = mData[i];
		FOR_VECTOR_J(cols)
		{
			mMainListWindow->SetCellValue(mStartRow, mStartCol, mData.size(), mData[0].size(), mData);
		}
	}
}