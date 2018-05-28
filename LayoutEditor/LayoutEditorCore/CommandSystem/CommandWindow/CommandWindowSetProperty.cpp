#include "txProperty.h"

#include "WindowHeader.h"

#include "CommandHeader.h"
#include "ECUndoChangeWindowProperty.h"
#include "ECUndoManager.h"
#include "EditorCoreRoot.h"
#include "LayoutEditorCore.h"

void CommandWindowSetProperty::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	txProperty* property = window->getProperty(mPropertyName);
	if (property == NULL)
	{
		return;
	}
	std::string oldValue = property->get(window);
	window->setProperty(mPropertyName, mPropertyValue);
	// 发送窗口属性改变的事件
	txVector<std::string> params;
	params.push_back(mPropertyName);
	params.push_back(oldValue);
	params.push_back(mPropertyValue);
	params.push_back(window->getName());
	params.push_back(txStringUtility::intToString((int)(mOperator)));
	mEditorCoreRoot->sendEvent(CE_WINDOW_PROPERTY_CHANGED, params);
	mLayoutEditorCore->setLayoutModified(true);

	// 添加到撤销列表
	ECUndoChangeWindowProperty* propertyUndo = mUndoManager->createUndo<ECUndoChangeWindowProperty>();
	propertyUndo->mWindowName = window->getName();
	propertyUndo->mPropertyName = mPropertyName;
	propertyUndo->mPropertyValue = oldValue;
	mUndoManager->addUndo(propertyUndo);
}

std::string CommandWindowSetProperty::showDebugInfo()
{
	COMMAND_DEBUG("property name : %s, property value : %s, operator : 0x%p", mPropertyName.c_str(), mPropertyValue.c_str(), mOperator);
}