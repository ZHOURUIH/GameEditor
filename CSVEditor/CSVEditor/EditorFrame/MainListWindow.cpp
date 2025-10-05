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

	mEditViewText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 30));
	sizer1->Add(mEditViewText, 0, wxEXPAND | wxALL, 5);

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

	mEditViewText->Bind(wxEVT_TEXT, &MainListWindow::OnEditViewTextChanged, this);
	mGrid->Bind(wxEVT_GRID_CELL_CHANGED, &MainListWindow::OnCellChanged, this);
	mGrid->Bind(wxEVT_GRID_SELECT_CELL, &MainListWindow::OnCellSelected, this);
	mGrid->Bind(wxEVT_GRID_EDITOR_CREATED, &MainListWindow::OnGridEditorCreated, this);
}

void MainListWindow::initData(CSVEditor* table)
{
	const Vector<ColumnData*>& colList = table->getColumnDataList();
	if (mGrid->GetNumberCols() > 0)
	{
		mGrid->DeleteCols(0, mGrid->GetNumberCols());
	}
	if (mGrid->GetNumberRows() > 0)
	{
		mGrid->DeleteRows(0, mGrid->GetNumberRows());
	}
	mGrid->AppendCols(colList.size());
	mGrid->AppendRows(EditorDefine::HEADER_ROW);
	mGrid->SetRowLabelValue(EditorDefine::ROW_TABLE_NAME, "�����");
	mGrid->SetRowLabelValue(EditorDefine::ROW_TABLE_OWNER, "�������");
	mGrid->SetRowLabelValue(EditorDefine::ROW_COLUMN_NAME, "�ֶ���");
	mGrid->SetRowLabelValue(EditorDefine::ROW_COLUMN_TYPE, "�ֶ�����");
	mGrid->SetRowLabelValue(EditorDefine::ROW_COLUMN_OWNER, "�ֶ�����");
	mGrid->SetRowLabelValue(EditorDefine::ROW_COLUMN_COMMENT, "�ֶ�ע��");
	mGrid->SetRowLabelValue(EditorDefine::ROW_COLUMN_LINK_TABLE, "�ֶ�����");
	mGrid->SetRowLabelValue(EditorDefine::ROW_COLUMN_FILTER, "����");
	// ����
	mGrid->SetCellValue(EditorDefine::ROW_TABLE_NAME, 0, table->getTableName());
	// ������
	mGrid->SetCellValue(EditorDefine::ROW_TABLE_OWNER, 0, getOwnerString(table->getTableOwner()));
	mFilterValueList.clear();
	// ǰ����������Ϣ
	FOR_VECTOR(colList)
	{
		mGrid->SetColLabelValue(i, colList[i]->mName);
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
		mFilterValueList.push_back();
	}

	// ��ͷ��Ҫȫ�������óɻ�ɫ����
	FOR_I(EditorDefine::HEADER_ROW)
	{
		FOR_J(colList.size())
		{
			mGrid->SetCellBackgroundColour(i, j, *wxYELLOW);
		}
	}
	// �̶������ͷ�к�ID��
	mGrid->FreezeTo(EditorDefine::HEADER_ROW, 1);

	// �����������е�Ԫ������
	mGrid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &MainListWindow::OnCellLeftClick, this);
	showData(table);
}

void MainListWindow::showData(CSVEditor* table)
{
	const Vector<Vector<GridData*>>& dataList = table->getAllGrid();
	const Vector<ColumnData*>& colList = table->getColumnDataList();
	bool hasFilter = false;
	FOR_VECTOR(mFilterValueList)
	{
		if (!mFilterValueList[i].empty())
		{
			hasFilter = true;
			break;
		}
	}

	if (mGrid->GetNumberRows() > EditorDefine::HEADER_ROW)
	{
		mGrid->DeleteRows(EditorDefine::HEADER_ROW, mGrid->GetNumberRows() - EditorDefine::HEADER_ROW);
	}
	mGridCoordToDataCoordList.clear();
	if (hasFilter)
	{
		// ���ݹ������������Ҫ��ʾ��Щ��
		Vector<int> showRows;
		FOR_VECTOR(dataList)
		{
			const auto& line = dataList[i];
			FOR_VECTOR_J(line)
			{
				if (!mFilterValueList[j].empty() && findString(line[j]->mOriginData, mFilterValueList[j].c_str()))
				{
					showRows.push_back(i);
					break;
				}
			}
		}
		if (!showRows.isEmpty())
		{
			mGrid->AppendRows(showRows.size());
			FOR_VECTOR(showRows)
			{
				const auto& line = dataList[showRows[i]];
				FOR_VECTOR_J(line)
				{
					mGrid->SetCellValue(i + EditorDefine::HEADER_ROW, j, line[j]->mOriginData);
				}
				mGridCoordToDataCoordList.push_back(showRows[i]);
				mGrid->SetRowLabelValue(i + EditorDefine::HEADER_ROW, IToS(i + 1));
			}
		}
	}
	else
	{
		mGrid->AppendRows(dataList.size());
		FOR_VECTOR(dataList)
		{
			const auto& line = dataList[i];
			FOR_VECTOR_J(line)
			{
				mGrid->SetCellValue(i + EditorDefine::HEADER_ROW, j, line[j]->mOriginData);
			}
			mGridCoordToDataCoordList.push_back(i);
			mGrid->SetRowLabelValue(i + EditorDefine::HEADER_ROW, IToS(i + 1));
		}
	}
}

void MainListWindow::OnEditViewTextChanged(wxCommandEvent& event)
{
	// ���������༭���ǽ���ʱ�Ŵ���ͬ��,������ڵ�Ԫ��ͬ�����ı��༭��ʱ���µ�Ԫ���ˢ��
	if (mEditViewText->HasFocus())
	{
		const int row = mGrid->GetGridCursorRow();
		const int col = mGrid->GetGridCursorCol();
		if (row >= EditorDefine::HEADER_ROW && col != -1)
		{
			mGrid->SetCellValue(row, col, mEditViewText->GetValue());
		}
	}
	event.Skip();
}

void MainListWindow::OnCellLeftClick(wxGridEvent& event)
{
	const int row = event.GetRow();
	if (row == EditorDefine::ROW_COLUMN_FILTER)
	{
		mGrid->SetGridCursor(row, event.GetCol());
		mGrid->EnableCellEditControl();
	}
	event.Skip();
}

void MainListWindow::OnCellSelected(wxGridEvent& event)
{
	mEditViewText->ChangeValue(mGrid->GetCellValue(event.GetRow(), event.GetCol()));
	event.Skip();
}

void MainListWindow::OnCellChanged(wxGridEvent& event)
{
	const int row = event.GetRow();
	if (row != EditorDefine::ROW_COLUMN_FILTER)
	{
		const int col = event.GetCol();
		const string data = mGrid->GetCellValue(row, col).ToStdString();
		// �༭���Ǳ�ͷ��
		HashMap<Vector2Int, string> temp;
		if (row < EditorDefine::HEADER_ROW)
		{
			temp.insert(Vector2Int(row, col), data);
		}
		// �༭����������
		else
		{
			const int dataRow = mGridCoordToDataCoordList[row - EditorDefine::HEADER_ROW];
			temp.insert(Vector2Int(dataRow, col), data);
		}
		setCellValue(temp, row < EditorDefine::HEADER_ROW, false);
	}
	event.Skip();
}

void MainListWindow::OnGridEditorCreated(wxGridEditorCreatedEvent& event)
{
	if (auto* editor = dynamic_cast<wxTextCtrl*>(event.GetWindow()))
	{
		editor->Bind(wxEVT_TEXT, &MainListWindow::OnEditorTextChanged, this);
	}
	event.Skip();
}

void MainListWindow::OnEditorTextChanged(wxCommandEvent& event)
{
	auto* editor = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (editor == nullptr)
	{
		return;
	}

	if (mGrid->GetGridCursorRow() == EditorDefine::ROW_COLUMN_FILTER)
	{
		if (mFilterValueList[mGrid->GetGridCursorCol()] != editor->GetValue())
		{
			mFilterValueList[mGrid->GetGridCursorCol()] = editor->GetValue();
			showData(mCSVEditor);
		}
	}
	else
	{
		if (mGrid->GetGridCursorRow() >= 0 && mGrid->GetGridCursorCol() >= 0)
		{
			mEditViewText->ChangeValue(editor->GetValue());
		}
	}
}

void MainListWindow::openFile()
{
	wxFileDialog openFileDialog(this, _("Open CSV file"), "", "", "CSV files (*.csv)|*.csv", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	openFile(openFileDialog.GetPath().ToStdString());
}

void MainListWindow::openFile(const string& path)
{
	mCSVEditor->closeFile();
	mCSVEditor->openFile(path);
	if (!mCSVEditor->isOpened())
	{
		dialogOK("�ļ���ʧ��,�ļ����ܱ�ռ�û����ļ������޷�����");
		return;
	}
	initData(mCSVEditor);
}

void MainListWindow::newFile()
{
	mCSVEditor->newFile();
	initData(mCSVEditor);
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

	HashMap<Vector2Int, string> dataMap;
	bool isHeader = false;
	// Ϊ�˼��ݱ�ͷ�е���������,�е��±�0��6��ʾ��ͷ,�������޷�������,���ڵ���8���±��ʾ������
	// ���ֻ������1��,����ѡ���˶��,��ȫ�����Ϊ��ͬ������
	if (dataList.size() == 1 && dataList[0].size() == 1)
	{
		const string& data = dataList[0][0];
		for (int i = topRow; i <= bottomRow; ++i)
		{
			int dataRow = -1;
			if (i < EditorDefine::HEADER_DATA_ROW)
			{
				isHeader = true;
				dataRow = i;
			}
			else if (i >= EditorDefine::HEADER_ROW)
			{
				dataRow = mGridCoordToDataCoordList[i - EditorDefine::HEADER_ROW];
			}
			for (int j = leftCol; j <= rightCol; ++j)
			{
				dataMap.insert({ dataRow, j}, data);
			}
		}
	}
	else
	{
		for (int i = topRow; i <= bottomRow; ++i)
		{
			int dataRow = -1;
			if (i < EditorDefine::HEADER_DATA_ROW)
			{
				isHeader = true;
				dataRow = i;
			}
			else if (i >= EditorDefine::HEADER_ROW)
			{
				dataRow = mGridCoordToDataCoordList[i - EditorDefine::HEADER_ROW];
			}
			const auto& cols = dataList[i - topRow];
			for (int j = leftCol; j <= rightCol; ++j)
			{
				dataMap.insert({ dataRow, j }, cols[j - leftCol]);
			}
		}
	}
	setCellValue(dataMap, isHeader, true);
}

// isHeaderΪtrueʱ,dataList�е�first.x������Ⱦ�������ʾ�����±�
// isHeaderΪfalseʱ,dataList�е�first.x�������ݱ�����±�,��0��ʼ��,����Ⱦ�޹ص�
void MainListWindow::setCellValue(const HashMap<Vector2Int, string>& dataList, bool isHeader, bool refreshGrid)
{
	// ��ӳ�������
	HashMap<Vector2Int, string> temp;
	if (isHeader)
	{
		for (const auto& item : dataList)
		{
			temp.insert(item.first, mCSVEditor->getCellDataAuto(item.first.x, item.first.y));
		}
	}
	else
	{
		for (const auto& item : dataList)
		{
			temp.insert(item.first, mCSVEditor->getCellData(item.first.x, item.first.y));
		}
	}
	mUndoManager->addUndo<UndoSetCellData>()->setData(temp, isHeader);
	
	// ���õ�Ԫ������
	if (isHeader)
	{
		for (const auto& item : dataList)
		{
			mCSVEditor->setCellDataAuto(item.first.x, item.first.y, item.second);
		}
	}
	else
	{
		for (const auto& item : dataList)
		{
			mCSVEditor->setCellData(item.first.x, item.first.y, item.second);
		}
	}
	if (refreshGrid)
	{
		if (isHeader)
		{
			for (const auto& item : dataList)
			{
				const int showRow = item.first.x;
				mGrid->SetCellValue(showRow, item.first.y, item.second);
				if (showRow == EditorDefine::ROW_COLUMN_NAME)
				{
					mGrid->SetColLabelValue(showRow, item.second);
				}
			}
		}
		else
		{
			for (const auto& item : dataList)
			{
				const int showRow = mGridCoordToDataCoordList.findFirstIndex(item.first.x) + EditorDefine::HEADER_ROW;
				mGrid->SetCellValue(showRow, item.first.y, item.second);
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

	// 0��ʾѡ�е��Ǳ�ͷ��,1��ʾѡ�е���������,���������ܻ��ѡ��
	int state = -1;
	for (int i = topRow; i <= bottomRow; ++i)
	{
		if (i < EditorDefine::HEADER_DATA_ROW)
		{
			if (state != 0 && state != -1)
			{
				dialogOK("��ͷ��������������ܻ��ѡ��");
				return false;
			}
			state = 0;
		}
		else if (i == EditorDefine::ROW_COLUMN_FILTER)
		{
			dialogOK("�������޷������ƻ���ճ��");
			return false;
		}
		else if (i >= EditorDefine::HEADER_ROW)
		{
			if (state != 1 && state != -1)
			{
				dialogOK("��ͷ��������������ܻ��ѡ��");
				return false;
			}
			state = 1;
		}
	}
	return true;
}

void MainListWindow::OnGridLabelRightClick(wxGridEvent& event)
{
	mGrid->ClearSelection();
	// ����б�ǩ
	if (event.GetRow() == -1 && event.GetCol() != -1)
	{
		mClickedCol = event.GetCol();
		mGrid->SelectCol(mClickedCol, true);
		mGrid->SetGridCursor(0, mClickedCol);
		wxMenu menu;
		menu.Append(ID_DELETE_COL, "ɾ��");
		menu.Append(ID_ADD_COL, "���Ҳ����һ��");
		if (wxGrid* grid = dynamic_cast<wxGrid*>(event.GetEventObject()))
		{
			grid->PopupMenu(&menu);
		}
	}
	// ����б�ǩ
	else if (event.GetRow() >= EditorDefine::HEADER_ROW && event.GetCol() == -1)
	{
		mClickedRow = event.GetRow();
		mGrid->SelectRow(mClickedRow, true);
		mGrid->SetGridCursor(mClickedRow, 0);
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
			grid->PopupMenu(&menu);
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
	deleteRow(mGridCoordToDataCoordList[mClickedRow - EditorDefine::HEADER_ROW]);
	mGrid->ForceRefresh();
}

void MainListWindow::OnAddRow(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedRow < EditorDefine::HEADER_ROW)
	{
		return;
	}
	// ���������һ��
	addRow(mGridCoordToDataCoordList[mClickedRow - EditorDefine::HEADER_ROW], {});
	mGrid->ForceRefresh();
}

void MainListWindow::OnAddRow10(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedRow < EditorDefine::HEADER_ROW)
	{
		return;
	}
	// ���������10��
	const int dataRow = mGridCoordToDataCoordList[mClickedRow - EditorDefine::HEADER_ROW];
	FOR_I(10)
	{
		addRow(dataRow, {});
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
	const int dataRow0 = mGridCoordToDataCoordList[mClickedRow - EditorDefine::HEADER_ROW];
	swapRow(dataRow0, dataRow0 - 1);
}

void MainListWindow::OnMoveDown(wxCommandEvent& event)
{
	if (mGrid->GetNumberCols() <= 0 || mClickedRow < EditorDefine::HEADER_ROW || mClickedRow >= EditorDefine::HEADER_ROW + mCSVEditor->getRowCount() - 1)
	{
		return;
	}
	// ������һ�н���λ��
	const int dataRow0 = mGridCoordToDataCoordList[mClickedRow - EditorDefine::HEADER_ROW];
	swapRow(dataRow0, dataRow0 + 1);
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

void MainListWindow::deleteRow(int dataRow)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	if (dataRow < 0)
	{
		dialogOK("����ɾ����ͷ��");
		return;
	}
	const int showRow = mGridCoordToDataCoordList.findFirstIndex(dataRow);
	if (showRow >= 0)
	{
		mGridCoordToDataCoordList.eraseAt(dataRow);
		FOR_VECTOR(mGridCoordToDataCoordList)
		{
			if (i >= dataRow)
			{
				mGridCoordToDataCoordList[i] -= 1;
			}
		}
		mGrid->DeleteRows(showRow + EditorDefine::HEADER_ROW);
	}
	// ��Ҫ�ҵ�ɾ���ж�Ӧ��������
	Vector<GridData*> rows;
	mCSVEditor->deleteRow(dataRow, rows);
	// ��ӳ�������
	mUndoManager->addUndo<UndoDeleteRow>()->setData(dataRow, move(rows));
}

void MainListWindow::addRow(int dataRow, Vector<GridData*>&& dataList)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	const int showRow = mGridCoordToDataCoordList.findFirstIndex(dataRow);
	if (showRow >= 0)
	{
		mGridCoordToDataCoordList.insert(showRow, dataRow);
		FOR_VECTOR(mGridCoordToDataCoordList)
		{
			if (i > dataRow)
			{
				mGridCoordToDataCoordList[i] += 1;
			}
		}
		mGrid->InsertRows(showRow + EditorDefine::HEADER_ROW);
		FOR_VECTOR(dataList)
		{
			mGrid->SetCellValue(showRow + EditorDefine::HEADER_ROW, i, dataList[i]->mOriginData);
		}
	}

	// ��ӳ�������
	mUndoManager->addUndo<UndoAddRow>()->setData(dataRow);
	mCSVEditor->addRow(dataRow, move(dataList));
}

void MainListWindow::addRowToEnd(Vector<GridData*>&& dataList)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	// ��Ϊ��Ҫ������м�ʹ��ɸѡ�����Ҳ��Ҫ��ʾ����,���Բ�����addRow
	const int dataRow = mCSVEditor->getRowCount();
	const int showRow = mGrid->GetNumberRows() - EditorDefine::HEADER_ROW;
	mGridCoordToDataCoordList.insert(showRow, dataRow);
	mGrid->InsertRows(showRow + EditorDefine::HEADER_ROW);
	FOR_VECTOR(dataList)
	{
		mGrid->SetCellValue(showRow + EditorDefine::HEADER_ROW, i, dataList[i]->mOriginData);
	}

	// ��ӳ�������
	mUndoManager->addUndo<UndoAddRow>()->setData(dataRow);
	mCSVEditor->addRow(dataRow, move(dataList));
	mGrid->MakeCellVisible(showRow + EditorDefine::HEADER_ROW, 0);
}

void MainListWindow::addRowToFirst(Vector<GridData*>&& dataList)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	// ��Ϊ��Ҫ������м�ʹ��ɸѡ�����Ҳ��Ҫ��ʾ����,���Բ�����addRow
	const int dataRow = 0;
	const int showRow = 0;
	FOR_VECTOR(mGridCoordToDataCoordList)
	{
		mGridCoordToDataCoordList[i] += 1;
	}
	mGridCoordToDataCoordList.insert(showRow, dataRow);
	mGrid->InsertRows(showRow + EditorDefine::HEADER_ROW);
	FOR_VECTOR(dataList)
	{
		mGrid->SetCellValue(showRow + EditorDefine::HEADER_ROW, i, dataList[i]->mOriginData);
	}

	// ��ӳ�������
	mUndoManager->addUndo<UndoAddRow>()->setData(dataRow);
	mCSVEditor->addRow(dataRow, move(dataList));
	mGrid->MakeCellVisible(EditorDefine::HEADER_ROW, 0);
}

void MainListWindow::swapRow(int dataRow0, int dataRow1)
{
	if (!mCSVEditor->isOpened())
	{
		return;
	}
	if (dataRow0 == dataRow1)
	{
		return;
	}

	// ��ӳ�������
	mUndoManager->addUndo<UndoSwapRow>()->setData(dataRow0, dataRow1);
	mCSVEditor->swapRow(dataRow0, dataRow1);
	// ����ˢ�������е�����
	const int showRow0 = mGridCoordToDataCoordList.findFirstIndex(dataRow0);
	const int showRow1 = mGridCoordToDataCoordList.findFirstIndex(dataRow1);
	if (showRow0 >= 0)
	{
		const auto& rowData0 = mCSVEditor->getAllGrid()[dataRow0];
		FOR_VECTOR(rowData0)
		{
			mGrid->SetCellValue(showRow0 + EditorDefine::HEADER_ROW, i, rowData0[i]->mOriginData);
		}
	}
	if (showRow1 >= 0)
	{
		const auto& rowData1 = mCSVEditor->getAllGrid()[dataRow1];
		FOR_VECTOR(rowData1)
		{
			mGrid->SetCellValue(showRow1 + EditorDefine::HEADER_ROW, i, rowData1[i]->mOriginData);
		}
		mGrid->SetGridCursor(showRow1 + EditorDefine::HEADER_ROW, 0);
		mGrid->SelectRow(showRow1 + EditorDefine::HEADER_ROW);
		mGrid->MakeCellVisible(showRow1 + EditorDefine::HEADER_ROW, 0);
	}
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

	if (mCSVEditor->getFilePath().empty())
	{
		wxFileDialog saveDialog(this, "�����ļ�", "", mCSVEditor->getTableName() + ".csv", "�ı��ļ� (*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveDialog.ShowModal() == wxID_OK)
		{
			mCSVEditor->setFilePath(saveDialog.GetPath().ToStdString());
			mCSVEditor->setTableName(getFileNameNoSuffix(mCSVEditor->getFilePath(), true));
			// ����ʧ���ٰ�·�����,�´�������ѡ��
			if (!mCSVEditor->save())
			{
				mCSVEditor->setFilePath("");
			}
			// �����޸�������,��Ҫˢ����ʾ
			mGrid->SetCellValue(0, 0, mCSVEditor->getTableName());
		}
	}
	else
	{
		mCSVEditor->save();
	}
}