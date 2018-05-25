#ifndef _COMPONENT_PROPERTY_PANEL_H_
#define _COMPONENT_PROPERTY_PANEL_H_

#include "txBaseProperty.h"

#include "PropertyPanelBase.h"

class ComponentPropertyPanel : public PropertyPanelBase
{
public:
	ComponentPropertyPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ComponentPropertyPanel(){}
	virtual void onRefreshProperty();
protected:
	virtual void populateProperties(txVector<txPropertyReceiver*>& receiverList);
	virtual void setProperty(txPropertyReceiver* receiver, const std::string& propertyName, const std::string& propertyValue);
protected:
};

#endif