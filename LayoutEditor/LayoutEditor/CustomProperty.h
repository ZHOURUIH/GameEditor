#ifndef _CUSTOM_PROPERTY_H_
#define _CUSTOM_PROPERTY_H_

#include "txEngineDefine.h"
#include "txPoint.h"

#include "EditorUtility.h"

class ClearDimValueEditor;
class ClearPointValueEditor;
class ClearVector2ValueEditor;
class ClearVector3ValueEditor;
class ClearVector4ValueEditor;

WX_PG_DECLARE_VARIANT_DATA(txDim)

class wxDimProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxDimProperty)
public:
	wxDimProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, const txDim& value = txDim());
	virtual ~wxDimProperty(){}
	virtual wxVariant ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const;
	virtual void RefreshChildren();
	static void changeChildValue(txDim& oriValue, const std::string& childName, float childValue);
	// string为int或者float,表示对应的属性的类型
	txVector<std::pair<wxPGProperty*, std::string> > getChildPropertyList();
public:
	static ClearDimValueEditor* mPropertyEditor;
};

WX_PG_DECLARE_VARIANT_DATA(txPoint)

class wxPointProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxPointProperty)
public:
	wxPointProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, const txPoint& value = txPoint());
	virtual ~wxPointProperty(){}
	virtual wxVariant ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const;
	virtual void RefreshChildren();
	static void changeChildValue(txPoint& oriValue, const std::string& childName, float childValue);
	// string为int或者float,表示对应的属性的类型
	txVector<std::pair<wxPGProperty*, std::string> > getChildPropertyList();
public:
	static ClearPointValueEditor* mPropertyEditor;
};

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
public:
	static ClearVector2ValueEditor* mPropertyEditor;
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
public:
	static ClearVector3ValueEditor* mPropertyEditor;
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
public:
	static ClearVector4ValueEditor* mPropertyEditor;
};

#endif
