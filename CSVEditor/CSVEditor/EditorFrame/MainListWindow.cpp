#include "AllHeader.h"

enum
{
	ID_DELETE_COL,
	ID_DELETE_ROW,
};

BEGIN_EVENT_TABLE(MainListWindow, wxPanel)
EVT_GRID_SELECT_CELL(MainListWindow::OnCellSelected)
EVT_GRID_LABEL_RIGHT_CLICK(MainListWindow::OnGridLabelRightClick)
EVT_MENU(ID_DELETE_COL, MainListWindow::OnDeleteColumn)
EVT_MENU(ID_DELETE_ROW, MainListWindow::OnDeleteRow)
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
	// 清空原有行列
	if (mGrid->GetNumberRows() > 0)
	{
		mGrid->DeleteRows(0, mGrid->GetNumberRows());
	}
	if (mGrid->GetNumberCols() > 0)
	{
		mGrid->DeleteCols(0, mGrid->GetNumberCols());
	}

	// 添加新的
	mGrid->AppendRows(dataList.size() + EditorDefine::HEADER_ROW);
	mGrid->AppendCols(colList.size());
	FOR_VECTOR(colList)
	{
		mGrid->SetColLabelValue(i, colList[i]->mName);
	}
	// 表名
	mGrid->SetCellValue(EditorDefine::ROW_TABLE_NAME, 0, table->getTableName());
	// 表所属
	mGrid->SetCellValue(EditorDefine::ROW_TABLE_OWNER, 0, getOwnerString(table->getTableOwner()));
	// 前几行是列信息
	FOR_VECTOR(colList)
	{
		// 字段名字
		mGrid->SetCellValue(EditorDefine::ROW_COLUMN_NAME, i, colList[i]->mName);
		// 字段类型
		mGrid->SetCellValue(EditorDefine::ROW_COLUMN_TYPE, i, colList[i]->mType);
		// 字段所属
		mGrid->SetCellValue(EditorDefine::ROW_COLUMN_OWNER, i, getOwnerString(colList[i]->mOwner));
		// 字段注释
		mGrid->SetCellValue(EditorDefine::ROW_COLUMN_COMMENT, i, colList[i]->mComment);
		// 字段链接表
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

	// 表头需要全部都设置成蓝色背景
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
			// 单元格分隔符（制表符）
			if (c < rightCol)
			{
				data += "\t";
			}
		}
		// 行分隔符（换行符）
		data += "\n";
	}
	// 移除最后一行的多余换行符
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
	if (rows.IsEmpty() || wxSplit(rows[0], '\t').IsEmpty())
	{
		return;
	}
	Vector<Vector<string>> dataList;
	for (const wxString& str : rows)
	{
		dataList.push_back(split(str.char_str(), '\t'));
	}
	SetCellValue(topRow, leftCol, bottomRow, rightCol, dataList);
}

void MainListWindow::SetCellValue(int topRow, int leftCol, int bottomRow, int rightCol, const Vector<Vector<string>>& dataList)
{
	// 设置单元格数据
	// 如果只复制了1格,但是选中了多格,则全部填充为相同的数据
	if (dataList.size() == 1 && dataList[0].size() == 1)
	{
		// 添加撤销操作
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
			}
		}
	}
	else
	{
		// 添加撤销操作
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
	// 尝试获取连续块选区
	wxGridCellCoordsArray topLeft = mGrid->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray bottomRight = mGrid->GetSelectionBlockBottomRight();
	if (!topLeft.IsEmpty() && !bottomRight.IsEmpty())
	{
		topRow = topLeft[0].GetRow();
		leftCol = topLeft[0].GetCol();
		bottomRow = bottomRight[0].GetRow();
		rightCol = bottomRight[0].GetCol();
	}
	// 如果没有块选区，尝试获取光标位置作为单格选区
	else if (mGrid->GetGridCursorRow() >= 0 && mGrid->GetGridCursorCol() >= 0)
	{
		topRow = bottomRow = mGrid->GetGridCursorRow();
		leftCol = rightCol = mGrid->GetGridCursorCol();
	}
	// 无法检测到选区则退出
	else
	{
		return false;
	}
	return true;
}

void MainListWindow::OnGridLabelRightClick(wxGridEvent& event)
{
	// 判断是否为列标签点击
	if (event.GetRow() == -1 && event.GetCol() != -1)
	{
		mClickedCol = event.GetCol();
		wxMenu menu;
		menu.Append(ID_DELETE_COL, "删除");
		if (wxGrid* grid = dynamic_cast<wxGrid*>(event.GetEventObject()))
		{
			grid->PopupMenu(&menu, event.GetPosition());
		}
	}
	else if (event.GetRow() != -1 && event.GetCol() == -1)
	{
		mClickedRow = event.GetRow();
		wxMenu menu;
		menu.Append(ID_DELETE_ROW, "删除");
		if (wxGrid* grid = dynamic_cast<wxGrid*>(event.GetEventObject()))
		{
			grid->PopupMenu(&menu, event.GetPosition());
		}
	}
	event.Skip();
}

void MainListWindow::OnDeleteColumn(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() > 0 && mClickedCol != -1)
	{
		deleteColumn(mClickedCol);
		mGrid->ForceRefresh();
	}
}

void MainListWindow::OnDeleteRow(wxCommandEvent& event)
{
	if (mGrid->GetNumberRows() > 0 && mClickedRow != -1)
	{
		deleteRow(mClickedRow);
		mGrid->ForceRefresh();
	}
}

void MainListWindow::deleteColumn(int col)
{
	if (col == 0)
	{
		dialogOK("不能删除ID列");
		return;
	}
	Vector<GridData*> cols;
	ColumnData* colData;
	mCSVEditor->deleteColumn(col, cols, colData);
	// 添加撤销操作
	mUndoManager->addUndo<UndoDeleteColumn>()->setData(col, move(cols), colData);
	mGrid->DeleteCols(col);
}

void MainListWindow::addColumn(int col, Vector<GridData*>&& dataList, ColumnData* columnData)
{
	// 添加撤销操作
	mUndoManager->addUndo<UndoAddColumn>()->setData(col);
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
	FOR_VECTOR(dataList)
	{
		mGrid->SetCellValue(i + EditorDefine::HEADER_ROW, col, dataList[i]->mOriginData);
	}
	mCSVEditor->addColumn(col, move(dataList), columnData);
}

void MainListWindow::deleteRow(int row)
{
	if (row < EditorDefine::HEADER_ROW)
	{
		dialogOK("不能删除表头行");
		return;
	}
	Vector<GridData*> rows;
	mCSVEditor->deleteRow(row - EditorDefine::HEADER_ROW, rows);
	// 添加撤销操作
	mUndoManager->addUndo<UndoDeleteRow>()->setData(row, move(rows));
	mGrid->DeleteRows(row);
}

void MainListWindow::addRow(int row, Vector<GridData*>&& dataList)
{
	// 添加撤销操作
	mUndoManager->addUndo<UndoAddRow>()->setData(row);
	mGrid->InsertRows(row);
	FOR_VECTOR(dataList)
	{
		mGrid->SetCellValue(row, i, dataList[i]->mOriginData);
	}
	mCSVEditor->addRow(row - EditorDefine::HEADER_ROW, move(dataList));
}