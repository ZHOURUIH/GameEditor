#ifndef _CARD_INFO_PANEL_H_
#define _CARD_INFO_PANEL_H_

#include <vector>

#include "EditorPanel.h"

class CardData;
class CardInfoPanel : public EditorPanel
{
public:
	CardInfoPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	virtual ~CardInfoPanel(){}
	void setCardInfo(CardData* cardData);
	DECLARE_EVENT_TABLE()
protected:
	void setup();
protected:
	wxStaticText* mDataLabel;
	wxStaticText* mDataValue;
	wxStaticText* mNameLabel;
	wxStaticText* mNameValue;
	wxStaticText* mIDLabel;
	wxStaticText* mIDValue;
	wxStaticText* mMachineIndexLabel;
	wxStaticText* mMachineIndexValue;
};

#endif