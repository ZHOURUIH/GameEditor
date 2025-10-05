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
	void openFile();
	void openFile(const string& path);
	void newFile();
	void copySelection();
	void pasteSelection();
	void setCellValue(int topRow, int leftCol, int bottomRow, int rightCol, const Vector<Vector<string>>& rowList);
	void deleteColumn(int col);
	void addColumn(int col, Vector<GridData*>&& dataList, ColumnData* columnData);
	void deleteRow(int row);
	void addRow(int row, Vector<GridData*>&& dataList);
	void addRowToEnd(Vector<GridData*>&& dataList);
	void addRowToFirst(Vector<GridData*>&& dataList);
	void swapRow(int row0, int row1);
	void save();
	DECLARE_EVENT_TABLE()
	void OnEditViewTextChanged(wxCommandEvent& event);
	void OnCellSelected(wxGridEvent& event);
	void OnCellChanged(wxGridEvent& event);
	void OnEditorTextChanged(wxCommandEvent& event);
	void OnGridLabelRightClick(wxGridEvent& event);
	void OnDeleteColumn(wxCommandEvent& event);
	void OnAddColumn(wxCommandEvent& event);
	void OnDeleteRow(wxCommandEvent& event);
	void OnAddRow(wxCommandEvent& event);
	void OnAddRow10(wxCommandEvent& event);
	void OnMoveUp(wxCommandEvent& event);
	void OnMoveDown(wxCommandEvent& event);
protected:
	bool getSelectionRect(int& topRow, int& leftCol, int& bottomRow, int& rightCol);
protected:
	wxGrid* mGrid = nullptr;
	wxTextCtrl* mEditViewText;
	int mClickedCol = -1;
	int mClickedRow = -1;
};