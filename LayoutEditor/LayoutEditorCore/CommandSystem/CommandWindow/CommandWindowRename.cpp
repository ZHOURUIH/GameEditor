#include "WindowHeader.h"

#include "CommandHeader.h"
#include "EditorCoreRoot.h"
#include "LayoutEditorCore.h"
#include "ECUndoManager.h"
#include "ECUndoWindowRename.h"

void CommandWindowRename::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	if (window != NULL)
	{
		// 此处不能使用常量引用,因为此时会改变名字,导致oldName和newName相同
		std::string oldName = window->getName();
		bool result = window->rename(mNewName);
		if (mResult != NULL)
		{
			*mResult = result;
		}
		// 如果改名成功,则需要发送窗口改名的事件
		if (result)
		{
			txVector<std::string> params;
			params.push_back(oldName);
			params.push_back(window->getName());
			mEditorCoreRoot->sendEvent(CE_WINDOW_RENAMED, params);
			mLayoutEditorCore->reselect();

			// 记录布局有改变
			mLayoutEditorCore->setLayoutModified(true);

			// 添加到撤销列表
			ECUndoWindowRename* renameUndo = mUndoManager->createUndo<ECUndoWindowRename>();
			if (NULL != renameUndo)
			{
				renameUndo->setName(oldName);
				renameUndo->setWindowName(window->getName());
				mUndoManager->addUndo(renameUndo);
			}
		}
	}
}

std::string CommandWindowRename::showDebugInfo()
{
	COMMAND_DEBUG("new name : %s", mNewName.c_str());
}