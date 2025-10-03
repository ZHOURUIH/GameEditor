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
	// ���㴰�ڻᱻwxWidgets�Զ�����,����Ҫ�Լ��ֶ�����
	EditorFrame* mMainFrame = nullptr;
};

DECLARE_APP(EditorApp)