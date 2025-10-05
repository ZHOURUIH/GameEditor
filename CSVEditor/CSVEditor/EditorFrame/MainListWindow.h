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
	void showData(CSVEditor* table);
	void update(float elapsedTime){}
	void openFile();
	void openFile(const string& path);
	void newFile();
	void copySelection();
	void pasteSelection();
	void setCellValue(const HashMap<Vector2Int, string>& dataList, bool isHeader, bool refreshGrid);
	void deleteColumn(int col);
	void addColumn(int col, Vector<GridData*>&& dataList, ColumnData* columnData);
	void deleteRow(int dataRow);
	void addRow(int row, Vector<GridData*>&& dataList);
	void addRowToEnd(Vector<GridData*>&& dataList);
	void addRowToFirst(Vector<GridData*>&& dataList);
	void swapRow(int row0, int row1);
	void save();
	DECLARE_EVENT_TABLE()
	void OnEditViewTextChanged(wxCommandEvent& event);
	void OnCellLeftClick(wxGridEvent& event);
	void OnCellSelected(wxGridEvent& event);
	void OnCellChanged(wxGridEvent& event);
	void OnGridEditorCreated(wxGridEditorCreatedEvent& event);
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
	Vector<string> mFilterValueList;
	Vector<int> mGridCoordToDataCoordList;	// 下标是渲染表格的行下标,value是数据的行下标,下标不含表头
	int mClickedCol = -1;
	int mClickedRow = -1;
};