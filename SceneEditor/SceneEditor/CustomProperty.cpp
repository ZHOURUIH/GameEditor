#include "CustomProperty.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(VECTOR2)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector2Property, wxPGProperty, VECTOR2, const VECTOR2&, TextCtrl)

wxVector2Property::wxVector2Property(const wxString& label, const wxString& name, const VECTOR2& value)
:
wxPGProperty(label, name)
{
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxFloatProperty(wxT("X"), wxPG_LABEL, value.x));
	AddPrivateChild(new wxFloatProperty(wxT("Y"), wxPG_LABEL, value.y));
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
	if (childName == "X")
	{
		oriValue.x = childValue;
	}
	else if (childName == "Y")
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

wxVector3Property::wxVector3Property(const wxString& label, const wxString& name, const VECTOR3& value)
:
wxPGProperty(label, name)
{
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxFloatProperty(wxT("X"), wxPG_LABEL, value.x));
	AddPrivateChild(new wxFloatProperty(wxT("Y"), wxPG_LABEL, value.y));
	AddPrivateChild(new wxFloatProperty(wxT("Z"), wxPG_LABEL, value.z));
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
	if (childName == "X")
	{
		oriValue.x = childValue;
	}
	else if (childName == "Y")
	{
		oriValue.y = childValue;
	}
	else if (childName == "Y")
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

wxVector4Property::wxVector4Property(const wxString& label, const wxString& name, const VECTOR4& value)
:
wxPGProperty(label, name)
{
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxFloatProperty(wxT("W"), wxPG_LABEL, value.w));
	AddPrivateChild(new wxFloatProperty(wxT("X"), wxPG_LABEL, value.x));
	AddPrivateChild(new wxFloatProperty(wxT("Y"), wxPG_LABEL, value.y));
	AddPrivateChild(new wxFloatProperty(wxT("Z"), wxPG_LABEL, value.z));
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
	if (childName == "W")
	{
		oriValue.w = childValue;
	}
	else if (childName == "X")
	{
		oriValue.x = childValue;
	}
	else if (childName == "Y")
	{
		oriValue.y = childValue;
	}
	else if (childName == "Y")
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