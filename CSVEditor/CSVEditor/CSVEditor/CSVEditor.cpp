#include "AllHeader.h"

void CSVEditor::openFile(const string& file)
{
	const string fileConetent = openTxtFile(file, true);
	if (fileConetent.empty())
	{
		return;
	}
	mFilePath = file;
	Vector<Vector<string>> result;
	parseCSV(fileConetent, result);
	FOR_I(EditorDefine::HEADER_DATA_ROW)
	{
		const Vector<string>& line = result[i];
		// 表名
		if (i == EditorDefine::ROW_TABLE_NAME)
		{
			mTableName = line[0];
		}
		// 表所属
		else if (i == EditorDefine::ROW_TABLE_OWNER)
		{
			mOwner = getOwner(line[0]);
		}
		// 字段名
		else if (i == EditorDefine::ROW_COLUMN_NAME)
		{
			FOR_VECTOR_J(line)
			{
				ColumnData* colData = new ColumnData;
				colData->mName = line[j];
				mColumnDataList.push_back(colData);
			}
		}
		// 字段类型
		else if (i == EditorDefine::ROW_COLUMN_TYPE)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mType = line[j];
			}
		}
		// 字段所属
		else if (i == EditorDefine::ROW_COLUMN_OWNER)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mOwner = getOwner(line[j]);
			}
		}
		// 字段注释
		else if (i == EditorDefine::ROW_COLUMN_COMMENT)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mComment = line[j];
			}
		}
		// 链接表格
		else if (i == EditorDefine::ROW_COLUMN_LINK_TABLE)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mLinkTable = line[j];
			}
		}
	}
	for (int i = EditorDefine::HEADER_DATA_ROW; i < result.size(); ++i)
	{
		const Vector<string>& line = result[i];
		Vector<GridData*> dataLine;
		FOR_VECTOR_J(line)
		{
			GridData* data = new GridData();
			data->mOriginData = line[j];
			dataLine.push_back(data);
		}
		if (!dataLine.isEmpty())
		{
			mAllGrid.push_back(move(dataLine));
		}
	}
	setDirty(false);
}

void CSVEditor::newFile()
{
	mTableName = "NewTable";
	mOwner = OWNER::BOTH;
	ColumnData* tempCol = new ColumnData();
	tempCol->mName = "ID";
	tempCol->mType = "int";
	tempCol->mOwner = OWNER::BOTH;
	tempCol->mComment = "唯一ID";
	mColumnDataList.push_back(tempCol);
	setDirty(true);
}

void CSVEditor::closeFile()
{
	for (auto& item : mAllGrid)
	{
		for (GridData* data : item)
		{
			delete data;
		}
	}
	mAllGrid.clear();
	for (ColumnData* data : mColumnDataList)
	{
		delete data;
	}
	mColumnDataList.clear();
	mTableName = "";
	mOwner = OWNER::NONE;
	setDirty(false);
}

bool CSVEditor::save()
{
	if (!validate())
	{
		return false;
	}
	string csv;
	const int colCount = mColumnDataList.size();
	FOR_I(EditorDefine::HEADER_DATA_ROW)
	{
		// 表名
		if (i == EditorDefine::ROW_TABLE_NAME)
		{
			csv += mTableName;
			FOR_I(colCount)
			{
				appendString(csv, "", i < colCount - 1);
			}
		}
		// 表所属
		else if (i == EditorDefine::ROW_TABLE_OWNER)
		{
			csv += getOwnerString(mOwner);
			FOR_I(colCount)
			{
				appendString(csv, "", i < colCount - 1);
			}
		}
		// 字段名字
		else if (i == EditorDefine::ROW_COLUMN_NAME)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, mColumnDataList[i]->mName, i < mColumnDataList.size() - 1);
			}
		}
		// 字段类型
		else if (i == EditorDefine::ROW_COLUMN_TYPE)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, mColumnDataList[i]->mType, i < mColumnDataList.size() - 1);
			}
		}
		// 字段所属
		else if (i == EditorDefine::ROW_COLUMN_OWNER)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, getOwnerString(mColumnDataList[i]->mOwner), i < mColumnDataList.size() - 1);
			}
		}
		// 字段注释
		else if (i == EditorDefine::ROW_COLUMN_COMMENT)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, mColumnDataList[i]->mComment, i < mColumnDataList.size() - 1);
			}
		}
		// 字段链接表格
		else if (i == EditorDefine::ROW_COLUMN_LINK_TABLE)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, mColumnDataList[i]->mLinkTable, i < mColumnDataList.size() - 1);
			}
		}
	}
	// 表数据
	FOR_VECTOR(mAllGrid)
	{
		const auto& row = mAllGrid[i];
		FOR_VECTOR_J(row)
		{
			appendString(csv, row[j]->mOriginData, j < row.size() - 1);
		}
	}
	writeFile(mFilePath, ANSIToUTF8(csv));
	setDirty(false);
	return true;
}

bool CSVEditor::validate()
{
	// 名字不能重复
	Set<string> nameSet;
	for (ColumnData* data : mColumnDataList)
	{
		if (data->mName.empty())
		{
			dialogOK("有空的列名");
			return false;
		}
		if (!nameSet.insert(data->mName))
		{
			dialogOK("有重复的字段名:" + data->mName);
			return false;
		}
		if (data->mType.empty())
		{
			dialogOK("有空的字段类型");
			return false;
		}
		if (data->mComment.empty())
		{
			dialogOK("有空的字段注释");
			return false;
		}
	}

	// ID不能重复
	Set<int> idSet;
	FOR_VECTOR(mAllGrid)
	{
		const auto& row = mAllGrid[i];
		if (row[0]->mOriginData.empty())
		{
			dialogOK("ID不能为空");
			return false;
		}
		if (!idSet.insert(SToI(row[0]->mOriginData)))
		{
			dialogOK("有重复的ID:" + row[0]->mOriginData);
			return false;
		}
	}
	return true;
}

const string& CSVEditor::getColumnName(int col)
{
	if (col < 0 || col >= mColumnDataList.size())
	{
		return FrameDefine::EMPTY;
	}
	return mColumnDataList[col]->mName;
}

string CSVEditor::getColumnOwner(int col)
{
	if (col < 0 || col >= mColumnDataList.size())
	{
		return FrameDefine::EMPTY;
	}
	return getOwnerString(mColumnDataList[col]->mOwner);
}

const string& CSVEditor::getColumnType(int col)
{
	if (col < 0 || col >= mColumnDataList.size())
	{
		return FrameDefine::EMPTY;
	}
	return mColumnDataList[col]->mType;
}

const string& CSVEditor::getColumnComment(int col)
{
	if (col < 0 || col >= mColumnDataList.size())
	{
		return FrameDefine::EMPTY;
	}
	return mColumnDataList[col]->mComment;
}

const string& CSVEditor::getColumnLinkTable(int col)
{
	if (col < 0 || col >= mColumnDataList.size())
	{
		return FrameDefine::EMPTY;
	}
	return mColumnDataList[col]->mLinkTable;
}

const string& CSVEditor::getCellData(int row, int col)
{
	return mAllGrid[row][col]->mOriginData;
}

string CSVEditor::getCellDataAuto(int row, int col)
{
	if (row == EditorDefine::ROW_TABLE_NAME)
	{
		if (col == 0)
		{
			return getTableName();
		}
		else
		{
			return FrameDefine::EMPTY;
		}
	}
	else if (row == EditorDefine::ROW_TABLE_OWNER)
	{
		if (col == 0)
		{
			return getOwnerString(getTableOwner());
		}
		else
		{
			return FrameDefine::EMPTY;
		}
	}
	else if (row == EditorDefine::ROW_COLUMN_NAME)
	{
		return getColumnName(col);
	}
	else if (row == EditorDefine::ROW_COLUMN_TYPE)
	{
		return getColumnType(col);
	}
	else if (row == EditorDefine::ROW_COLUMN_OWNER)
	{
		return getColumnOwner(col);
	}
	else if (row == EditorDefine::ROW_COLUMN_COMMENT)
	{
		return getColumnComment(col);
	}
	else if (row == EditorDefine::ROW_COLUMN_LINK_TABLE)
	{
		return getColumnLinkTable(col);
	}
	else if (row == EditorDefine::ROW_COLUMN_FILTER)
	{
		return "";
	}
	else
	{
		return getCellData(row - EditorDefine::HEADER_ROW, col);
	}
}

void CSVEditor::setCellDataAuto(int row, int col, const string& value)
{
	if (row == EditorDefine::ROW_TABLE_NAME)
	{
		if (col == 0)
		{
			setTableName(value);
		}
	}
	else if (row == EditorDefine::ROW_TABLE_OWNER)
	{
		if (col == 0)
		{
			setTableOwner(value);
		}
	}
	else if (row == EditorDefine::ROW_COLUMN_NAME)
	{
		setColumnName(col, value);
	}
	else if (row == EditorDefine::ROW_COLUMN_TYPE)
	{
		setColumnType(col, value);
	}
	else if (row == EditorDefine::ROW_COLUMN_OWNER)
	{
		setColumnOwner(col, value);
	}
	else if (row == EditorDefine::ROW_COLUMN_COMMENT)
	{
		setColumnComment(col, value);
	}
	else if (row == EditorDefine::ROW_COLUMN_LINK_TABLE)
	{
		setColumnLinkTable(col, value);
	}
	else if (row == EditorDefine::ROW_COLUMN_FILTER)
	{
		;
	}
	else
	{
		setCellData(row - EditorDefine::HEADER_ROW, col, value);
	}
}

void CSVEditor::setCellData(int row, int col, const string& data)
{
	mAllGrid[row][col]->mOriginData = data;
	setDirty(true);
}

void CSVEditor::setColumnName(int col, const string& name)
{
	mColumnDataList[col]->mName = name;
	setDirty(true);
}

void CSVEditor::setColumnOwner(int col, const string& owner)
{
	mColumnDataList[col]->mOwner = getOwner(owner);
	setDirty(true);
}

void CSVEditor::setColumnType(int col, const string& type)
{
	mColumnDataList[col]->mType = type;
	setDirty(true);
}

void CSVEditor::setColumnComment(int col, const string& comment)
{
	mColumnDataList[col]->mComment = comment;
	setDirty(true);
}

void CSVEditor::setColumnLinkTable(int col, const string& linkTable)
{
	mColumnDataList[col]->mLinkTable = linkTable;
	setDirty(true);
}

void CSVEditor::setTableName(const string& name)
{
	mTableName = name;
	setDirty(true);
}

void CSVEditor::setTableOwner(const string& owner)
{
	mOwner = getOwner(owner);
	setDirty(true);
}

void CSVEditor::deleteColumn(int col, Vector<GridData*>& outList, ColumnData*& outCol)
{
	outCol = mColumnDataList[col];
	mColumnDataList.eraseAt(col);
	for (auto& item : mAllGrid)
	{
		outList.push_back(item[col]);
		item.eraseAt(col);
	}
	setDirty(true);
}

void CSVEditor::addColumn(int col, Vector<GridData*>&& cols, ColumnData*& colData)
{
	if (colData == nullptr)
	{
		colData = new ColumnData();
	}
	mColumnDataList.insert(col, colData);
	
	if (cols.isEmpty())
	{
		FOR_VECTOR(mAllGrid)
		{
			mAllGrid[i].insert(col, new GridData());
		}
	}
	else
	{
		FOR_VECTOR(mAllGrid)
		{
			mAllGrid[i].insert(col, cols[i]);
		}
	}
	cols.clear();
	setDirty(true);
}

void CSVEditor::deleteRow(int row, Vector<GridData*>& outRows)
{
	outRows.setRange(move(mAllGrid[row]));
	mAllGrid.eraseAt(row);
	setDirty(true);
}

void CSVEditor::addRow(int row, Vector<GridData*>&& rows)
{
	if (rows.isEmpty())
	{
		Vector<GridData*> temp;
		FOR_I(mColumnDataList.size())
		{
			temp.push_back(new GridData());
		}
		mAllGrid.insert(row, move(temp));
	}
	else
	{
		mAllGrid.insert(row, move(rows));
	}
	setDirty(true);
}

void CSVEditor::swapRow(int row0, int row1)
{
	auto temp = move(mAllGrid[row0]);
	mAllGrid[row0] = move(mAllGrid[row1]);
	mAllGrid[row1] = temp;
	setDirty(true);
}