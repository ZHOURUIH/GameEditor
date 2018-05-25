#ifndef _CUSTOM_PROPERTY_H_
#define _CUSTOM_PROPERTY_H_

#include "txEngineDefine.h"

#include "EditorUtility.h"

WX_PG_DECLARE_VARIANT_DATA(VECTOR2)

class wxVector2Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxVector2Property)
public:
	wxVector2Property(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, const VECTOR2& value = VECTOR2());
	virtual ~wxVector2Property() {}
	virtual wxVariant ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const;
	virtual void RefreshChildren();
	static void changeChildValue(VECTOR2& oriValue, const std::string& childName, float childValue);
	txVector<wxPGProperty*> getChildPropertyList();
};

WX_PG_DECLARE_VARIANT_DATA(VECTOR3)

class wxVector3Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxVector3Property)
public:
	wxVector3Property(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, const VECTOR3& value = VECTOR3());
	virtual ~wxVector3Property() {}
	virtual wxVariant ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const;
	virtual void RefreshChildren();
	static void changeChildValue(VECTOR3& oriValue, const std::string& childName, float childValue);
	txVector<wxPGProperty*> getChildPropertyList();
};

WX_PG_DECLARE_VARIANT_DATA(VECTOR4)

class wxVector4Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxVector4Property)
public:
	wxVector4Property(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, const VECTOR4& value = VECTOR4());
	virtual ~wxVector4Property() {}
	virtual wxVariant ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const;
	virtual void RefreshChildren();
	static void changeChildValue(VECTOR4& oriValue, const std::string& childName, float childValue);
	txVector<wxPGProperty*> getChildPropertyList();
};

#endif
