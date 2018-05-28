#ifndef _PROPERTY_WINDOW_H_
#define _PROPERTY_WINDOW_H_

#include "txBaseProperty.h"

#include "PropertyPanelBase.h"

class Window;
class ExpressionEditor;
class PropertyWindow : public PropertyPanelBase
{
public:
	PropertyWindow(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~PropertyWindow();
	virtual void onRefreshProperty();
	// 当属性窗口的属性值发生改变时
	virtual void onPropertyValueChanged(const std::string& propertyName, const std::string& propertyValue);
	virtual void onPropertyRightClick(wxPGProperty* prop);
	virtual void onPropertyDoubleClick(wxPGProperty* prop);
	void setSpecialPropertyValue(const std::string& value, bool floatOrInt);

	DECLARE_EVENT_TABLE()
	void OnToAbsolute(wxCommandEvent& event);
	void OnToRelative(wxCommandEvent& event);
	void OnUseTextureSize(wxCommandEvent& event);
	void OnScaleApplyToSize(wxCommandEvent& event);
protected:
	void setWindowName(const std::string& propertyValue, bool replaceKeyword = false, bool floatOrInt = true);
	virtual void populateProperties(txVector<txPropertyReceiver*>& receiverList);
	virtual void setProperty(txPropertyReceiver* receiver, const std::string& propertyName, const std::string& propertyValue);
protected:
	ExpressionEditor*				mExpressionEditor;
	wxPGProperty*					mSpecialValueProperty;
	wxPGProperty*					mRightProperty;
};
#endif