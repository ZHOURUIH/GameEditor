#include "ActionWindow.h"

#include "ActionTreeEditorCore.h"

#include "EditorCommandHeader.h"
#include "ActionPropertyPanel.h"

ActionPropertyPanel::ActionPropertyPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
PropertyPanelBase(parent, position, size, style, name)
{}

void ActionPropertyPanel::populateProperties(txVector<txPropertyReceiver*>& receiverList)
{
	int actionCount = receiverList.size();
	if (actionCount == 0)
	{
		return;
	}
	txAction* firstAction = static_cast<txAction*>(receiverList[0]);
	// 只显示一个窗口的属性时才会显示类型
	if (actionCount == 1)
	{
		const std::string& typeName = firstAction->getTypeName();
		mPropertyStandPage->Append(new wxStringProperty("Type", "Type", wxString(typeName.c_str())))->Enable(false);
		mPropertyStandPage->SetPropertyHelpString(wxString("Type"), wxT("类型"));
	}

	// 显示第一个组件的名字
	const std::string& actionName = firstAction->getName();
	mPropertyStandPage->Append(new wxStringProperty("Name", "Name", wxString(actionName.c_str())));
	mPropertyStandPage->SetPropertyHelpString(wxString("Name"), wxT("行为名字"));

	PropertyPanelBase::populateProperties(receiverList);
}

void ActionPropertyPanel::onRefreshProperty()
{
	mActionTreeEditorCore->reselectAction();
}

void ActionPropertyPanel::setProperty(txPropertyReceiver* receiver, const std::string& propertyName, const std::string& propertyValue)
{
	CommandActionSetProperty* cmd = NEW_CMD(cmd);
	cmd->mPropertyName = propertyName;
	cmd->mPropertyValue = propertyValue;
	cmd->mOperator = this;
	mCommandSystem->pushCommand(cmd, static_cast<ActionWindow*>(receiver));
}