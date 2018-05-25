#include "CoreCommandHeader.h"
#include "ECScene.h"
#include "SceneEditorCore.h"
#include "EditorCoreRoot.h"

void CommandSceneRename::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	// 此处不能使用引用
	std::string oldName = scene->getName();
	bool result = scene->rename(mNewName);
	// 如果改名成功,则需要发送布局改名的事件
	if (result)
	{
		txVector<std::string> params;
		params.push_back(oldName);
		params.push_back(mNewName);
		mEditorCoreRoot->sendEvent(CE_SCENE_RENAMED, params);

		// 记录布局有改变
		mSceneEditorCore->setSceneModified(true);
	}
	if (mResult != NULL)
	{
		*mResult = result;
	}
}

std::string CommandSceneRename::showDebugInfo()
{
	COMMAND_DEBUG("new name : %s", mNewName.c_str());
}