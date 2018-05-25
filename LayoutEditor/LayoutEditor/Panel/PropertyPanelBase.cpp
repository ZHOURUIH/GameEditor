#include "txPropertyReceiver.h"

#include "PropertyPanelBase.h"
#include "CustomProperty.h"

enum
{
	PGID = 1,
	ID_REFRESH_BUTTON,
	ID_SPIN_STEP_TEXT,
};

BEGIN_EVENT_TABLE(PropertyPanelBase, EditorPanel)
EVT_PG_CHANGED(PGID, PropertyPanelBase::OnPropertyGridChange)
EVT_PG_RIGHT_CLICK(PGID, PropertyPanelBase::OnRightClicked)
EVT_PG_DOUBLE_CLICK(PGID, PropertyPanelBase::OnPropertyDClicked)
EVT_BUTTON(ID_REFRESH_BUTTON, PropertyPanelBase::OnRefresh)
EVT_TEXT(ID_SPIN_STEP_TEXT, PropertyPanelBase::OnSpinStep)
END_EVENT_TABLE()

PropertyPanelBase::PropertyPanelBase(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mPropertyGrid(NULL),
mPropGridManager(NULL),
mPropertyStandPage(NULL),
mSpecialValueProperty(NULL),
mSpinStep(0.1f)
{
	setup();
}

PropertyPanelBase::~PropertyPanelBase()
{
	;
}

void PropertyPanelBase::setup()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);

	wxBitmapButton* refreshButton = new wxBitmapButton(this, ID_REFRESH_BUTTON, wxBITMAP(IDB_REFRESH));
	sizer1->Add(refreshButton, 0, wxALL, 5);

	wxStaticText* spinStepLabel = new wxStaticText(this, wxID_ANY, wxT("spin step : "));
	sizer1->Add(spinStepLabel, 0, wxALL, 5);

	mSpinStepEditText = new wxTextCtrl(this, ID_SPIN_STEP_TEXT, txStringUtility::floatToString(mSpinStep, 2));
	sizer1->Add(mSpinStepEditText, 0, wxALL, 5);

	sizer->Add(sizer1, 0, wxEXPAND);

	mPropGridManager = new wxPropertyGridManager(this, PGID, wxDefaultPosition, wxSize(100, 100), wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_TOOLBAR | wxPG_DESCRIPTION);

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

void PropertyPanelBase::clearProperty()
{
	mPropertyStandPage->Clear();
	mPropertyReceiverList.clear();
}

void PropertyPanelBase::onPropertyValueChanged(const std::string& propertyName, const std::string& propertyValue)
{
	if (mPropertyReceiverList.size() == 0)
	{
		return;
	}
	PROPERTY_TYPE gwpt = getSelectPropertyType(propertyName);
	wxString newValue = adjustPropertyValue(gwpt, propertyValue);
	setReceiverProperty(propertyName, newValue.ToStdString());
}

void PropertyPanelBase::showReceiverProperty(txPropertyReceiver* receiver)
{
	txVector<txPropertyReceiver*> receiverList;
	if (receiver != NULL)
	{
		receiverList.push_back(receiver);
	}
	showReceiverProperty(receiverList);
}

void PropertyPanelBase::showReceiverProperty(const txVector<txPropertyReceiver*>& receiverList)
{
	clearProperty();
	mPropertyReceiverList = receiverList;
	populateProperties(mPropertyReceiverList);
	mPropertyGrid->Show(true);
}

void PropertyPanelBase::populateProperties(txVector<txPropertyReceiver*>& receiverList)
{
	int receiverCount = receiverList.size();
	if (receiverCount == 0)
	{
		return;
	}
	// 显示出所有窗口都具有的属性,但是属性值是显示的第一个窗口的
	txVector<txProperty*> propertyList;
	findSameProperty(receiverList, propertyList);
	showProperties(propertyList, receiverList[0]);
}

void PropertyPanelBase::OnRefresh(wxCommandEvent& event)
{
	onRefreshProperty();
}

void PropertyPanelBase::OnSpinStep(wxCommandEvent& event)
{
	mSpinStep = txStringUtility::stringToFloat(mSpinStepEditText->GetValue().ToStdString());
	onRefreshProperty();
}

void PropertyPanelBase::OnPropertyGridChange(wxPropertyGridEvent& event)
{
	wxPGProperty* property = event.GetProperty();
	wxString value = property->GetValueAsString();
	if (!value.IsNull())
	{
		onPropertyValueChanged(property->GetName().ToStdString(), value.ToStdString());
	}
}

void PropertyPanelBase::OnRightClicked(wxPropertyGridEvent& event)
{
	onPropertyRightClick(event.GetProperty());
}

void PropertyPanelBase::OnPropertyDClicked(wxPropertyGridEvent& event)
{
	onPropertyDoubleClick(event.GetProperty());
}

wxPGProperty* PropertyPanelBase::appendDimProperty(txDimProperty* property, const txDim& dim)
{
	wxString name = wxString(property->getName().c_str());
	wxDimProperty* wxProp = static_cast<wxDimProperty*>(mPropertyStandPage->Append(new wxDimProperty(name, name, dim)));
	txVector<std::pair<wxPGProperty*, std::string> > propertyList = wxProp->getChildPropertyList();
	int childPropertyCount = propertyList.size();
	FOR_STL(propertyList, int i = 0; i < childPropertyCount; ++i)
	{
		mPropertyStandPage->SetPropertyEditor(propertyList[i].first, wxPGEditor_SpinCtrl);
		if (propertyList[i].second == typeid(int).name())
		{
			mPropertyStandPage->SetPropertyAttribute(propertyList[i].first, wxT("Step"), 1);
		}
		else if (propertyList[i].second == typeid(float).name())
		{
			mPropertyStandPage->SetPropertyAttribute(propertyList[i].first, wxT("Step"), mSpinStep);
		}
		mPropertyStandPage->SetPropertyAttribute(propertyList[i].first, wxT("MotionSpin"), true);
		mPropertyStandPage->SetPropertyLabel(propertyList[i].first, property->mChildName[i]);
		mPropertyStandPage->SetPropertyHelpString(propertyList[i].first, property->mChildDescribe[i]);
	}
	END_FOR_STL(propertyList);
	return wxProp;
}

wxPGProperty* PropertyPanelBase::appendPointProperty(txPointProperty* property, const txPoint& point)
{
	wxString name = wxString(property->getName().c_str());
	wxPointProperty* wxProp = static_cast<wxPointProperty*>(mPropertyStandPage->Append(new wxPointProperty(name, name, point)));
	txVector<std::pair<wxPGProperty*, std::string> > propertyList = wxProp->getChildPropertyList();
	int childPropertyCount = propertyList.size();
	FOR_STL(propertyList, int i = 0; i < childPropertyCount; ++i)
	{
		mPropertyStandPage->SetPropertyEditor(propertyList[i].first, wxPGEditor_SpinCtrl);
		if (propertyList[i].second == typeid(int).name())
		{
			mPropertyStandPage->SetPropertyAttribute(propertyList[i].first, wxT("Step"), 1);
		}
		else if (propertyList[i].second == typeid(float).name())
		{
			mPropertyStandPage->SetPropertyAttribute(propertyList[i].first, wxT("Step"), mSpinStep);
		}
		mPropertyStandPage->SetPropertyAttribute(propertyList[i].first, wxT("MotionSpin"), true);
		mPropertyStandPage->SetPropertyLabel(propertyList[i].first, property->mChildName[i]);
		mPropertyStandPage->SetPropertyHelpString(propertyList[i].first, property->mChildDescribe[i]);
	}
	END_FOR_STL(propertyList);
	return wxProp;
}

wxPGProperty* PropertyPanelBase::appendBoolProperty(txBoolProperty* property, bool value)
{
	wxString name = wxString(property->getName().c_str());
	wxBoolProperty* boolProperty = new wxBoolProperty(name, name, value);
	wxPGProperty* wxProp = mPropertyStandPage->Append(boolProperty);
	mPropertyStandPage->SetPropertyAttribute(wxProp, wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	return wxProp;
}

wxPGProperty* PropertyPanelBase::appendIntProperty(txIntProperty* property, int value)
{
	wxString name = wxString(property->getName().c_str());
	wxPGProperty* wxProp = mPropertyStandPage->Append(new wxIntProperty(name, name, value));
	mPropertyStandPage->SetPropertyEditor(wxProp, wxPGEditor_SpinCtrl);
	mPropertyStandPage->SetPropertyAttribute(wxProp, wxT("Step"), 1);
	mPropertyStandPage->SetPropertyAttribute(wxProp, wxT("MotionSpin"), true);
	return wxProp;
}

wxPGProperty* PropertyPanelBase::appendStringProperty(txStringProperty* property, const std::string& value)
{
	wxString name = wxString(property->getName().c_str());
	wxPGProperty* wxProp = NULL;
	txVector<std::string>* valueList = property->getChoices();
	if (valueList != NULL)
	{
		wxPGChoices choices;
		int curChoice = -1;
		int listSize = valueList->size();
		FOR_STL((*valueList), int i = 0; i < listSize; ++i)
		{
			choices.Add((*valueList)[i]);
			if (curChoice < 0 && (*valueList)[i] == value)
			{
				curChoice = i;
			}
		}
		END_FOR_STL((*valueList));
		wxProp = mPropertyStandPage->Append(new wxEnumProperty(name, name, choices, curChoice));
	}
	else
	{
		wxProp = mPropertyStandPage->Append(new wxStringProperty(name, name, wxString(value.c_str())));
	}
	return wxProp;
}

wxPGProperty* PropertyPanelBase::appendFloatProperty(txFloatProperty* property, float value)
{
	wxString name = wxString(property->getName().c_str());
	wxPGProperty* wxProp = mPropertyStandPage->Append(new wxFloatProperty(name, name, value));
	mPropertyStandPage->SetPropertyEditor(wxProp, wxPGEditor_SpinCtrl);
	mPropertyStandPage->SetPropertyAttribute(wxProp, wxT("Step"), mSpinStep);
	mPropertyStandPage->SetPropertyAttribute(wxProp, wxT("MotionSpin"), true);
	return wxProp;
}

wxPGProperty* PropertyPanelBase::appendTextureProperty(txTextureProperty* property, const std::string& value)
{
	wxPGChoices choices;
	int curChoice = -1;
	txVector<std::string>* textureList = property->getChoices();
	if (textureList != NULL)
	{
		int textureCount = textureList->size();
		FOR_STL((*textureList), int i = 0; i < textureCount; ++i)
		{
			choices.Add(wxString((*textureList)[i].c_str()), i);
			if (curChoice < 0 && (*textureList)[i] == value)
			{
				curChoice = i;
			}
		}
		END_FOR_STL((*textureList));
	}
	wxPGProperty* wxProp = mPropertyStandPage->Append(new wxEnumProperty(property->getName(), wxPG_LABEL, choices, curChoice));
	return wxProp;
}

wxPGProperty* PropertyPanelBase::appendEnumProperty(txEnumProperty<int>* property, const std::string& value)
{
	auto& enumList = property->getEnumList();
	wxString name = wxString(property->getName().c_str());
	int curEnum = property->getEnumValueByName(value);
	wxPGChoices soc;
	auto itr = enumList.begin();
	auto itrEnd = enumList.end();
	FOR_STL(enumList, ; itr != itrEnd; ++itr)
	{
		soc.Add(itr->second.c_str(), itr->first);
	}
	END_FOR_STL(enumList);
	wxPGProperty* wxProp = mPropertyStandPage->Append(new wxEnumProperty(name, wxPG_LABEL, soc, curEnum));
	return wxProp;
}

wxString PropertyPanelBase::adjustPropertyValue(const PROPERTY_TYPE& type, const wxString& oldValue)
{
	wxString newValue;
	if (PT_DIM == type ||
		PT_POINT == type ||
		PT_VECTOR2 == type ||
		PT_VECTOR3 == type)
	{
		// 将分号替换为逗号,去掉所有空格
		int valueLen = oldValue.size();
		for(int i = 0; i < valueLen; ++i)
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

std::string PropertyPanelBase::changeSubProperty(const std::string& oldValue, const PROPERTY_TYPE& type, const std::string& subPropertyName, const std::string& subPropertyValue)
{
	std::string fullValueString;
	if (type == PT_DIM)
	{
		txDim dim = txStringUtility::stringToDim(oldValue);
		wxDimProperty::changeChildValue(dim, subPropertyName, txStringUtility::stringToFloat(subPropertyValue));
		fullValueString = txStringUtility::dimToString(dim);
	}
	else if (type == PT_POINT)
	{
		txPoint point = txStringUtility::stringToPoint(oldValue);
		wxPointProperty::changeChildValue(point, subPropertyName, txStringUtility::stringToFloat(subPropertyValue));
		fullValueString = txStringUtility::pointToString(point);
	}
	else if (type == PT_VECTOR2)
	{
		VECTOR2 vec2Value = txStringUtility::stringToVector2(oldValue);
		wxVector2Property::changeChildValue(vec2Value, subPropertyName, txStringUtility::stringToFloat(subPropertyValue));
		fullValueString = txStringUtility::vector2ToString(vec2Value);
	}
	else if (type == PT_VECTOR3)
	{
		VECTOR3 vec3Value = txStringUtility::stringToVector3(oldValue);
		wxVector3Property::changeChildValue(vec3Value, subPropertyName, txStringUtility::stringToFloat(subPropertyValue));
		fullValueString = txStringUtility::vector3ToString(vec3Value);
	}
	return fullValueString;
}

void PropertyPanelBase::findSameProperty(txVector<txPropertyReceiver*>& receiverList, txVector<txProperty*>& propertyList)
{
	if (receiverList.size() == 0)
	{
		return;
	}
	// 要求相同类型的属性的描述内容和只读属性也必须相同
	// 首先把第一个窗口的所有属性放入列表中
	txPropertyReceiver* firstReceiver = receiverList[0];
	propertyList = firstReceiver->getPropertyList();
	int windowCount = receiverList.size();
	FOR_STL(receiverList, int i = 1; i < windowCount; ++i)
	{
		auto& winProperties = receiverList[i]->getPropertyMap();
		auto iterPro = propertyList.begin();
		FOR_STL(propertyList, ; iterPro != propertyList.end();)
		{
			// 如果在winProperties中找不到该类型的属性,则将属性从propertyList中删除
			auto iterWinPro = winProperties.find((*iterPro)->getName());
			if (iterWinPro == winProperties.end())
			{
				iterPro = propertyList.erase(iterPro, false);
			}
			else
			{
				++iterPro;
			}
		}
		END_FOR_STL(propertyList);
	}
	END_FOR_STL(receiverList);
}

void PropertyPanelBase::showProperties(txVector<txProperty*>& propertyList, txPropertyReceiver* firstReceiver)
{
	auto itr = propertyList.begin();
	auto itrEnd = propertyList.end();
	FOR_STL(propertyList, ; itr != itrEnd; ++itr)
	{
		txProperty* property = *itr;
		const PROPERTY_TYPE& propertyType = property->getType();
		wxPGProperty* wxProp = NULL;
		if (PT_DIM == propertyType)
		{
			txDimProperty* pointProperty = static_cast<txDimProperty*>(property);
			wxProp = appendDimProperty(pointProperty, pointProperty->getValue(firstReceiver));
		}
		else if (PT_POINT == propertyType)
		{
			txPointProperty* pointProperty = static_cast<txPointProperty*>(property);
			wxProp = appendPointProperty(pointProperty, pointProperty->getValue(firstReceiver));
		}
		else if (PT_BOOL == propertyType)
		{
			txBoolProperty* boolProperty = static_cast<txBoolProperty*>(property);
			wxProp = appendBoolProperty(boolProperty, boolProperty->getValue(firstReceiver));
		}
		else if (PT_INT == propertyType)
		{
			txIntProperty* intProperty = static_cast<txIntProperty*>(property);
			wxProp = appendIntProperty(intProperty, intProperty->getValue(firstReceiver));
		}
		else  if (PT_STRING == propertyType)
		{
			txStringProperty* stringProperty = static_cast<txStringProperty*>(property);
			wxProp = appendStringProperty(stringProperty, stringProperty->getValue(firstReceiver));
		}
		else if (PT_VECTOR2 == propertyType)
		{
			txVector2Property* vector2Property = static_cast<txVector2Property*>(property);
			wxProp = appendVectorProperty<txVector2Property, wxVector2Property, VECTOR2>(vector2Property, vector2Property->getValue(firstReceiver));
		}
		else if (PT_VECTOR3 == propertyType)
		{
			txVector3Property* vector3Property = static_cast<txVector3Property*>(property);
			wxProp = appendVectorProperty<txVector3Property, wxVector3Property, VECTOR3>(vector3Property, vector3Property->getValue(firstReceiver));
		}
		else if (PT_VECTOR4 == propertyType)
		{
			txVector4Property* vector4Property = static_cast<txVector4Property*>(property);
			wxProp = appendVectorProperty<txVector4Property, wxVector4Property, VECTOR4>(vector4Property, vector4Property->getValue(firstReceiver));
		}
		else if (PT_FLOAT == propertyType)
		{
			txFloatProperty* floatProperty = static_cast<txFloatProperty*>(property);
			wxProp = appendFloatProperty(floatProperty, floatProperty->getValue(firstReceiver));
		}
		else if (PT_TEXTURE == propertyType)
		{
			txTextureProperty* textureProperty = static_cast<txTextureProperty*>(property);
			wxProp = appendTextureProperty(textureProperty, textureProperty->get(firstReceiver));
		}
		else if (PT_ENUM == propertyType)
		{
			txEnumProperty<int>* enumProperty = static_cast<txEnumProperty<int>*>(property);
			wxProp = appendEnumProperty(enumProperty, enumProperty->get(firstReceiver));
		}
		// 设置属性描述
		wxString description = wxString(property->getPropertyDescirption().c_str());
		mPropertyStandPage->SetPropertyHelpString(wxString(property->getName()), description);
		// 根据只读属性设置是否可修改
		if (wxProp != NULL)
		{
			wxProp->Enable(!property->isReadOnly());
		}
	}
	END_FOR_STL(propertyList);
}

PROPERTY_TYPE PropertyPanelBase::getSelectPropertyType(const std::string& propertyName)
{
	if (mPropertyReceiverList.size() > 0)
	{
		txProperty* prop = mPropertyReceiverList[0]->getProperty(propertyName);
		if (prop != NULL)
		{
			return prop->getType();
		}
	}
	return PT_STRING;
}

void PropertyPanelBase::setReceiverProperty(const std::string& propertyName, const std::string& propertyValue, bool replaceKeyword, bool floatOrInt)
{
	int componentCount = mPropertyReceiverList.size();
	FOR_STL(mPropertyReceiverList, int i = 0; i < componentCount; ++i)
	{
		std::string finalValue = propertyValue;
		if (replaceKeyword)
		{
			txMath::replaceKeywordAndCalculate(finalValue, "i", i, floatOrInt);
		}
		setProperty(mPropertyReceiverList[i], propertyName, finalValue);
	}
	END_FOR_STL(mPropertyReceiverList);
}

void PropertyPanelBase::setSubPropertyValue(wxPGProperty* subProperty, const std::string& subPropertyValue, bool floatOrInt)
{
	int componentCount = mPropertyReceiverList.size();
	FOR_STL(mPropertyReceiverList, int i = 0; i < componentCount; ++i)
	{
		std::string finalValue = subPropertyValue;
		txMath::replaceKeywordAndCalculate(finalValue, "i", i, floatOrInt);

		// 被修改的子属性的父属性
		wxPGProperty* parentProperty = subProperty->GetParent();
		// 被修改的子属性的父属性名字
		std::string parentName = parentProperty->GetName().ToStdString();
		// 获得窗口属性
		txProperty* windowProperty = mPropertyReceiverList[i]->getProperty(parentName);
		// 需要将子属性名中包含的父属性名去掉
		std::string subPropertyName = subProperty->GetName().ToStdString();
		subPropertyName = subPropertyName.substr(parentName.length() + 1, subPropertyName.length() - parentName.length() - 1);
		// 获得只修改其中一个分量后的属性值
		std::string valueString = changeSubProperty(windowProperty->get(mPropertyReceiverList[i]), windowProperty->getType(), subPropertyName, finalValue);
		setProperty(mPropertyReceiverList[i], parentName, valueString);
	}
	END_FOR_STL(mPropertyReceiverList);
}