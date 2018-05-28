#include "CustomProperty.h"
#include "CustomPropertyEditor.h"

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(txDim)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxDimProperty, wxPGProperty, txDim, const txDim&, TextCtrl)

ClearDimValueEditor* wxDimProperty::mPropertyEditor = NULL;
wxDimProperty::wxDimProperty(const wxString& label, const wxString& name, const txDim& value)
:
wxPGProperty(label, name)
{
	if (mPropertyEditor == NULL)
	{
		mPropertyEditor = static_cast<ClearDimValueEditor*>(wxPropertyGrid::RegisterEditorClass(new ClearDimValueEditor()));
	}
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxFloatProperty(wxT("Relative"), wxPG_LABEL, value.mRelative));
	AddPrivateChild(new wxIntProperty(wxT("Absolute"), wxPG_LABEL, value.mAbsolute));
	SetEditor(mPropertyEditor);
}

wxVariant wxDimProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
	txDim point;
	point << thisValue;
	switch (childIndex)
	{
	case 0: point.mRelative = childValue.GetDouble(); break;
	case 1: point.mAbsolute = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << point;
	return newVariant;
}

void wxDimProperty::RefreshChildren()
{
	if (!GetChildCount())
	{
		return;
	}
	const txDim& dim = txDimRefFromVariant(m_value);
	Item(0)->SetValue(dim.mRelative);
	Item(1)->SetValue(dim.mAbsolute);
}

void wxDimProperty::changeChildValue(txDim& oriValue, const std::string& childName, float childValue)
{
	if (childName == wxT("Relative"))
	{
		oriValue.mRelative = childValue;
	}
	else if (childName == wxT("Absolute"))
	{
		oriValue.mAbsolute = (int)childValue;
	}
}

txVector<std::pair<wxPGProperty*, std::string> > wxDimProperty::getChildPropertyList()
{
	txVector<std::pair<wxPGProperty*, std::string> > propertyList;
	propertyList.push_back(std::pair<wxPGProperty*, std::string>(Item(0), typeid(float).name()));
	propertyList.push_back(std::pair<wxPGProperty*, std::string>(Item(1), typeid(int).name()));
	return propertyList;
}

//---------------------------------------------------------------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(txPoint)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxPointProperty, wxPGProperty, txPoint, const Point&, TextCtrl)

ClearPointValueEditor* wxPointProperty::mPropertyEditor = NULL;
wxPointProperty::wxPointProperty(const wxString& label, const wxString& name, const txPoint& value)
:
wxPGProperty(label, name)
{
	if (mPropertyEditor == NULL)
	{
		mPropertyEditor = static_cast<ClearPointValueEditor*>(wxPropertyGrid::RegisterEditorClass(new ClearPointValueEditor()));
	}
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxFloatProperty(wxT("RelativeX"), wxPG_LABEL, value.x.mRelative));
	AddPrivateChild(new wxIntProperty(wxT("AbsoluteX"), wxPG_LABEL, value.x.mAbsolute));
	AddPrivateChild(new wxFloatProperty(wxT("RelativeY"), wxPG_LABEL, value.y.mRelative));
	AddPrivateChild(new wxIntProperty(wxT("AbsoluteY"), wxPG_LABEL, value.y.mAbsolute));
	SetEditor(mPropertyEditor);
}

wxVariant wxPointProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
	txPoint point;
	point << thisValue;
	switch (childIndex)
	{
	case 0: point.x.mRelative = childValue.GetDouble(); break;
	case 1: point.x.mAbsolute = childValue.GetDouble(); break;
	case 2: point.y.mRelative = childValue.GetDouble(); break;
	case 3: point.y.mAbsolute = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << point;
	return newVariant;
}

void wxPointProperty::RefreshChildren()
{
	if (!GetChildCount())
		return;
	const txPoint& point = txPointRefFromVariant(m_value);
	Item(0)->SetValue(point.x.mRelative);
	Item(1)->SetValue(point.x.mAbsolute);
	Item(2)->SetValue(point.y.mRelative);
	Item(3)->SetValue(point.y.mAbsolute);
}

void wxPointProperty::changeChildValue(txPoint& oriValue, const std::string& childName, float childValue)
{
	if (childName == wxT("RelativeX"))
	{
		oriValue.x.mRelative = childValue;
	}
	else if (childName == wxT("AbsoluteX"))
	{
		oriValue.x.mAbsolute = (int)childValue;
	}
	else if (childName == wxT("RelativeY"))
	{
		oriValue.y.mRelative = childValue;
	}
	else if (childName == wxT("AbsoluteY"))
	{
		oriValue.y.mAbsolute = (int)childValue;
	}
}

txVector<std::pair<wxPGProperty*, std::string> > wxPointProperty::getChildPropertyList()
{
	txVector<std::pair<wxPGProperty*, std::string> > propertyList;
	propertyList.push_back(std::pair<wxPGProperty*, std::string>(Item(0), typeid(float).name()));
	propertyList.push_back(std::pair<wxPGProperty*, std::string>(Item(1), typeid(int).name()));
	propertyList.push_back(std::pair<wxPGProperty*, std::string>(Item(2), typeid(float).name()));
	propertyList.push_back(std::pair<wxPGProperty*, std::string>(Item(3), typeid(int).name()));
	return propertyList;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(VECTOR2)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector2Property, wxPGProperty, VECTOR2, const VECTOR2&, TextCtrl)

ClearVector2ValueEditor* wxVector2Property::mPropertyEditor = NULL;
wxVector2Property::wxVector2Property(const wxString& label, const wxString& name, const VECTOR2& value)
:
wxPGProperty(label, name)
{
	if (mPropertyEditor == NULL)
	{
		mPropertyEditor = static_cast<ClearVector2ValueEditor*>(wxPropertyGrid::RegisterEditorClass(new ClearVector2ValueEditor()));
	}
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxFloatProperty(wxT("X"), wxPG_LABEL, value.x));
	AddPrivateChild(new wxFloatProperty(wxT("Y"), wxPG_LABEL, value.y));
	SetEditor(mPropertyEditor);
}

wxVariant wxVector2Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
	VECTOR2 point;
	point << thisValue;
	switch (childIndex)
	{
	case 0: point.x = childValue.GetDouble(); break;
	case 1: point.y = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << point;
	return newVariant;
}

void wxVector2Property::RefreshChildren()
{
	if (!GetChildCount()) 
		return;
	const VECTOR2& point = VECTOR2RefFromVariant(m_value);
	Item(0)->SetValue(point.x);
	Item(1)->SetValue(point.y);
}

void wxVector2Property::changeChildValue(VECTOR2& oriValue, const std::string& childName, float childValue)
{
	if (childName == wxT("X"))
	{
		oriValue.x = childValue;
	}
	else if (childName == wxT("Y"))
	{
		oriValue.y = childValue;
	}
}

txVector<wxPGProperty*> wxVector2Property::getChildPropertyList()
{
	txVector<wxPGProperty*> propertyList;
	propertyList.push_back(Item(0));
	propertyList.push_back(Item(1));
	return propertyList;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(VECTOR3)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector3Property, wxPGProperty, VECTOR3, const VECTOR3&, TextCtrl)

ClearVector3ValueEditor* wxVector3Property::mPropertyEditor = NULL;
wxVector3Property::wxVector3Property(const wxString& label, const wxString& name, const VECTOR3& value)
:
wxPGProperty(label, name)
{
	if (mPropertyEditor == NULL)
	{
		mPropertyEditor = static_cast<ClearVector3ValueEditor*>(wxPropertyGrid::RegisterEditorClass(new ClearVector3ValueEditor()));
	}
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxFloatProperty(wxT("X"), wxPG_LABEL, value.x));
	AddPrivateChild(new wxFloatProperty(wxT("Y"), wxPG_LABEL, value.y));
	AddPrivateChild(new wxFloatProperty(wxT("Z"), wxPG_LABEL, value.z));
	SetEditor(mPropertyEditor);
}

wxVariant wxVector3Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
	VECTOR3 point;
	point << thisValue;
	switch (childIndex)
	{
	case 0: point.x = childValue.GetDouble(); break;
	case 1: point.y = childValue.GetDouble(); break;
	case 2: point.z = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << point;
	return newVariant;
}

void wxVector3Property::RefreshChildren()
{
	if (!GetChildCount()) 
		return;
	const VECTOR3& point = VECTOR3RefFromVariant(m_value);
	Item(0)->SetValue(point.x);
	Item(1)->SetValue(point.y);
	Item(2)->SetValue(point.z);
}

void wxVector3Property::changeChildValue(VECTOR3& oriValue, const std::string& childName, float childValue)
{
	if (childName == wxT("X"))
	{
		oriValue.x = childValue;
	}
	else if (childName == wxT("Y"))
	{
		oriValue.y = childValue;
	}
	else if (childName == wxT("Y"))
	{
		oriValue.z = childValue;
	}
}

txVector<wxPGProperty*> wxVector3Property::getChildPropertyList()
{
	txVector<wxPGProperty*> propertyList;
	propertyList.push_back(Item(0));
	propertyList.push_back(Item(1));
	propertyList.push_back(Item(2));
	return propertyList;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(VECTOR4)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector4Property, wxPGProperty, VECTOR4, const VECTOR4&, TextCtrl)

ClearVector4ValueEditor* wxVector4Property::mPropertyEditor = NULL;
wxVector4Property::wxVector4Property(const wxString& label, const wxString& name, const VECTOR4& value)
:
wxPGProperty(label, name)
{
	if (mPropertyEditor == NULL)
	{
		mPropertyEditor = static_cast<ClearVector4ValueEditor*>(wxPropertyGrid::RegisterEditorClass(new ClearVector4ValueEditor()));
	}
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxFloatProperty(wxT("W"), wxPG_LABEL, value.w));
	AddPrivateChild(new wxFloatProperty(wxT("X"), wxPG_LABEL, value.x));
	AddPrivateChild(new wxFloatProperty(wxT("Y"), wxPG_LABEL, value.y));
	AddPrivateChild(new wxFloatProperty(wxT("Z"), wxPG_LABEL, value.z));
	SetEditor(mPropertyEditor);
}

wxVariant wxVector4Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
	VECTOR4 v4;
	v4 << thisValue;
	switch (childIndex)
	{
	case 0: v4.w = childValue.GetDouble(); break;
	case 1: v4.x = childValue.GetDouble(); break;
	case 2: v4.y = childValue.GetDouble(); break;
	case 3: v4.z = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << v4;
	return newVariant;
}

void wxVector4Property::RefreshChildren()
{
	if (!GetChildCount())
		return;
	const VECTOR4& v4 = VECTOR4RefFromVariant(m_value);
	Item(0)->SetValue(v4.w);
	Item(1)->SetValue(v4.x);
	Item(2)->SetValue(v4.y);
	Item(3)->SetValue(v4.z);
}

void wxVector4Property::changeChildValue(VECTOR4& oriValue, const std::string& childName, float childValue)
{
	if (childName == wxT("W"))
	{
		oriValue.w = childValue;
	}
	else if (childName == wxT("X"))
	{
		oriValue.x = childValue;
	}
	else if (childName == wxT("Y"))
	{
		oriValue.y = childValue;
	}
	else if (childName == wxT("Z"))
	{
		oriValue.z = childValue;
	}
}

txVector<wxPGProperty*> wxVector4Property::getChildPropertyList()
{
	txVector<wxPGProperty*> propertyList;
	propertyList.push_back(Item(0));
	propertyList.push_back(Item(1));
	propertyList.push_back(Item(2));
	propertyList.push_back(Item(3));
	return propertyList;
}