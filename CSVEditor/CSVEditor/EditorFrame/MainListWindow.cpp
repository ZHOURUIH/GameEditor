#include "AllHeader.h"

enum
{
	ID_SEARCH_TEXT,
};

BEGIN_EVENT_TABLE(MainListWindow, wxPanel)
END_EVENT_TABLE()

MainListWindow::MainListWindow(wxWindow* parent, long style)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);

	mGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);

	// Grid
	mGrid->CreateGrid(1, 1);
	mGrid->EnableEditing(true);
	mGrid->EnableGridLines(true);
	mGrid->EnableDragGridSize(false);
	mGrid->SetMargins(0, 0);

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

MainListWindow::~MainListWindow()
{
	;
}

void MainListWindow::init()
{
	
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

void MainListWindow::OnCellChanged(wxGridEvent& event)
{
	const int row = event.GetRow();
	const int col = event.GetCol();
	const string value = mGrid->GetCellValue(row, col).ToStdString();
	if (row == EditorDefine::ROW_TABLE_NAME)
	{
		mCSVEditor->setTableName(value);
	}
	else if (row == EditorDefine::ROW_TABLE_OWNER)
	{
		mCSVEditor->setTableOwner(value);
	}
	else if (row == EditorDefine::ROW_COLUMN_NAME)
	{
		mCSVEditor->setColumnName(col, value);
	}
	else if (row == EditorDefine::ROW_COLUMN_TYPE)
	{
		mCSVEditor->setColumnType(col, value);
	}
	else if (row == EditorDefine::ROW_COLUMN_OWNER)
	{
		mCSVEditor->setColumnOwner(col, value);
	}
	else if (row == EditorDefine::ROW_COLUMN_COMMENT)
	{
		mCSVEditor->setColumnComment(col, value);
	}
	else if (row == EditorDefine::ROW_COLUMN_LINK_TABLE)
	{
		mCSVEditor->setColumnLinkTable(col, value);
	}
	else
	{
		mCSVEditor->setCellData(row - EditorDefine::HEADER_ROW, col, value);
	}
	event.Skip(); // ����Ĭ����Ϊ
}