#include "txEngineRoot.h"
#include "txPropertyManager.h"
#include "txPropertyReceiver.h"
#include "txComponentFactory.h"
#include "txComponentFactoryManager.h"

#include "EditorCoreRoot.h"
#include "ECEditor.h"
#include "ECScene.h"

#include "EditorCommandHeader.h"
#include "PropertyWindow.h"
#include "EditorFrame.h"
#include "EditorApp.h"
#include "CustomProperty.h"

enum
{
	ID_PROPERTY_GRID,
	ID_REFRESH_BUTTON,
};

BEGIN_EVENT_TABLE(PropertyWindow, EditorPanel)
EVT_PG_CHANGED(ID_PROPERTY_GRID, PropertyWindow::OnPropertyGridChange)
EVT_TOOL(ID_REFRESH_BUTTON, PropertyWindow::OnRefresh)
END_EVENT_TABLE()

PropertyWindow::PropertyWindow(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

void PropertyWindow::setup()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	mAuiToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT);
	mAuiToolBar->AddTool(ID_REFRESH_BUTTON, "刷新", wxBITMAP(IDB_REFRESH));
	mAuiToolBar->Realize();
	sizer->Add(mAuiToolBar, 0, wxALL, 5);

	mPropGridManager = new wxPropertyGridManager(this, ID_PROPERTY_GRID, wxDefaultPosition, wxSize(100, 100), wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_TOOLBAR | wxPG_DESCRIPTION);
	mPropertyGrid = mPropGridManager->GetGrid();
	mPropGridManager->SetExtraStyle(wxPG_EX_MODE_BUTTONS | wxPG_EX_MULTIPLE_SELECTION);
	mPropGridManager->SetValidationFailureBehavior(wxPG_VFB_MARK_CELL | wxPG_VFB_SHOW_MESSAGEBOX);
	mPropGridManager->GetGrid()->SetVerticalSpacing(2);
	wxPGCell cell;
	cell.SetText(wxT("Unspecified"));
	cell.SetFgCol(*wxLIGHT_GREY);
	mPropertyGrid->SetUnspecifiedValueAppearance(cell);

	sizer->Add(mPropGridManager, 1, wxEXPAND);

	mPropGridManager->RegisterAdditionalEditors();
	mPropGridManager->EnableCategories(true);
	mPropertyGrid->Show(false);

	mPropertyStandPage = mPropGridManager->AddPage(wxT("Stand"));

	this->SetSizer(sizer);
	this->Layout();
}

void PropertyWindow::showProperty(txComponent* component)
{
	txVector<txComponent*> componentList;
	if (component != NULL)
	{
		componentList.push_back(component);
	}
	showProperty(componentList);
}

void PropertyWindow::showProperty(const txVector<txComponent*>& selections)
{
	clearProperty();
	mShowEditorList = selections;
	showEditorProperty();
	mPropertyGrid->Show(true);
}

void PropertyWindow::clearProperty()
{
	mPropertyStandPage->Clear();
	mShowEditorList.clear();
}

void PropertyWindow::refreshProperty()
{
	CommandEditorFrameNotifyComponentSelectionChanged* cmd = NEW_CMD(cmd);
	mCommandSystem->pushCommand(cmd, mEditorFrame);
}

PROPERTY_TYPE PropertyWindow::getSelectPropertyType(const std::string& propertyName)
{
	if (mShowEditorList.size() > 0)
	{
		txProperty* prop = mShowEditorList[0]->getProperty(propertyName);
		if (prop != NULL)
		{
			return prop->getType();
		}
	}
	return PT_STRING;
}

void PropertyWindow::showEditorProperty()
{
	if (mShowEditorList.size() == 0)
	{
		return;
	}

	txComponent* component = mShowEditorList[0];
	// 显示第一个编辑体的名字
	const std::string& componentName = component->getName();
	mPropertyStandPage->Append(new wxStringProperty(wxT("Name"), wxT("Name"), wxString(componentName.c_str())));
	mPropertyStandPage->SetPropertyHelpString(wxT("Name"), wxT("组件名"));

	// 暂时只显示第一个编辑体的所有属性
	wxPGProperty* propCategory = mPropertyStandPage->Append(new wxPropertyCategory(componentName, wxPG_LABEL));
	// 获得该组件的属性列表
	auto& propertyList = component->getPropertyList();
	auto iterProperty = propertyList.begin();
	auto iterPropertyEnd = propertyList.end();
	FOR_STL(propertyList, ; iterProperty != iterPropertyEnd; ++iterProperty)
	{
		txProperty* prop = *iterProperty;
		const PROPERTY_TYPE& propertyType = prop->getType();
		wxPGProperty* wxProp = NULL;
		if (PT_BOOL == propertyType)
		{
			txBoolProperty* boolProperty = static_cast<txBoolProperty*>(prop);
			wxProp = appendBoolProperty(propCategory, boolProperty, boolProperty->getValue(component));
		}
		else if (PT_INT == propertyType)
		{
			txIntProperty* intProperty = static_cast<txIntProperty*>(prop);
			wxProp = appendIntProperty(propCategory, intProperty, intProperty->getValue(component));
		}
		else  if (PT_STRING == propertyType)
		{
			txStringProperty* stringProperty = static_cast<txStringProperty*>(prop);
			wxProp = appendStringProperty(propCategory, stringProperty, stringProperty->getValue(component));
		}
		else if (PT_VECTOR2 == propertyType)
		{
			txVector2Property* vector2Property = static_cast<txVector2Property*>(prop);
			wxProp = appendVectorProperty<txVector2Property, wxVector2Property, VECTOR2>(propCategory, vector2Property, vector2Property->getValue(component));
		}
		else if (PT_VECTOR3 == propertyType)
		{
			txVector3Property* vector3Property = static_cast<txVector3Property*>(prop);
			wxProp = appendVectorProperty<txVector3Property, wxVector3Property, VECTOR3>(propCategory, vector3Property, vector3Property->getValue(component));
		}
		else if (PT_VECTOR4 == propertyType)
		{
			txVector4Property* vector4Property = static_cast<txVector4Property*>(prop);
			wxProp = appendVectorProperty<txVector4Property, wxVector4Property, VECTOR4>(propCategory, vector4Property, vector4Property->getValue(component));
		}
		else if (PT_FLOAT == propertyType)
		{
			txFloatProperty* floatProperty = static_cast<txFloatProperty*>(prop);
			wxProp = appendFloatProperty(propCategory, floatProperty, floatProperty->getValue(component));
		}
		else if (PT_ENUM == propertyType)
		{
			txEnumProperty<int>* enumProperty = static_cast<txEnumProperty<int>*>(prop);
			wxProp = appendEnumProperty(propCategory, enumProperty, enumProperty->get(component));
		}
		// 根据只读属性设置是否可修改
		if (wxProp != NULL)
		{
			// 设置属性描述
			mPropertyStandPage->SetPropertyHelpString(wxProp->GetName(), prop->getPropertyDescirption());
			wxProp->Enable(!prop->isReadOnly());
		}
	}
	END_FOR_STL(propertyList);
}

wxPGProperty* PropertyWindow::appendBoolProperty(wxPGProperty* parent, txBoolProperty* property, bool value)
{
	wxString label = wxString(property->getName().c_str());
	wxString propertyName = makePropertyName(parent->GetName(), label);
	wxPGProperty* wxProp = mPropertyStandPage->AppendIn(parent, new wxBoolProperty(label, propertyName, value));
	mPropertyStandPage->SetPropertyAttribute(propertyName, wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	return wxProp;
}

wxPGProperty* PropertyWindow::appendIntProperty(wxPGProperty* parent, txIntProperty* property, int value)
{
	wxString label = wxString(property->getName().c_str());
	wxString propertyName = makePropertyName(parent->GetName(), label);
	wxPGProperty* wxProp = mPropertyStandPage->AppendIn(parent, new wxIntProperty(label, propertyName, value));
	mPropertyStandPage->SetPropertyEditor(propertyName, wxPGEditor_SpinCtrl);
	mPropertyStandPage->SetPropertyAttribute(propertyName, wxT("Step"), (long)1);
	mPropertyStandPage->SetPropertyAttribute(propertyName, wxT("MotionSpin"), true);
	return wxProp;
}

wxPGProperty* PropertyWindow::appendStringProperty(wxPGProperty* parent, txStringProperty* property, const std::string& value)
{
	wxString label = wxString(property->getName().c_str());
	wxString propertyName = makePropertyName(parent->GetName(), label);
	wxPGProperty* wxProp = NULL;
	txVector<std::string>* valueList = property->getChoices();
	if (valueList != NULL)
	{
		wxPGChoices choices;
		int cueEnum = -1;
		int listSize = valueList->size();
		FOR_STL((*valueList), int i = 0; i < listSize; ++i)
		{
			choices.Add((*valueList)[i], i);
			if (cueEnum < 0 && value == (*valueList)[i])
			{
				cueEnum = i;
			}
		}
		END_FOR_STL((*valueList));
		wxProp = mPropertyStandPage->AppendIn(parent, new wxEnumProperty(label, propertyName, choices, cueEnum));
	}
	else
	{
		wxProp = mPropertyStandPage->AppendIn(parent, new wxStringProperty(label, propertyName, wxString(value.c_str())));
	}
	return wxProp;
}

wxPGProperty* PropertyWindow::appendFloatProperty(wxPGProperty* parent, txFloatProperty* property, float value)
{
	wxString label = wxString(property->getName().c_str());
	wxString propertyName = makePropertyName(parent->GetName(), label);
	wxPGProperty* wxProp = mPropertyStandPage->AppendIn(parent, new wxFloatProperty(label, propertyName, value));
	mPropertyStandPage->SetPropertyEditor(propertyName, wxPGEditor_SpinCtrl);
	mPropertyStandPage->SetPropertyAttribute(propertyName, wxT("Step"), 0.1f);
	mPropertyStandPage->SetPropertyAttribute(propertyName, wxT("MotionSpin"), true);
	return wxProp;
}

wxPGProperty* PropertyWindow::appendEnumProperty(wxPGProperty* parent, txEnumProperty<int>* property, const std::string& value)
{
	txMap<int, std::string>& enumList = property->getEnumList();
	wxString label = wxString(property->getName().c_str());
	wxString propertyName = makePropertyName(parent->GetName(), label);
	int curEnum = property->getEnumValueByName(value);
	wxPGChoices soc;
	auto itr = enumList.begin();
	auto itrEnd = enumList.end();
	FOR_STL(enumList, ; itr != itrEnd; ++itr)
	{
		soc.Add(itr->second.c_str(), itr->first);
	}
	END_FOR_STL(enumList);

	wxPGProperty* wxProp = mPropertyStandPage->AppendIn(parent, new wxEnumProperty(label, propertyName, soc, curEnum));
	return wxProp;
}

void PropertyWindow::OnPropertyGridChange(wxPropertyGridEvent& event)
{
	wxPGProperty* pgProp = event.GetProperty();
	wxString propertyFullName = pgProp->GetName();
	wxString propertyValue = pgProp->GetValueAsString();
	std::string componentName, oriPropName;
	getPropertyName(propertyFullName.ToStdString(), componentName, oriPropName);

	PROPERTY_TYPE type = getSelectPropertyType(oriPropName);
	wxString newValue = adjustPropertyValue(type, propertyValue);

	CommandEditorSetProperty* cmdSetProperty = NEW_CMD(cmdSetProperty);
	cmdSetProperty->mComponentName = componentName;
	cmdSetProperty->mPropertyName = oriPropName;
	cmdSetProperty->mPropertyValue = newValue.ToStdString();
	cmdSetProperty->mChangeOperator = this;
	mCommandSystem->pushCommand(cmdSetProperty, mShowEditorList[0]->getOwner());
}

wxString PropertyWindow::adjustPropertyValue(PROPERTY_TYPE type, wxString& oldValue)
{
	wxString newValue;
	if (PT_DIM == type ||
		PT_POINT == type ||
		PT_VECTOR2 == type ||
		PT_VECTOR3 == type)
	{
		// 将分号替换为逗号,去掉所有空格
		int valueLen = oldValue.size();
		for (int i = 0; i < valueLen; ++i)
		{
			if (oldValue[i] != ' ')
			{
				if (oldValue[i] == ';')
				{
					newValue += ',';
				}
				else
				{
					newValue += oldValue[i];
				}
			}
		}
	}
	else
	{
		newValue = oldValue;
	}
	return newValue;
}

void PropertyWindow::OnRefresh(wxCommandEvent& event)
{
	refreshProperty();
}