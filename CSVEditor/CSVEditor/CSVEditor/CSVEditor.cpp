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
		if (i == 0)
		{
			mTableName = line[0];
		}
		// ������
		else if (i == 1)
		{
			mOwner = getOwner(line[0]);
		}
		// �ֶ���
		else if (i == 2)
		{
			FOR_VECTOR_J(line)
			{
				ColumnData* colData = new ColumnData;
				colData->mName = line[j];
				mColumnDataList.push_back(colData);
			}
		}
		// �ֶ�����
		else if (i == 3)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mType = line[j];
			}
		}
		// �ֶ�����
		else if (i == 4)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mOwner = getOwner(line[j]);
			}
		}
		// �ֶ�ע��
		else if (i == 5)
		{
			FOR_VECTOR_J(line)
			{
				mColumnDataList[j]->mComment = line[j];
			}
		}
		// ���ӱ��
		else if (i == 6)
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
	string csv;
	const int colCount = mColumnDataList.size();
	// ����
	csv += mTableName;
	FOR_I(colCount)
	{
		appendString(csv, "", i < colCount - 1);
	}
	// ������
	csv += getOwnerString(mOwner);
	FOR_I(colCount)
	{
		appendString(csv, "", i < colCount - 1);
	}
	// ��ͷ
	// ����
	FOR_VECTOR(mColumnDataList)
	{
		appendString(csv, mColumnDataList[i]->mName, i < mColumnDataList.size() - 1);
	}
	// ����
	FOR_VECTOR(mColumnDataList)
	{
		appendString(csv, mColumnDataList[i]->mType, i < mColumnDataList.size() - 1);
	}
	// ����
	FOR_VECTOR(mColumnDataList)
	{
		appendString(csv, getOwnerString(mColumnDataList[i]->mOwner), i < mColumnDataList.size() - 1);
	}
	// ע��
	FOR_VECTOR(mColumnDataList)
	{
		appendString(csv, mColumnDataList[i]->mComment, i < mColumnDataList.size() - 1);
	}
	// ���ӱ��
	FOR_VECTOR(mColumnDataList)
	{
		appendString(csv, mColumnDataList[i]->mLinkTable, i < mColumnDataList.size() - 1);
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