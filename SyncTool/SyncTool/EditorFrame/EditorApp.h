#ifndef _EDITOR_APP_H_
#define _EDITOR_APP_H_

#include "EditorUtility.h"

class EditorFrame;
class EditorBusyInfo;
class EditorApp : public wxApp
{
public:
	EditorApp();
	virtual ~EditorApp();
	EditorFrame* GetMainFrame() { return mMainFrame; }
	
	virtual bool OnInit();
	EditorBusyInfo* getBusyInfo(){ return mBusyInfo; }
	// 该函数只能由在EditorFrame中调用
	void destroyBusyInfo();

	DECLARE_EVENT_TABLE()
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
protected:
	// 顶层窗口会被wxWidgets自动销毁,不需要自己手动销毁
	EditorFrame* mMainFrame;
	EditorBusyInfo* mBusyInfo;
	bool hasFile = false;
};

DECLARE_APP(EditorApp)

#endif