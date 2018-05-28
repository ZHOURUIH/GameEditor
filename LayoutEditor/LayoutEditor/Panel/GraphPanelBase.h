#ifndef _GRAPH_PANEL_BASE_H_
#define _GRAPH_PANEL_BASE_H_

#include "EditorPanel.h"

class GraphPanelBase : public EditorPanel
{
public:
	GraphPanelBase(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~GraphPanelBase(){}
	virtual void setup(){}
	void refreshGraph();
	virtual void render(wxDC* dc){}
	virtual void notifyMouseWheel(wxMouseEvent& event){}
	virtual void notifyMouseLeftUp(wxMouseEvent& event){}
	virtual void notifyMouseRightUp(wxMouseEvent& event){}
	virtual void notifyMiddleDown(wxMouseEvent& event){}
	virtual void notifyMouseMove(wxMouseEvent& event){}
	virtual void notifyKeyDown(wxKeyEvent& key){}
	virtual void notifyKeyUp(wxKeyEvent& key){}
	DECLARE_EVENT_TABLE()
	void OnSize(wxSizeEvent& event);
	void OnMouseWheel(wxMouseEvent& event){ notifyMouseWheel(event); }
	void OnMouseLeftUp(wxMouseEvent& event){ notifyMouseLeftUp(event); }
	void OnMouseRightUp(wxMouseEvent& event){ notifyMouseRightUp(event); }
	void OnMiddleDown(wxMouseEvent& event){ notifyMiddleDown(event); }
	void OnMouseMove(wxMouseEvent& event){ notifyMouseMove(event); }
	void OnKeyDown(wxKeyEvent& event){ notifyKeyDown(event); }
	void OnKeyUp(wxKeyEvent& event){ notifyKeyUp(event); }
protected:
	void drawLine(wxDC* dc, const int& startX, const int& startY, const int& endX, const int& endY, const VECTOR3& colour);
	void drawRect(wxDC* dc, const int& startX, const int& startY, const int& width, const int& height, const VECTOR3& colour);
	void drawSolidRect(wxDC* dc, const int& startX, const int& startY, const int& width, const int& height, const VECTOR3& colour);
	void drawText(wxDC* dc, const std::string& text, const int& posX, const int& posY, const VECTOR3& colour);
	int transPosY(const int& clientPosY)
	{
		return mSize.y - clientPosY - 5;
	}
protected:
	wxSize mSize;
};

#endif