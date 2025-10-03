#pragma once

#include <map>

#include "EditorHeader.h"

class GridData;
class ColumnData;
class MainListWindow : public wxPanel
{
public:
	MainListWindow(wxWindow* parent, long style);
	~MainListWindow();
	void init();
	void initData(CSVEditor* table);
	void update(float elapsedTime);
	DECLARE_EVENT_TABLE()
protected:
	wxGrid* mGrid;
};