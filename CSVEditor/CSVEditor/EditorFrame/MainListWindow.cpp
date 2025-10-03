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
	mGrid->SetCellValue(0, 0, table->getTableName());
	// ������
	mGrid->SetCellValue(1, 0, getOwnerString(table->getTableOwner()));
	// ǰ����������Ϣ
	FOR_VECTOR(colList)
	{
		// �ֶ�����
		mGrid->SetCellValue(2, i, colList[i]->mName);
		// �ֶ�����
		mGrid->SetCellValue(3, i, colList[i]->mType);
		// �ֶ�����
		mGrid->SetCellValue(4, i, getOwnerString(colList[i]->mOwner));
		// �ֶ�ע��
		mGrid->SetCellValue(5, i, colList[i]->mComment);
		// �ֶ����ӱ�
		mGrid->SetCellValue(6, i, colList[i]->mLinkTable);
	}

	FOR_VECTOR(dataList)
	{
		const auto& line = dataList[i];
		FOR_VECTOR_J(line)
		{
			mGrid->SetCellValue(i + EditorDefine::HEADER_ROW, j, line[j]->mOriginData);
		}
	}
}

void MainListWindow::update(float elapsedTime)
{
	
}