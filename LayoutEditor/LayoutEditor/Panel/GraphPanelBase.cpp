#include "GraphPanelBase.h"

BEGIN_EVENT_TABLE(GraphPanelBase, EditorPanel)
EVT_SIZE(GraphPanelBase::OnSize)
EVT_KEY_DOWN(GraphPanelBase::OnKeyDown)
EVT_KEY_UP(GraphPanelBase::OnKeyUp)
EVT_MOUSEWHEEL(GraphPanelBase::OnMouseWheel)
EVT_LEFT_UP(GraphPanelBase::OnMouseLeftUp)
EVT_RIGHT_UP(GraphPanelBase::OnMouseRightUp)
EVT_MOTION(GraphPanelBase::OnMouseMove)
EVT_MIDDLE_DOWN(GraphPanelBase::OnMiddleDown)
END_EVENT_TABLE()

GraphPanelBase::GraphPanelBase(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
	:
	EditorPanel(parent, position, size, style, name)
{
	setup();
}

void GraphPanelBase::OnSize(wxSizeEvent& event)
{
	mSize = GetClientSize();
}

void GraphPanelBase::refreshGraph()
{
	wxClientDC clientDC(this);
	wxMemoryDC dc;
	wxBitmap bmp(mSize);
	dc.SelectObject(bmp);
	dc.SetBackgroundMode(wxTRANSPARENT);
	dc.Clear();
	render(&dc);
	clientDC.Blit(0, 0, mSize.x, mSize.y, &dc, 0, 0, wxCOPY);
	dc.SelectObject(wxNullBitmap);
}

void GraphPanelBase::drawLine(wxDC* dc, const int& startX, const int& startY, const int& endX, const int& endY, const VECTOR3& colour)
{
	wxPen* pen = wxThePenList->FindOrCreatePen(wxColour(colour.x, colour.y, colour.z), 1, wxSOLID);
	dc->SetPen(*pen);
	dc->DrawLine(startX, transPosY(startY), endX, transPosY(endY));
	dc->SetPen(wxNullPen);
}

void GraphPanelBase::drawRect(wxDC* dc, const int& startX, const int& startY, const int& width, const int& height, const VECTOR3& colour)
{
	wxPen* pen = wxThePenList->FindOrCreatePen(wxColour(colour.x, colour.y, colour.z), 1, wxSOLID);
	dc->SetPen(*pen);
	dc->DrawRectangle(startX, transPosY(startY), width, height);
	dc->SetPen(wxNullPen);
}

void GraphPanelBase::drawSolidRect(wxDC* dc, const int& startX, const int& startY, const int& width, const int& height, const VECTOR3& colour)
{
	wxBrush* brush = wxTheBrushList->FindOrCreateBrush(wxColour(colour.x, colour.y, colour.z), wxSOLID);
	dc->SetBrush(*brush);
	dc->DrawRectangle(startX, transPosY(startY), width, height);
	dc->SetBrush(wxNullBrush);
}

void GraphPanelBase::drawText(wxDC* dc, const std::string& text, const int& posX, const int& posY, const VECTOR3& colour)
{
	wxPen* pen = wxThePenList->FindOrCreatePen(wxColour(colour.x, colour.y, colour.z), 1, wxSOLID);
	dc->SetPen(*pen);
	dc->DrawText(text, posX, transPosY(posY));
	dc->SetPen(wxNullPen);
}