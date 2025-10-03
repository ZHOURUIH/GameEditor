#pragma once

#include "FrameHeader.h"

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
	void SetCellValue(int row, int col, const string& data);
	void SetCellValue(int row, int col, const wxString& data);
	DECLARE_EVENT_TABLE()
	void OnCellSelected(wxGridEvent& event);
	void OnCellChanged(wxGridEvent& event);
protected:
	bool getSelectionRect(int& topRow, int& leftCol, int& bottomRow, int& rightCol);
protected:
	wxGrid* mGrid = nullptr;
};