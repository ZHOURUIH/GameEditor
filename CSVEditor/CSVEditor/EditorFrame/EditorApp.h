#pragma once

#include "wxHeader.h"

class EditorFrame;
class EditorBusyInfo;
class EditorApp : public wxApp
{
public:
	EditorApp();
	virtual ~EditorApp();
	EditorFrame* GetMainFrame() { return mMainFrame; }
	
	virtual bool OnInit();

	DECLARE_EVENT_TABLE()
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
protected:
	// 顶层窗口会被wxWidgets自动销毁,不需要自己手动销毁
	EditorFrame* mMainFrame = nullptr;
};

DECLARE_APP(EditorApp)