#include "txProperty.h"
#include "txComponent.h"

#include "WindowHeader.h"

#include "CommandHeader.h"
#include "ECUndoChangeWindowComponentProperty.h"
#include "ECUndoManager.h"
#include "EditorCoreRoot.h"
#include "LayoutEditorCore.h"

void CommandWindowSetComponentProperty::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	txComponent* component = window->getComponent(mComponentName);
	txProperty* property = component->getProperty(mPropertyName);
	if (property == NULL)
	{
		return;
	}
	std::string oldValue = property->get(component);
	window->setComponentProperty(component, property, mPropertyValue);
	// 发送窗口属性改变的事件
	txVector<std::string> params;
	params.push_back(mComponentName);
	params.push_back(mPropertyName);
	params.push_back(oldValue);
	params.push_back(mPropertyValue);
	params.push_back(window->getName());
	params.push_back(txStringUtility::intToString((int)(mOperator)));
	mEditorCoreRoot->sendEvent(CE_WINDOW_COMPONENT_PROPERTY_CHANGED, params);
	mLayoutEditorCore->setLayoutModified(true);

	// 添加到撤销列表
	ECUndoChangeWindowComponentProperty* propertyUndo = mUndoManager->createUndo<ECUndoChangeWindowComponentProperty>();
	if (NULL == propertyUndo)
	{
		return;
	}
	propertyUndo->mWindowName = window->getName();
	propertyUndo->mComponentName = mComponentName;
	propertyUndo->mPropertyName = mPropertyName;
	propertyUndo->mPropertyValue = oldValue;
	mUndoManager->addUndo(propertyUndo);
}

std::string CommandWindowSetComponentProperty::showDebugInfo()
{
	COMMAND_DEBUG("component name : %s, property name : %s, property value : %s, operator : 0x%p", mComponentName.c_str(), mPropertyName.c_str(), mPropertyValue.c_str(), mOperator);
}