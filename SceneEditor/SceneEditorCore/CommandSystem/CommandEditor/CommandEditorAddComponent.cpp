#include "txComponent.h"

#include "CoreCommandHeader.h"
#include "ECEditor.h"
#include "EditorCoreRoot.h"
#include "ECUndoManager.h"
#include "ECUndoAddComponent.h"
#include "GameLog.h"
#include "SceneEditorCore.h"

void CommandEditorAddComponent::execute()
{
	ECEditor* editor = CMD_CAST<ECEditor*>(mReceiver);
	// 如果源组件不为空,则使用源组件的类型
	if (mComponent != NULL)
	{
		mType = mComponent->getType();
	}

	// 添加组件
	txComponent* component = editor->addComponent(mComponentName, mType);
	txComponent* parent = NULL;
	if (mParentComponentName != EMPTY_STRING)
	{
		parent = editor->getComponent(mParentComponentName);
	}
	// 如果是添加碰撞组件,则父组件必须是物理组件
	if (component->getBaseType() == TOSTRING(txComponentCollision) && (parent == NULL || parent->getBaseType() != TOSTRING(txComponentPhysics)))
	{
		GAME_ERROR("error : collision component must be attach to physics component!");
		editor->destroyComponent(component->getName());
		return;
	}
	// 如果有源组件,则将源组件的所有属性拷贝到新组件中
	if (mComponent != NULL)
	{
		mComponent->copyProperty(component);
	}

	// 先将子组件与原来的组件拥有者和父组件的联系断开
	component->detachOwnerParentComponent();
	// 重新建立与组件拥有者和父组件的联系
	component->attachOwnerParentComponent(editor, parent, mComponentPos);

	// 添加组件后加入撤销操作
	ECUndoAddComponent* undoAdd = mUndoManager->createUndo<ECUndoAddComponent>();
	undoAdd->mEditorName = editor->getName();
	undoAdd->mComponentName = component->getName();
	mUndoManager->addUndo(undoAdd);

	// 发送已添加组件的事件
	txVector<std::string> params;
	params.push_back(editor->getName());
	params.push_back(component->getName());
	params.push_back(txStringUtility::intToString(mComponentPos));
	mEditorCoreRoot->sendEvent(CE_COMPONENT_ADDED, params);

	// 记录场景有改变
	mSceneEditorCore->setSceneModified(true);
}

std::string CommandEditorAddComponent::showDebugInfo()
{
	COMMAND_DEBUG("component type : %s, component name : %s", mType.c_str(), mComponentName.c_str());
}