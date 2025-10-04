#pragma once

#include "FrameHeader.h"

class GridData;
class ColumnData;
class MainListWindow : public wxPanel
{
public:
	MainListWindow(wxWindow* parent, long style);
	~MainListWindow() = default;
	void init() {}
	void initData(CSVEditor* table);
	void update(float elapsedTime){}
	void CopySelection();
	void PasteSelection();
	void SetCellValue(int topRow, int leftCol, int bottomRow, int rightCol, const Vector<Vector<string>>& rowList);
	void deleteColumn(int col);
	void addColumn(int col, Vector<GridData*>&& dataList, ColumnData* columnData);
	void deleteRow(int row);
	void addRow(int row, Vector<GridData*>&& dataList);
	DECLARE_EVENT_TABLE()
	void OnCellSelected(wxGridEvent& event);
	void OnCellChanged(wxGridEvent& event);
	void OnGridLabelRightClick(wxGridEvent& event);
	void OnDeleteColumn(wxCommandEvent& event);
	void OnDeleteRow(wxCommandEvent& event);
protected:
	bool getSelectionRect(int& topRow, int& leftCol, int& bottomRow, int& rightCol);
protected:
	wxGrid* mGrid = nullptr;
	int mClickedCol = -1;
	int mClickedRow = -1;
};