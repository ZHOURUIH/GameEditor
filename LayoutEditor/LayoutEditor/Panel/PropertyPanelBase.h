#ifndef _PROPERTY_PANEL_BASE_H_
#define _PROPERTY_PANEL_BASE_H_

#include "txBaseProperty.h"

#include "EditorPanel.h"

class PropertyPanelBase : public EditorPanel
{
public:
	PropertyPanelBase(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~PropertyPanelBase();
	void setup();
	virtual void clearProperty();
	virtual void onPropertyValueChanged(const std::string& propertyName, const std::string& propertyValue);
	virtual void onRefreshProperty() = 0;
	virtual void onPropertyRightClick(wxPGProperty* prop){}
	virtual void onPropertyDoubleClick(wxPGProperty* prop){}
	virtual void showReceiverProperty(txPropertyReceiver* receiver);
	virtual void showReceiverProperty(const txVector<txPropertyReceiver*>& receiverList);
	template<typename T>
	void showPropertyT(txVector<T*>& list)
	{
		txVector<txPropertyReceiver*> receiverList;
		int listSize = list.size();
		FOR_STL(list, int i = 0; i < listSize; ++i)
		{
			receiverList.push_back(list[i]);
		}
		END_FOR_STL(list);
		showReceiverProperty(receiverList);
	}

	DECLARE_EVENT_TABLE()
	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void OnRightClicked(wxPropertyGridEvent& event);
	void OnPropertyDClicked(wxPropertyGridEvent& event);
	void OnRefresh(wxCommandEvent& event);
	void OnSpinStep(wxCommandEvent& event);

protected:
	wxPGProperty* appendDimProperty(txDimProperty* property, const txDim& point);
	wxPGProperty* appendPointProperty(txPointProperty* property, const txPoint& point);
	wxPGProperty* appendBoolProperty(txBoolProperty* property, bool value);
	wxPGProperty* appendIntProperty(txIntProperty* property, int value);
	wxPGProperty* appendStringProperty(txStringProperty* property, const std::string& value);
	wxPGProperty* appendFloatProperty(txFloatProperty* property, float value);
	wxPGProperty* appendTextureProperty(txTextureProperty* property, const std::string& value);
	wxPGProperty* appendEnumProperty(txEnumProperty<int>* property, const std::string& value);
	template<typename TXProp, typename WXProp, typename VEC>
	wxPGProperty* appendVectorProperty(TXProp* property, const VEC& value)
	{
		wxString name = wxString(property->getName().c_str());
		WXProp* wxProp = static_cast<WXProp*>(mPropertyStandPage->Append(new WXProp(name, name, value)));
		txVector<wxPGProperty*> propertyList = wxProp->getChildPropertyList();
		int childPropertyCount = propertyList.size();
		FOR_STL(propertyList, int i = 0; i < childPropertyCount; ++i)
		{
			mPropertyStandPage->SetPropertyEditor(propertyList[i], wxPGEditor_SpinCtrl);
			mPropertyStandPage->SetPropertyAttribute(propertyList[i], wxT("Step"), mSpinStep);
			mPropertyStandPage->SetPropertyAttribute(propertyList[i], wxT("MotionSpin"), true);
			mPropertyStandPage->SetPropertyLabel(propertyList[i], property->mChildName[i].c_str());
			mPropertyStandPage->SetPropertyHelpString(propertyList[i], property->mChildDescribe[i].c_str());
		}
		END_FOR_STL(propertyList);
		return wxProp;
	}

	// 将从wxProperty中获得的字符串属性值转换为窗口可读的字符串属性值
	wxString adjustPropertyValue(const PROPERTY_TYPE& type, const wxString& oldValue);
	std::string changeSubProperty(const std::string& oldValue, const PROPERTY_TYPE& type, const std::string& subPropertyName, const std::string& subPropertyValue);
	// 获得多个属性集中的所有相同的属性
	void findSameProperty(txVector<txPropertyReceiver*>& componentList, txVector<txProperty*>& propertyList);
	void showProperties(txVector<txProperty*>& propertyList, txPropertyReceiver* firstReceiver);
	PROPERTY_TYPE getSelectPropertyType(const std::string& propertyName);
	virtual void setProperty(txPropertyReceiver* receiver, const std::string& propertyName, const std::string& propertyValue) = 0;
	void setReceiverProperty(const std::string& propertyName, const std::string& propertyValue, bool replaceKeyword = false, bool floatOrInt = true);
	void setSubPropertyValue(wxPGProperty* subProperty, const std::string& subPropertyValue, bool floatOrInt);
	void populateProperties(txVector<txPropertyReceiver*>& receiverList);
protected:
	wxPropertyGrid*						mPropertyGrid;
	wxPropertyGridPage*					mPropertyStandPage;
	wxPropertyGridManager*				mPropGridManager;
	wxPGProperty*						mSpecialValueProperty;
	wxPGProperty*						mRightProperty;
	wxTextCtrl*							mSpinStepEditText;
	float								mSpinStep;
	txVector<txPropertyReceiver*>		mPropertyReceiverList;
};

#endif