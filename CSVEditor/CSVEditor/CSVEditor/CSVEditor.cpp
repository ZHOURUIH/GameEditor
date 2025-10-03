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
	FOR_I(EditorDefine::HEADER_ROW)
	{
		const Vector<string>& line = result[i];
		// ����
		if (i == EditorDefine::ROW_TABLE_NAME)
		{
			mTableName = line[0];
		}
		// ������
		else if (i == EditorDefine::ROW_TABLE_OWNER)
		{
			mOwner = getOwner(line[0]);
		}
		// �ֶ���
		else if (i == EditorDefine::ROW_COLUMN_NAME)
		{
			FOR_VECTOR_J(line)
			{
				ColumnData* colData = new ColumnData;
				colData->mName = line[j];
				mColumnDataList.push_back(colData);
			}
		}
		// �ֶ�����
		else if (i == EditorDefine::ROW_COLUMN_TYPE)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mType = line[j];
			}
		}
		// �ֶ�����
		else if (i == EditorDefine::ROW_COLUMN_OWNER)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mOwner = getOwner(line[j]);
			}
		}
		// �ֶ�ע��
		else if (i == EditorDefine::ROW_COLUMN_COMMENT)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mComment = line[j];
			}
		}
		// ���ӱ��
		else if (i == EditorDefine::ROW_COLUMN_LINK_TABLE)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mLinkTable = line[j];
			}
		}
	}
	for (int i = EditorDefine::HEADER_ROW; i < result.size(); ++i)
	{
		const Vector<string>& line = result[i];
		Vector<GridData*> dataLine;
		FOR_VECTOR_J(line)
		{
			GridData* data = new GridData();
			data->mOriginData = line[j];
			dataLine.push_back(data);
		}
		if (dataLine.size() > 0)
		{
			mAllGrid.push_back(move(dataLine));
		}
	}
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
}

void CSVEditor::save()
{
	if (!validate())
	{
		return;
	}
	string csv;
	const int colCount = mColumnDataList.size();
	FOR_I(EditorDefine::HEADER_ROW)
	{
		// ����
		if (i == EditorDefine::ROW_TABLE_NAME)
		{
			csv += mTableName;
			FOR_I(colCount)
			{
				appendString(csv, "", i < colCount - 1);
			}
		}
		// ������
		else if (i == EditorDefine::ROW_TABLE_OWNER)
		{
			csv += getOwnerString(mOwner);
			FOR_I(colCount)
			{
				appendString(csv, "", i < colCount - 1);
			}
		}
		// �ֶ�����
		else if (i == EditorDefine::ROW_COLUMN_NAME)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, mColumnDataList[i]->mName, i < mColumnDataList.size() - 1);
			}
		}
		// �ֶ�����
		else if (i == EditorDefine::ROW_COLUMN_TYPE)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, mColumnDataList[i]->mType, i < mColumnDataList.size() - 1);
			}
		}
		// �ֶ�����
		else if (i == EditorDefine::ROW_COLUMN_OWNER)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, getOwnerString(mColumnDataList[i]->mOwner), i < mColumnDataList.size() - 1);
			}
		}
		// �ֶ�ע��
		else if (i == EditorDefine::ROW_COLUMN_COMMENT)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, mColumnDataList[i]->mComment, i < mColumnDataList.size() - 1);
			}
		}
		// �ֶ����ӱ��
		else if (i == EditorDefine::ROW_COLUMN_LINK_TABLE)
		{
			FOR_VECTOR(mColumnDataList)
			{
				appendString(csv, mColumnDataList[i]->mLinkTable, i < mColumnDataList.size() - 1);
			}
		}
	}
	// ������
	FOR_VECTOR(mAllGrid)
	{
		const auto& row = mAllGrid[i];
		FOR_VECTOR_J(row)
		{
			appendString(csv, row[j]->mOriginData, j < row.size() - 1);
		}
	}
	writeFile(mFilePath, ANSIToUTF8(csv));
}

bool CSVEditor::validate()
{
	// �������ID�Ƿ�Ψһ
	Set<int> idSet;
	for (const auto& item : mAllGrid)
	{
		if (!idSet.insert(SToI(item[0]->mOriginData)))
		{
			wxMessageBox("���ظ���ID:" + item[0]->mOriginData, "����", wxOK | wxICON_ERROR);
			return false;
		}
	}
	return true;
}

void CSVEditor::setCellData(int row, int col, const string& data)
{
	mAllGrid[row][col]->mOriginData = data;
}

void CSVEditor::setColumnName(int col, const string& name)
{
	mColumnDataList[col]->mName = name;
}

void CSVEditor::setColumnOwner(int col, const string& owner)
{
	mColumnDataList[col]->mOwner = getOwner(owner);
}

void CSVEditor::setColumnType(int col, const string& type)
{
	mColumnDataList[col]->mType = type;
}

void CSVEditor::setColumnComment(int col, const string& comment)
{
	mColumnDataList[col]->mComment = comment;
}

void CSVEditor::setColumnLinkTable(int col, const string& linkTable)
{
	mColumnDataList[col]->mLinkTable = linkTable;
}

void CSVEditor::setTableName(const string& name)
{
	mTableName = name;
}

void CSVEditor::setTableOwner(const string& owner)
{
	mOwner = getOwner(owner);
}