#pragma once

#include "wxHeader.h"
#include "FrameHeader.h"

class GridData;
class ColumnData;
class MainListWindow : public wxPanel
{
public:
	MainListWindow(wxWindow* parent, long style);
	~MainListWindow();
	void init();
	void initData(CSVEditor* table);
	void update(float elapsedTime){}
	void CopySelection();
	void PasteSelection();
	DECLARE_EVENT_TABLE()
	void OnCellSelected(wxGridEvent& event);
	void OnCellChanged(wxGridEvent& event);
protected:
	wxGrid* mGrid;
};