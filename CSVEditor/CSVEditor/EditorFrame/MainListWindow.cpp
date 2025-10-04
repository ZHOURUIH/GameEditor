#include "AllHeader.h"

enum
{
	ID_DELETE_COL,
	ID_DELETE_ROW,
	ID_ADD_COL,
	ID_ADD_ROW,
	ID_ADD_ROW_10,
	ID_MOVE_UP,
	ID_MOVE_DOWN,
};

BEGIN_EVENT_TABLE(MainListWindow, wxPanel)
EVT_GRID_SELECT_CELL(MainListWindow::OnCellSelected)
EVT_GRID_LABEL_RIGHT_CLICK(MainListWindow::OnGridLabelRightClick)
EVT_MENU(ID_DELETE_COL, MainListWindow::OnDeleteColumn)
EVT_MENU(ID_DELETE_ROW, MainListWindow::OnDeleteRow)
EVT_MENU(ID_ADD_COL, MainListWindow::OnAddColumn)
EVT_MENU(ID_ADD_ROW, MainListWindow::OnAddRow)
EVT_MENU(ID_ADD_ROW_10, MainListWindow::OnAddRow10)
EVT_MENU(ID_MOVE_UP, MainListWindow::OnMoveUp)
EVT_MENU(ID_MOVE_DOWN, MainListWindow::OnMoveDown)
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
	mUndoManager->addUndo<UndoSetCellData>()->setData(row, col, mCSVEditor->getCellDataAuto(row, col));
	mCSVEditor->setCellDataAuto(row, col, mGrid->GetCellValue(row, col).ToStdString());
	if (row == EditorDefine::ROW_COLUMN_NAME)
	{
		mGrid->SetColLabelValue(col, mGrid->GetCellValue(row, col));
	}
	event.Skip();
}

void MainListWindow::copySelection()
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

void MainListWindow::pasteSelection()
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
	if (rows.IsEmpty() || wxSplit(rows[0], '\t').IsEmpty())
	{
		return;
	}
	Vector<Vector<string>> dataList;
	for (const wxString& str : rows)
	{
		dataList.push_back(split(str.char_str(), '\t'));
	}
	setCellValue(topRow, leftCol, bottomRow, rightCol, dataList);
}

void MainListWindow::setCellValue(int topRow, int leftCol, int bottomRow, int rightCol, const Vector<Vector<string>>& dataList)
{
	// ���õ�Ԫ������
	// ���ֻ������1��,����ѡ���˶��,��ȫ�����Ϊ��ͬ������
	if (dataList.size() == 1 && dataList[0].size() == 1)
	{
		// ��ӳ�������
		Vector<Vector<string>> rectData;
		for (int i = topRow; i <= bottomRow; ++i)
		{
			Vector<string> tempRow;
			for (int j = leftCol; j <= rightCol; ++j)
			{
				tempRow.push_back(mCSVEditor->getCellDataAuto(i, j));
			}
			rectData.push_back(move(tempRow));
		}
		mUndoManager->addUndo<UndoSetCellData>()->setData(topRow, leftCol, move(rectData));

		const string& data = dataList[0][0];
		for (int i = topRow; i <= bottomRow; ++i)
		{
			for (int j = leftCol; j <= rightCol; ++j)
			{
				mGrid->SetCellValue(i, j, data);
				mCSVEditor->setCellDataAuto(i, j, data);
				if (i == EditorDefine::ROW_COLUMN_NAME)
				{
					mGrid->SetColLabelValue(j, data);
				}
			}
		}
	}
	else
	{
		// ��ӳ�������
		Vector<Vector<string>> rectData;
		FOR_VECTOR(dataList)
		{
			Vector<string> tempRow;
			FOR_VECTOR_J(dataList[i])
			{
				tempRow.push_back(mCSVEditor->getCellDataAuto(topRow + i, leftCol + j));
			}
			rectData.push_back(move(tempRow));
		}
		mUndoManager->addUndo<UndoSetCellData>()->setData(topRow, leftCol, move(rectData));
		FOR_VECTOR(dataList)
		{
			const auto& cols = dataList[i];
			FOR_VECTOR_J(cols)
			{
				mGrid->SetCellValue(topRow + i, leftCol + j, cols[j]);
				mCSVEditor->setCellDataAuto(topRow + i, leftCol + j, cols[j]);
				if (topRow + i == EditorDefine::ROW_COLUMN_NAME)
				{
					mGrid->SetColLabelValue(leftCol + j, cols[j]);
				}
			}
		}
	}
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

void MainListWindow::OnGridLabelRightClick(wxGridEvent& event)
{
	// ����б�ǩ
	if (event.GetRow() == -1 && event.GetCol() != -1)
	{
		mClickedCol = event.GetCol();
		wxMenu menu;
		menu.Append(ID_DELETE_COL, "ɾ��");
		menu.Append(ID_ADD_COL, "���Ҳ����һ��");
		if (wxGrid* grid = dynamic_cast<wxGrid*>(event.GetEventObject()))
		{
			grid->PopupMenu(&menu, event.GetPosition());
		}
	}
	// ����б�ǩ
	else if (event.GetRow() >= EditorDefine::HEADER_ROW && event.GetCol() == -1)
	{
		mClickedRow = event.GetRow();
		wxMenu menu;
		menu.Append(ID_DELETE_ROW, "ɾ��");
		menu.Append(ID_ADD_ROW, "���������һ��");
		menu.Append(ID_ADD_ROW_10, "���������10��");
		if (event.GetRow() != EditorDefine::HEADER_ROW)
		{
			menu.Append(ID_MOVE_UP, "����һ��");
		}
		if (event.GetRow() != EditorDefine::HEADER_ROW + mCSVEditor->getRowCount() - 1)
		{
			menu.Append(ID_MOVE_DOWN, "����һ��");
		}
		if (wxGrid* grid = dynamic_cast<wxGrid*>(event.GetEventObject()))
		{
			grid->PopupMenu(&menu, event.GetPosition());
		}
	}
	event.Skip();
}

void MainListWindow::OnDeleteColumn(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedCol < 0)
	{
		return;
	}
	deleteColumn(mClickedCol);
	mGrid->ForceRefresh();
}

void MainListWindow::OnAddColumn(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedCol < 0)
	{
		return;
	}
	// ���Ҳ����һ��
	addColumn(mClickedCol + 1, {}, nullptr);
	mGrid->ForceRefresh();
}

void MainListWindow::OnDeleteRow(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedRow < EditorDefine::HEADER_ROW)
	{
		return;
	}
	deleteRow(mClickedRow);
	mGrid->ForceRefresh();
}

void MainListWindow::OnAddRow(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedRow < EditorDefine::HEADER_ROW)
	{
		return;
	}
	// ���������һ��
	addRow(mClickedRow, {});
	mGrid->ForceRefresh();
}

void MainListWindow::OnAddRow10(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedRow < EditorDefine::HEADER_ROW)
	{
		return;
	}
	// ���������10��
	FOR_I(10)
	{
		addRow(mClickedRow, {});
	}
	mGrid->ForceRefresh();
}

void MainListWindow::OnMoveUp(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedRow <= EditorDefine::HEADER_ROW)
	{
		return;
	}
	// ������һ�н���λ��
	swapRow(mClickedRow, mClickedRow - 1);
}

void MainListWindow::OnMoveDown(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedRow < EditorDefine::HEADER_ROW || mClickedRow >= EditorDefine::HEADER_ROW + mCSVEditor->getRowCount() - 1)
	{
		return;
	}
	// ������һ�н���λ��
	swapRow(mClickedRow, mClickedRow + 1);
}

void MainListWindow::deleteColumn(int col)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	if (col == 0)
	{
		dialogOK("����ɾ��ID��");
		return;
	}
	Vector<GridData*> cols;
	ColumnData* colData;
	mCSVEditor->deleteColumn(col, cols, colData);
	// ��ӳ�������
	mUndoManager->addUndo<UndoDeleteColumn>()->setData(col, move(cols), colData);
	mGrid->DeleteCols(col);
}

void MainListWindow::addColumn(int col, Vector<GridData*>&& dataList, ColumnData* columnData)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	// ��ӳ�������
	mUndoManager->addUndo<UndoAddColumn>()->setData(col);
	FOR_VECTOR(dataList)
	{
		mGrid->SetCellValue(i + EditorDefine::HEADER_ROW, col, dataList[i]->mOriginData);
	}
	mCSVEditor->addColumn(col, move(dataList), columnData);

	// ˢ�½���
	mGrid->InsertCols(col);
	mGrid->SetColLabelValue(col, columnData->mName);
	mGrid->SetCellValue(EditorDefine::ROW_COLUMN_NAME, col, columnData->mName);
	mGrid->SetCellValue(EditorDefine::ROW_COLUMN_TYPE, col, columnData->mType);
	mGrid->SetCellValue(EditorDefine::ROW_COLUMN_OWNER, col, getOwnerString(columnData->mOwner));
	mGrid->SetCellValue(EditorDefine::ROW_COLUMN_COMMENT, col, columnData->mComment);
	mGrid->SetCellValue(EditorDefine::ROW_COLUMN_LINK_TABLE, col, columnData->mLinkTable);
	FOR_I(EditorDefine::HEADER_ROW)
	{
		mGrid->SetCellBackgroundColour(i, col, *wxCYAN);
	}
}

void MainListWindow::deleteRow(int row)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	if (row < EditorDefine::HEADER_ROW)
	{
		dialogOK("����ɾ����ͷ��");
		return;
	}
	Vector<GridData*> rows;
	mCSVEditor->deleteRow(row - EditorDefine::HEADER_ROW, rows);
	// ��ӳ�������
	mUndoManager->addUndo<UndoDeleteRow>()->setData(row, move(rows));
	mGrid->DeleteRows(row);
}

void MainListWindow::addRow(int row, Vector<GridData*>&& dataList)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	// ��ӳ�������
	mUndoManager->addUndo<UndoAddRow>()->setData(row);
	mGrid->InsertRows(row);
	FOR_VECTOR(dataList)
	{
		mGrid->SetCellValue(row, i, dataList[i]->mOriginData);
	}
	mCSVEditor->addRow(row - EditorDefine::HEADER_ROW, move(dataList));
}

void MainListWindow::addRowToEnd(Vector<GridData*>&& dataList)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	addRow(mCSVEditor->getRowCount() + EditorDefine::HEADER_ROW, move(dataList));
	mGrid->MakeCellVisible(mCSVEditor->getRowCount() + EditorDefine::HEADER_ROW - 1, 0);
}

void MainListWindow::addRowToFirst(Vector<GridData*>&& dataList)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	addRow(EditorDefine::HEADER_ROW, move(dataList));
	mGrid->MakeCellVisible(EditorDefine::HEADER_ROW, 0);
}

void MainListWindow::swapRow(int row0, int row1)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}

	// ��ӳ�������
	mUndoManager->addUndo<UndoSwapRow>()->setData(row0, row1);
	mCSVEditor->swapRow(row0 - EditorDefine::HEADER_ROW, row1 - EditorDefine::HEADER_ROW);
	// ����ˢ�������е�����
	const auto& rowData0 = mCSVEditor->getAllGrid()[row0 - EditorDefine::HEADER_ROW];
	const auto& rowData1 = mCSVEditor->getAllGrid()[row1 - EditorDefine::HEADER_ROW];
	FOR_VECTOR(rowData0)
	{
		mGrid->SetCellValue(row0, i, rowData0[i]->mOriginData);
	}
	FOR_VECTOR(rowData1)
	{
		mGrid->SetCellValue(row1, i, rowData1[i]->mOriginData);
	}
	mGrid->SetGridCursor(row1, 0);
	mGrid->SelectRow(row1);
	mGrid->MakeCellVisible(row1, 0);
	mGrid->ForceRefresh();
}

void MainListWindow::save()
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	if (mGrid->IsCellEditControlEnabled())
	{
		// ǿ���ύ�༭���е�ֵ������ģ��
		mGrid->SaveEditControlValue();
		// �رձ༭���������༭״̬��
		mGrid->DisableCellEditControl();
	}
	mCSVEditor->save();
}