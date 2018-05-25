#ifndef _CUSTOM_PROPERTY_EDITOR_H_
#define _CUSTOM_PROPERTY_EDITOR_H_

#include "wxEditorHeader.h"

class ClearValueEditor : public wxPGTextCtrlEditor
{
	wxDECLARE_DYNAMIC_CLASS(ClearValueEditor);
public:
	virtual wxPGWindowList CreateControls(wxPropertyGrid* propGrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const;
};

//----------------------------------------------------------------------------------------------------------------------------------------
class ClearDimValueEditor : public ClearValueEditor
{
	wxDECLARE_DYNAMIC_CLASS(ClearDimValueEditor);
public:
	virtual bool OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const;
};

//----------------------------------------------------------------------------------------------------------------------------------------
class ClearPointValueEditor : public ClearValueEditor
{
	wxDECLARE_DYNAMIC_CLASS(ClearPointValueEditor);
public:
	virtual bool OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const;
};

//----------------------------------------------------------------------------------------------------------------------------------------
class ClearVector2ValueEditor : public ClearValueEditor
{
	wxDECLARE_DYNAMIC_CLASS(ClearVector2ValueEditor);
public:
	virtual bool OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const;
};

//----------------------------------------------------------------------------------------------------------------------------------------
class ClearVector3ValueEditor : public ClearValueEditor
{
	wxDECLARE_DYNAMIC_CLASS(ClearVector3ValueEditor);
public:
	virtual bool OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const;
};

//----------------------------------------------------------------------------------------------------------------------------------------
class ClearVector4ValueEditor : public ClearValueEditor
{
	wxDECLARE_DYNAMIC_CLASS(ClearVector4ValueEditor);
public:
	virtual bool OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const;
};

#endif