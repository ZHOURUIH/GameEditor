#include "AllHeader.h"

enum
{
	ID_SEARCH_TEXT,
};

BEGIN_EVENT_TABLE(MainListWindow, wxPanel)
EVT_GRID_SELECT_CELL(MainListWindow::OnCellSelected)
END_EVENT_TABLE()

MainListWindow::MainListWindow(wxWindow* parent, long style)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style)
{
	mMainListWindow = this;
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);

	mGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);

	// Grid
	mGrid->CreateGrid(0, 0);
	mGrid->EnableEditing(true);
	mGrid->EnableGridLines(true);
	mGrid->EnableDragGridSize(false);
	mGrid->SetMargins(0, 0);
	mGrid->SetSelectionMode(wxGrid::wxGridSelectCells);

	// Columns
	mGrid->EnableDragColMove(false);
	mGrid->EnableDragColSize(true);
	mGrid->SetColLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

	// Rows
	mGrid->EnableDragRowSize(true);
	mGrid->SetRowLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

	// Label Appearance

	// Cell Defaults
	mGrid->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_TOP);
	sizer1->Add(mGrid, 1, wxALL | wxEXPAND, 5);
	SetSizer(sizer1);
	Layout();
	Centre(wxBOTH);

	mGrid->Bind(wxEVT_GRID_CELL_CHANGED, &MainListWindow::OnCellChanged, this);
}

void MainListWindow::initData(CSVEditor* table)
{
	const Vector<Vector<GridData*>>& dataList = table->getAllGrid();
	const Vector<ColumnData*>& colList = table->getColumnDataList();
	// ���ԭ������
	if (mGrid->GetNumberRows() > 0)
	{
		mGrid->DeleteRows(0, mGrid->GetNumberRows());
	}
	if (mGrid->GetNumberCols() > 0)
	{
		mGrid->DeleteCols(0, mGrid->GetNumberCols());
	}

	// ����µ�
	mGrid->AppendRows(dataList.size() + EditorDefine::HEADER_ROW);
	mGrid->AppendCols(colList.size());
	FOR_VECTOR(colList)
	{
		mGrid->SetColLabelValue(i, colList[i]->mName);
	}
	// ����
	mGrid->SetCellValue(EditorDefine::ROW_TABLE_NAME, 0, table->getTableName());
	// ������
	mGrid->SetCellValue(EditorDefine::ROW_TABLE_OWNER, 0, getOwnerString(table->getTableOwner()));
	// ǰ����������Ϣ
	FOR_VECTOR(colList)
	{
		// �ֶ�����
		mGrid->SetCellValue(EditorDefine::ROW_COLUMN_NAME, i, colList[i]->mName);
		// �ֶ�����
		mGrid->SetCellValue(EditorDefine::ROW_COLUMN_TYPE, i, colList[i]->mType);
		// �ֶ�����
		mGrid->SetCellValue(EditorDefine::ROW_COLUMN_OWNER, i, getOwnerString(colList[i]->mOwner));
		// �ֶ�ע��
		mGrid->SetCellValue(EditorDefine::ROW_COLUMN_COMMENT, i, colList[i]->mComment);
		// �ֶ����ӱ�
		mGrid->SetCellValue(EditorDefine::ROW_COLUMN_LINK_TABLE, i, colList[i]->mLinkTable);
	}

	FOR_VECTOR(dataList)
	{
		const auto& line = dataList[i];
		FOR_VECTOR_J(line)
		{
			mGrid->SetCellValue(i + EditorDefine::HEADER_ROW, j, line[j]->mOriginData);
		}
	}

	// ��ͷ��Ҫȫ�������ó���ɫ����
	FOR_I(EditorDefine::HEADER_ROW)
	{
		FOR_J(colList.size())
		{
			mGrid->SetCellBackgroundColour(i, j, *wxCYAN);
		}
	}
}

void MainListWindow::OnCellSelected(wxGridEvent& event)
{
	int row = event.GetRow();
	int col = event.GetCol();
	LOG("select:" + IToS(row) + ", " + IToS(col));
	event.Skip();
}

void MainListWindow::OnCellChanged(wxGridEvent& event)
{
	const int row = event.GetRow();
	const int col = event.GetCol();
	LOG("cell:" + IToS(row) + ", " + IToS(col));
	UndoSetCellData* undo = new UndoSetCellData();
	undo->setData(row, col, mCSVEditor->getCellDataAuto(row, col));
	mUndoManager->addUndo(undo);
	mCSVEditor->setCellDataAuto(row, col, mGrid->GetCellValue(row, col).ToStdString());
	event.Skip();
}

void MainListWindow::CopySelection()
{
	if (!wxTheClipboard->Open())
	{
		return;
	}
	int topRow = -1;
	int leftCol = -1;
	int bottomRow = -1;
	int rightCol = -1;
	if (!getSelectionRect(topRow, leftCol, bottomRow, rightCol))
	{
		wxTheClipboard->Close();
		return;
	}

	wxString data;
	for (int r = topRow; r <= bottomRow; ++r)
	{
		for (int c = leftCol; c <= rightCol; ++c)
		{
			data += mGrid->GetCellValue(r, c);
			// ��Ԫ��ָ������Ʊ����
			if (c < rightCol)
			{
				data += "\t";
			}
		}
		// �зָ��������з���
		data += "\n";
	}
	// �Ƴ����һ�еĶ��໻�з�
	if (!data.IsEmpty())
	{
		data.RemoveLast();
	}

	wxTheClipboard->SetData(new wxTextDataObject(data));
	wxTheClipboard->Close();
}

void MainListWindow::PasteSelection()
{
	if (!wxTheClipboard->Open())
	{
		return;
	}

	if (!wxTheClipboard->IsSupported(wxDF_TEXT))
	{
		wxTheClipboard->Close();
		return;
	}

	wxTextDataObject dataObj;
	wxTheClipboard->GetData(dataObj);
	wxTheClipboard->Close();

	int topRow = -1;
	int leftCol = -1;
	int bottomRow = -1;
	int rightCol = -1;
	if (!getSelectionRect(topRow, leftCol, bottomRow, rightCol))
	{
		wxTheClipboard->Close();
		return;
	}

	wxArrayString rows = wxSplit(dataObj.GetText(), '\n');
	if (rows.size() == 0 || wxSplit(rows[0], '\t').size() == 0)
	{
		return;
	}

	// ��ӳ�������
	Vector<Vector<string>> rectData;
	FOR_VECTOR(rows)
	{
		const wxArrayString cols = wxSplit(rows[i], '\t');
		Vector<string> tempRow;
		FOR_VECTOR_J(cols)
		{
			tempRow.push_back(mCSVEditor->getCellDataAuto(topRow + i, leftCol + j));
		}
		rectData.push_back(move(tempRow));
	}
	UndoSetCellData* undo = new UndoSetCellData();
	undo->setData(topRow, leftCol, move(rectData));
	mUndoManager->addUndo(undo);

	// ���õ�Ԫ������
	FOR_VECTOR(rows)
	{
		const wxArrayString cols = wxSplit(rows[i], '\t');
		FOR_VECTOR_J(cols)
		{
			SetCellValue(topRow + i, leftCol + j, cols[j]);
		}
	}
}

void MainListWindow::SetCellValue(int row, int col, const string& data)
{
	mGrid->SetCellValue(row, col, data);
	mCSVEditor->setCellDataAuto(row, col, data);
}

void MainListWindow::SetCellValue(int row, int col, const wxString& data)
{
	mGrid->SetCellValue(row, col, data);
	mCSVEditor->setCellDataAuto(row, col, data.ToStdString());
}

bool MainListWindow::getSelectionRect(int& topRow, int& leftCol, int& bottomRow, int& rightCol)
{
	topRow = -1;
	leftCol = -1;
	bottomRow = -1;
	rightCol = -1;
	// ���Ի�ȡ������ѡ��
	wxGridCellCoordsArray topLeft = mGrid->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray bottomRight = mGrid->GetSelectionBlockBottomRight();
	if (!topLeft.IsEmpty() && !bottomRight.IsEmpty())
	{
		topRow = topLeft[0].GetRow();
		leftCol = topLeft[0].GetCol();
		bottomRow = bottomRight[0].GetRow();
		rightCol = bottomRight[0].GetCol();
	}
	// ���û�п�ѡ�������Ի�ȡ���λ����Ϊ����ѡ��
	else if (mGrid->GetGridCursorRow() >= 0 && mGrid->GetGridCursorCol() >= 0)
	{
		topRow = bottomRow = mGrid->GetGridCursorRow();
		leftCol = rightCol = mGrid->GetGridCursorCol();
	}
	// �޷���⵽ѡ�����˳�
	else
	{
		return false;
	}
	return true;
}