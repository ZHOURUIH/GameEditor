#include "SceneEditorCore.h"
#include "ECScene.h"
#include "CameraManager.h"
#include "txCamera.h"
#include "EditorCoreRoot.h"
#include "GameCamera.h"
#include "ECSceneManager.h"
#include "txUtility.h"
#include "ECUndoManager.h"
#include "ComponentEditorCore.h"
#include "ECAxis.h"
#include "ECGrid.h"

SceneEditorCore::SceneEditorCore()
:
mSceneModified(false),
mCurScene(NULL)
{
	;
}

SceneEditorCore::~SceneEditorCore()
{
	destroyScene();
}

void SceneEditorCore::update(const float& elapsedTime)
{
	if (mCurScene != NULL)
	{
		// 更新场景
		mCurScene->update(elapsedTime);
	}
}

void SceneEditorCore::render()
{
	// 渲染辅助网格
	if (mCurScene != NULL)
	{
		txCamera* engineCamera = mCameraManager->getMainCamera()->getCamera();
		mCurScene->renderHelper(engineCamera);
	}
}

void SceneEditorCore::setSceneModified(bool modified, bool force, bool notifyEvent)
{
	if (modified != mSceneModified || force)
	{
		mSceneModified = modified;
		if (notifyEvent)
		{
			// 发送修改状态改变的事件
			txVector<std::string> params;
			params.push_back(mCurScene ? mCurScene->getName() : EMPTY_STRING);
			params.push_back(txStringUtility::boolToString(mSceneModified));
			mEditorCoreRoot->sendEvent(CE_SCENE_MODIFIED, params);
		}
	}
}

void SceneEditorCore::newScene(const std::string& newSceneName)
{
	if (isSceneModified())
	{
		if (!askSaveScene("文件未保存,是否保存然后新建文件?"))
		{
			return;
		}
	}

	// 首先关闭场景
	destroyScene();
	// 然后创建场景
	mCurScene = mEditorSceneManager->createScene(newSceneName);
	mCurScene->readSceneFile(true);

	// 发送新建场景的事件
	mEditorCoreRoot->sendEvent(CE_SCENE_NEW, newSceneName);

	setSceneModified(true, true);
}

void SceneEditorCore::openAdvanceScene(const std::string& sceneName)
{
	if (isSceneModified())
	{
		if (!askSaveScene("当前文件未保存,是否保存文件然后打开新的文件?"))
		{
			return;
		}
	}
	// 首先关闭场景
	destroyScene();

	// 然后创建场景
	mCurScene = mEditorSceneManager->createScene(sceneName);
	mCurScene->readAdvanceScene(false);

	// 发送打开场景的事件
	mEditorCoreRoot->sendEvent(CE_SCENE_OPEN, sceneName);

	// 强制发送布局改变的事件,以便能在加载布局时提示界面层修改窗口标题
	setSceneModified(false, true);
}

void SceneEditorCore::saveAdvanceScene()
{
	if (mCurScene != NULL)
	{
		mCurScene->saveAdvanceScene();
	}
	setSceneModified(false);
}

void SceneEditorCore::openScene(const std::string& sceneName, const bool& binary)
{
	if (isSceneModified())
	{
		if (!askSaveScene("当前文件未保存,是否保存文件然后打开新的文件?"))
		{
			return;
		}
	}

	// 首先关闭场景
	destroyScene();
	// 然后创建场景
	mCurScene = mEditorSceneManager->createScene(sceneName);
	mCurScene->readSceneFile(true);

	// 发送打开场景的事件
	mEditorCoreRoot->sendEvent(CE_SCENE_OPEN, sceneName);

	// 强制发送布局改变的事件,以便能在加载布局时提示界面层修改窗口标题
	setSceneModified(false, true);
}

bool SceneEditorCore::transSceneFile(const std::string& fullFileName, const bool& tranMeshFile)
{
	std::string newSceneName;
	return txLoadUtility::jsonSceneFileToBinarySceneFile(fullFileName, newSceneName, tranMeshFile);
}

bool SceneEditorCore::transSkinMeshFile(const std::string& fullFileName, const bool& transAllFile)
{
	// 将全部的\转换为/
	std::string fileName = fullFileName;
	txStringUtility::rightToLeft(fileName);
	if (!transAllFile)
	{
		std::string newFileName;
		return txLoadUtility::jsonSkeletonMeshFileToBinarySkeletonMeshFile(fileName, newFileName);
	}
	else
	{
		// 遍历文件所在目录中的所有文件,找出动态模型文件
		std::string filPath = txStringUtility::getFilePath(fileName);
		txVector<std::string> fileList;
		txFileUtility::findFiles(filPath, fileList, ".json");
		int fileCount = fileList.size();
		FOR_STL(fileList, int i = 0; i < fileCount; ++i)
		{
			const std::string& filename = fileList[i];
			// 去除以_skeleton.json结尾的骨骼文件和_animation.json结尾的动作文件
			if (!txStringUtility::endWith(filename, "_skeleton.json") && !txStringUtility::endWith(filename, "_animation.json"))
			{
				std::string newFileName;
				if (!txLoadUtility::jsonSkeletonMeshFileToBinarySkeletonMeshFile(filename, newFileName))
				{
					END_FOR_STL(fileList);
					return false;
				}
			}
		}
		END_FOR_STL(fileList);
		return true;
	}
}

void SceneEditorCore::destroyScene(ECScene* scene)
{
	if (isSceneModified())
	{
		if (!askSaveScene("当前文件未保存,是否保存然后关闭文件?"))
		{
			return;
		}
	}
	if (mCurScene == NULL)
	{
		return;
	}
	if (mCurScene == scene)
	{
		// 首先清空所有选中的物体
		clearEditorSelection();
		mComponentEditorCore->clearComponentSelections();
		// 先发送关闭场景的事件
		mEditorCoreRoot->sendEvent(CE_SCENE_DESTROIED, mCurScene->getName());
		// 标记场景未修改
		setSceneModified(false);
		// 销毁场景
		mEditorSceneManager->destroyScene(mCurScene->getName());
	}

	// 销毁场景时清空所有撤销操作
	mUndoManager->clearRedo();
	mUndoManager->clearUndo();
}

void SceneEditorCore::setEditorSelection(ECEditor* selection)
{
	txVector<ECEditor*> editorList;
	editorList.push_back(selection);
	setEditorSelection(editorList);
}

void SceneEditorCore::setEditorSelection(txVector<ECEditor*>& selection)
{
	bool sameSelection = true;
	// 不能重复设置选中项,避免无限循环的递归调用
	if (mEditorMultiSelections.size() == selection.size())
	{
		int selectionCount = mEditorMultiSelections.size();
		FOR_STL(mEditorMultiSelections, int i = 0; i < selectionCount; ++i)
		{
			if (mEditorMultiSelections[i] != selection[i])
			{
				sameSelection = false;
				break;
			}
		}
		END_FOR_STL(mEditorMultiSelections);
	}
	else
	{
		sameSelection = false;
	}
	if (sameSelection)
	{
		return;
	}

	clearEditorSelection();
	int selCount = selection.size();
	FOR_STL(selection, int i = 0; i < selCount; ++i)
	{
		auto iter = mEditorMultiSelSet.find(selection[i]);
		if (iter == mEditorMultiSelSet.end())
		{
			mEditorMultiSelections.push_back(selection[i]);
			mEditorMultiSelSet.insert(selection[i]);
		}
	}
	END_FOR_STL(selection);
	// 发出选择项改变的事件
	notifyEditorSelectionChanged();
}

void SceneEditorCore::reselectEditor()
{
	notifyEditorSelectionChanged();
}

void SceneEditorCore::clearEditorSelection()
{
	mEditorMultiSelSet.clear();
	mEditorMultiSelections.clear();
	// 发出选择项改变的事件
	notifyEditorSelectionChanged();
	// 编辑体选中项改变时,清空组件选中项
	mComponentEditorCore->clearComponentSelections();
}

void SceneEditorCore::notifyEditorSelectionChanged(const bool& notifyEvent)
{
	if (notifyEvent)
	{
		mEditorCoreRoot->sendEvent(CE_EDITOR_SELECTION_CHANGED);
	}
	if (mCurScene != NULL)
	{
		// 选择项改变时需要将网格和坐标轴设置到该编辑体上
		ECAxis* axis = mCurScene->getEditorAxis();
		ECGrid* grid = mCurScene->getEditorGrid();
		if (mEditorMultiSelections.size() == 0)
		{
			axis->attachTarget(NULL);
			grid->attachTarget(NULL);
		}
		else
		{
			axis->attachTarget(mEditorMultiSelections[0]);
			grid->attachTarget(mEditorMultiSelections[0]);
		}
	}
}

bool SceneEditorCore::askSaveScene(const std::string& info)
{
	MODAL_DIALOG_RESULT ret = mEditorCoreRoot->showModalYesNoCancel(info);
	// 点击是,则保存文件
	if (ret == MDR_YES)
	{
		saveAdvanceScene();
	}
	// 点击否,则标记当前布局为未修改状态
	else if (ret == MDR_NO)
	{
		setSceneModified(false, false, false);
	}
	else
	{
		return false;
	}
	return true;
}