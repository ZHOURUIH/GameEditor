#include "txAction.h"
#include "txActionTree.h"
#include "txActionTreeManager.h"
#include "ActionTreeEditorCore.h"
#include "EditorCoreRoot.h"
#include "ECLog.h"
#include "Utility.h"

ActionTreeEditorCore::ActionTreeEditorCore()
:
mCurAction(NULL),
mCurActionTree(NULL),
mActionTreeModified(false)
{
	;
}

void ActionTreeEditorCore::loadActionTree(const std::string& tree)
{
	if (mActionTreeModified && mCurActionTree != NULL && mCurActionTree->getName() != tree)
	{
		askSaveActionTree("是否保存当前的行为树?");
	}
	// 清空选择项,然后加载行为树
	selectActionTree(EMPTY_STRING);
	selectAction(EMPTY_STRING);
	txActionTree* action = mActionTreeManager->getActionTree(tree);
	// 发送加载行为树的事件
	mEditorCoreRoot->sendEvent(CE_LOAD_ACTION_TREE, tree);
	// 选中该行为树
	selectActionTree(action->getName());
	// 最后要发送通知事件
	setActionTreeModified(false, true);
}

void ActionTreeEditorCore::createActionTree(const std::string& name, const bool& createRoot)
{
	if (mActionTreeModified && mCurActionTree != NULL)
	{
		askSaveActionTree("是否保存当前的行为树?");
	}
	selectActionTree(EMPTY_STRING);
	selectAction(EMPTY_STRING);
	txActionTree* actionTree = mActionTreeManager->createActionTree(name, createRoot);
	if (actionTree == NULL)
	{
		return;
	}
	// 发送加载行为树的事件
	mEditorCoreRoot->sendEvent(CE_NEW_ACTION_TREE, actionTree->getName());
	// 选中该行为树
	selectActionTree(actionTree->getName());
	// 最后要设置修改状态
	setActionTreeModified(true, true);
}

void ActionTreeEditorCore::deleteActionTree(const std::string& name)
{
	// 因为name是引用,所以需要创建一个临时的变量
	std::string treeName = name;
	bool ret = mActionTreeManager->destroyActionTree(treeName);
	if (ret)
	{
		selectActionTree(EMPTY_STRING);
		selectAction(EMPTY_STRING);
		mEditorCoreRoot->sendEvent(CE_DELETE_ACTION_TREE, treeName);
	}
}

void ActionTreeEditorCore::selectActionTree(const std::string& name)
{
	if (mActionTreeModified && mCurActionTree != NULL)
	{
		askSaveActionTree("是否保存当前的行为树?");
	}
	mCurActionTree = mActionTreeManager->getActionTree(name);
	// 发送选中行为树的事件
	mEditorCoreRoot->sendEvent(CE_SELECT_ACTION_TREE, mCurActionTree != NULL ? mCurActionTree->getName() : EMPTY_STRING);
}

void ActionTreeEditorCore::addAction(const std::string& actionType, const std::string& actionName, const std::string& parentName)
{
	if (mCurActionTree == NULL)
	{
		return;
	}
	txAction* parent = mCurActionTree->getAction(parentName);
	if (parent == NULL && mCurActionTree->getRootAction())
	{
		EDITOR_CORE_ERROR("行为树已经有根节点了,无法再添加根节点,请选择一个父节点再添加行为!");
		return;
	}
	txAction* action = mCurActionTree->addAction(actionType, actionName, parent);
	if (action == NULL)
	{
		return;
	}
	// 发送添加行为的事件
	txVector<std::string> params;
	params.push_back(action->getName());
	params.push_back(parentName);
	mEditorCoreRoot->sendEvent(CE_ADD_ACTION, params);
	// 设置修改状态
	setActionTreeModified(true, true);
}

void ActionTreeEditorCore::deleteAction(const std::string& actionName)
{
	if (mCurActionTree == NULL)
	{
		return;
	}
	clearActionSelection();
	// 因为actionName传进来的是引用,所以为了避免崩溃,需要使用一个临时变量
	std::string name = actionName;
	bool ret = mCurActionTree->deleteAction(name);
	if (!ret)
	{
		return;
	}
	// 发送删除行为的事件
	mEditorCoreRoot->sendEvent(CE_DELETE_ACTION, name);
	// 设置修改状态
	setActionTreeModified(true, true);
}

void ActionTreeEditorCore::setActionTreeModified(const bool& modified, const bool& force, const bool& sendEvt)
{
	if (mActionTreeModified != modified || force)
	{
		mActionTreeModified = modified;
		if (sendEvt)
		{
			// 发送修改状态改变的事件
			txVector<std::string> params;
			params.push_back(mCurActionTree ? mCurActionTree->getName() : EMPTY_STRING);
			params.push_back(txStringUtility::boolToString(mActionTreeModified));
			mEditorCoreRoot->sendEvent(CE_ACTION_TREE_MODIFIED, params);
		}
	}
}

void ActionTreeEditorCore::saveActionTree()
{
	;
}

void ActionTreeEditorCore::selectAction(const std::string& actionName)
{
	// 不能重复设置选中项,避免无限循环的递归调用
	if (mCurAction != NULL && mCurAction->getName() == actionName)
	{
		return;
	}
	clearActionSelection(true);
	if (mCurActionTree != NULL)
	{
		mCurAction = mCurActionTree->getAction(actionName);
	}
	mEditorCoreRoot->sendEvent(CE_ACTION_SELECTION_CHANGED);
}

void ActionTreeEditorCore::reselectAction()
{
	mEditorCoreRoot->sendEvent(CE_ACTION_SELECTION_CHANGED);
}

void ActionTreeEditorCore::clearActionSelection(const bool& sendEvt)
{
	mCurAction = NULL;
	if (sendEvt)
	{
		mEditorCoreRoot->sendEvent(CE_ACTION_SELECTION_CHANGED);
	}
}

bool ActionTreeEditorCore::askSaveActionTree(const std::string& info)
{
	MODAL_DIALOG_RESULT ret = mEditorCoreRoot->showModalYesNoCancel(info);
	// 点击是,则保存文件
	if (ret == MDR_YES)
	{
		saveActionTree();
	}
	// 点击否,则标记当前布局为未修改状态
	else if (ret == MDR_NO)
	{
		setActionTreeModified(false, false, false);
	}
	// 点击取消则不打开文件
	else
	{
		return false;
	}
	return true;
}