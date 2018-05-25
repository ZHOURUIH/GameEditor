#ifndef _SCENE_EDITOR_CORE_H_
#define _SCENE_EDITOR_CORE_H_

#include "CommonDefine.h"
#include "EditorCoreBase.h"
#include "txEngineBase.h"

class ECScene;
class ECEditor;
class SceneEditorCore : public EditorCoreBase, public txEngineBase
{
public:
	SceneEditorCore();
	~SceneEditorCore();
	virtual void init(){}
	virtual void update(const float& elapsedTime);
	virtual void render();
	ECScene* getCurScene() { return mCurScene; }
	txVector<ECEditor*>& getEditorSelections() { return mEditorMultiSelections; }
	void setEditorSelection(ECEditor* selection);
	void setEditorSelection(txVector<ECEditor*>& selection);
	void reselectEditor();
	void clearEditorSelection();
	void newScene(const std::string& newSceneName);
	void openAdvanceScene(const std::string& sceneName);
	void saveAdvanceScene();
	// 打开场景,参数是场景名
	void openScene(const std::string& sceneName, const bool& binary);
	// 转换json场景文件到二进制场景文件
	bool transSceneFile(const std::string& fullFileName, const bool& tranMeshFile);
	// 转换json动态模型文件到二进制动态模型文件,transAllFile为是否转换文件所在目录中的所有动态模型
	bool transSkinMeshFile(const std::string& fullFileName, const bool& transAllFile);
	void destroyScene(){ destroyScene(mCurScene); }
	void destroyScene(ECScene* scene);
	// force表示是否强制设置状态,即使状态相同也要设置,并且发送状态改变的事件
	void setSceneModified(bool modified, bool force = false, bool notifyEvent = true);
	bool isSceneModified() { return mSceneModified; }
protected:
	bool askSaveScene(const std::string& info);
	void notifyEditorSelectionChanged(const bool& notifyEvent = true);
protected:
	txVector<ECEditor*> mEditorMultiSelections;			// 所有被选中的物体,并且有顺序之分
	txSet<ECEditor*> mEditorMultiSelSet;				// 所有被选中的物体,仅用作判断是否选中了相同的物体
	ECScene* mCurScene;									// 当前场景
	bool mSceneModified;								// 当前的场景是否改变过
};

#endif