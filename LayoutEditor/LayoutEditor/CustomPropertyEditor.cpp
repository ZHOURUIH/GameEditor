#include "CustomPropertyEditor.h"
#include "CustomProperty.h"

IMPLEMENT_DYNAMIC_CLASS(ClearValueEditor, wxPGTextCtrlEditor)

wxPGWindowList ClearValueEditor::CreateControls(wxPropertyGrid* propGrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const
{
	// Create and populate buttons-subwindow
	wxPGMultiButton* buttons = new wxPGMultiButton(propGrid, sz);

	// Add regular buttons
	buttons->Add("0");

	// Create the 'primary' editor control (textctrl in this case)
	wxPGWindowList wndList = wxPGTextCtrlEditor::CreateControls(propGrid, property, pos, buttons->GetPrimarySize());

	// Finally, move buttons-subwindow to correct position and make sure
	// returned wxPGWindowList contains our custom button list.
	buttons->Finalize(propGrid, pos);

	wndList.SetSecondary(buttons);
	return wndList;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(ClearDimValueEditor, wxPGTextCtrlEditor)
bool ClearDimValueEditor::OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const
{
	if (event.GetEventType() == wxEVT_BUTTON)
	{
		wxPGMultiButton* buttons = (wxPGMultiButton*)propGrid->GetEditorControlSecondary();
		if (event.GetId() == buttons->GetButtonId(0))
		{
			propGrid->ChangePropertyValue(property, WXVARIANT(txDim()));
			return false;
		}
	}
	return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(ClearPointValueEditor, wxPGTextCtrlEditor)
bool ClearPointValueEditor::OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const
{
	if (event.GetEventType() == wxEVT_BUTTON)
	{
		wxPGMultiButton* buttons = (wxPGMultiButton*)propGrid->GetEditorControlSecondary();
		if (event.GetId() == buttons->GetButtonId(0))
		{
			propGrid->ChangePropertyValue(property, WXVARIANT(txPoint()));
			return false;
		}
	}
	return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(ClearVector2ValueEditor, wxPGTextCtrlEditor)
bool ClearVector2ValueEditor::OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const
{
	if (event.GetEventType() == wxEVT_BUTTON)
	{
		wxPGMultiButton* buttons = (wxPGMultiButton*)propGrid->GetEditorControlSecondary();
		if (event.GetId() == buttons->GetButtonId(0))
		{
			propGrid->ChangePropertyValue(property, WXVARIANT(VECTOR2()));
			return false;
		}
	}
	return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(ClearVector3ValueEditor, wxPGTextCtrlEditor)
bool ClearVector3ValueEditor::OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const
{
	if (event.GetEventType() == wxEVT_BUTTON)
	{
		wxPGMultiButton* buttons = (wxPGMultiButton*)propGrid->GetEditorControlSecondary();
		if (event.GetId() == buttons->GetButtonId(0))
		{
			propGrid->ChangePropertyValue(property, WXVARIANT(VECTOR3()));
			return false;
		}
	}
	return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(ClearVector4ValueEditor, wxPGTextCtrlEditor)
bool ClearVector4ValueEditor::OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const
{
	if (event.GetEventType() == wxEVT_BUTTON)
	{
		wxPGMultiButton* buttons = (wxPGMultiButton*)propGrid->GetEditorControlSecondary();
		if (event.GetId() == buttons->GetButtonId(0))
		{
			propGrid->ChangePropertyValue(property, WXVARIANT(VECTOR4()));
			return false;
		}
	}
	return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
}