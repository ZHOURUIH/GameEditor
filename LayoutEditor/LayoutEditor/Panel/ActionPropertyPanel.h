#ifndef _ACTION_PROPERTY_PANEL_H_
#define _ACTION_PROPERTY_PANEL_H_

#include "PropertyPanelBase.h"

class ActionWindow;

class ActionPropertyPanel : public PropertyPanelBase
{
public:
	ActionPropertyPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ActionPropertyPanel(){}
	virtual void onRefreshProperty();
protected:
	void populateProperties(txVector<txPropertyReceiver*>& receiverList);
	virtual void setProperty(txPropertyReceiver* receiver, const std::string& propertyName, const std::string& propertyValue);
protected:
};

#endif