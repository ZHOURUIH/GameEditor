#ifndef _RENDER_CANVAS_H_
#define _RENDER_CANVAS_H_

#include "txEngineBase.h"

#include "EditorCoreBase.h"
#include "EditorUtility.h"
#include "EditorBase.h"

class EditorFrame;
class RenderCanvas : public wxWindow, public txEngineBase, public EditorCoreBase, public EditorBase
{
public:
	RenderCanvas(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~RenderCanvas(){}
	DECLARE_EVENT_TABLE()
public:
	void OnShow(wxShowEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseRightDown(wxMouseEvent& event);
	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
protected:
	wxPoint mMouseRightLastPos; // 鼠标上一帧的坐标
};

#endif