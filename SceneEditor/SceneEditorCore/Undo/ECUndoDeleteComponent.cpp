#include "txComponent.h"
#include "txComponentOwner.h"

#include "ECUndoDeleteComponent.h"
#include "EditorCoreRoot.h"
#include "ECScene.h"
#include "ECEditor.h"
#include "CoreCommandHeader.h"
#include "SceneEditorCore.h"

void ECUndoDeleteComponent::setComponent(txComponent* component)
{
	if (component == NULL || component->getOwner() == NULL)
	{
		return;
	}
	// 复制一个组件,保存组件的所有属性,保存编辑体名字,父组件名,组件位置
	mComponent = txComponentOwner::createIndependentComponent(component->getName(), component);
	mEditorName = component->getOwner()->getName();
	if (component->getParentComponent() != NULL)
	{
		mParentComponentName = component->getParentComponent()->getName();
	}
	mComponentPos = component->getOwner()->getComponentPosition(component);
}

void ECUndoDeleteComponent::undo()
{
	ECScene* scene = mSceneEditorCore->getCurScene();
	if (scene == NULL)
	{
		return;
	}
	ECEditor* editor = scene->getEditor(mEditorName);
	if (editor == NULL)
	{
		return;
	}
	// 添加组件
	CommandEditorAddComponent* cmdAdd = NEW_CMD(cmdAdd);
	cmdAdd->mComponent = mComponent;
	cmdAdd->mType = mComponent->getType();
	cmdAdd->mComponentName = mComponent->getName();
	cmdAdd->mComponentPos = mComponentPos;
	mCommandSystem->pushCommand(cmdAdd, editor);
	// 销毁源组件
	txComponentOwner::destroyComponent(mComponent);
}