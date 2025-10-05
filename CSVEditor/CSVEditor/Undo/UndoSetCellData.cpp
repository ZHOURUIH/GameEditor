#include "AllHeader.h"

void UndoSetCellData::setData(const HashMap<Vector2Int, string>& data, bool isHeader)
{
	data.clone(mData);
	mIsHeader = isHeader;
}

void UndoSetCellData::undo()
{
	// �ָ�����
	mMainListWindow->setCellValue(mData, mIsHeader, true);
}