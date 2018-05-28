#ifndef _COMPONENT_RESOURCE_H_
#define _COMPONENT_RESOURCE_H_

#include "EditorPanel.h"

class ComponentResource : public EditorPanel
{
public:
	ComponentResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ComponentResource();
	void setup();
	void refreshList();

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);
	void OnDoubleClick(wxListEvent& event);
	const std::string& getComponentTypeFromLabel(const std::string& label)
	{
		auto iter = mComponentLabelTypeList.find(label);
		if (iter != mComponentLabelTypeList.end())
		{
			return iter->second;
		}
		return EMPTY_STRING;
	}
	const std::string& getComponentLabelFromType(const std::string& type)
	{
		auto iter = mComponentTypeLabelList.find(type);
		if (iter != mComponentTypeLabelList.end())
		{
			return iter->second;
		}
		return EMPTY_STRING;
	}
protected:
	wxListCtrl* mResourceList;
	wxImageList* mImageList;
	txMap<std::string, int> mComponentIconIndexList;
	static txMap<std::string, std::string> mComponentTypeLabelList;
	static txMap<std::string, std::string> mComponentLabelTypeList;
};

#endif