#ifndef _PROPERTY_WINDOW_H_
#define _PROPERTY_WINDOW_H_

#include "txBaseProperty.h"

#include "EditorPanel.h"

class txComponent;
class PropertyWindow : public EditorPanel
{
public:
	PropertyWindow(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~PropertyWindow(){}
	void setup();
	void showProperty(txComponent* component);
	void showProperty(const txVector<txComponent*>& componentList);
	void clearProperty();
	void refreshProperty();
	DECLARE_EVENT_TABLE()
	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void OnRefresh(wxCommandEvent& event);
protected:
	wxString makePropertyName(const wxString& parentName, const wxString& propertyName)
	{
		return parentName + ":" + propertyName;
	}
	void getPropertyName(const std::string& fullName, std::string& parentName, std::string& oriPropName)
	{
		int lastColonPos = fullName.find_last_of(":");
		if (lastColonPos != -1)
		{
			parentName = fullName.substr(0, lastColonPos);
			oriPropName = fullName.substr(lastColonPos + 1, fullName.length() - lastColonPos - 1);
		}
		else
		{
			parentName = fullName;
			oriPropName = EMPTY_STRING;
		}
	}
	PROPERTY_TYPE getSelectPropertyType(const std::string& propertyName);
	void showEditorProperty();
	wxString adjustPropertyValue(PROPERTY_TYPE type, wxString& oldValue);
	wxPGProperty* appendBoolProperty(wxPGProperty* parent, txBoolProperty* property, bool value);
	wxPGProperty* appendIntProperty(wxPGProperty* parent, txIntProperty* property, int value);
	wxPGProperty* appendStringProperty(wxPGProperty* parent, txStringProperty* property, const std::string& value);
	wxPGProperty* appendFloatProperty(wxPGProperty* parent, txFloatProperty* property, float value);
	wxPGProperty* appendEnumProperty(wxPGProperty* parent, txEnumProperty<int>* property, const std::string& value);
	template<typename T, typename W, typename V>
	wxPGProperty* appendVectorProperty(wxPGProperty* parent, T* property, const V& value)
	{
		wxString label = wxString(property->getName().c_str());
		wxString propertyName = makePropertyName(parent->GetName(), label);
		W* wxProp = static_cast<W*>(mPropertyStandPage->AppendIn(parent, new W(label, propertyName, value)));
		txVector<wxPGProperty*> propertyList = wxProp->getChildPropertyList();
		int childPropertyCount = propertyList.size();
		FOR_STL(propertyList, int i = 0; i < childPropertyCount; ++i)
		{
			mPropertyStandPage->SetPropertyEditor(propertyList[i], wxPGEditor_SpinCtrl);
			mPropertyStandPage->SetPropertyAttribute(propertyList[i], wxT("Step"), 0.1f);
			mPropertyStandPage->SetPropertyAttribute(propertyList[i], wxT("MotionSpin"), true);
			mPropertyStandPage->SetPropertyLabel(propertyList[i], property->mChildName[i].c_str());
			mPropertyStandPage->SetPropertyHelpString(propertyList[i], property->mChildDescribe[i].c_str());
		}
		END_FOR_STL(propertyList);
		return wxProp;
	}
protected:
	wxAuiToolBar*					mAuiToolBar;
	txVector<txComponent*>		mShowEditorList;	// 当前显示属性的编辑体
	wxPropertyGrid*					mPropertyGrid;
	wxPropertyGridPage*				mPropertyStandPage;
	wxPropertyGridManager*			mPropGridManager;
};

#endif