#ifndef _EC_SCENE_H_
#define _EC_SCENE_H_

#include "txComponentOwner.h"

#include "EditorCoreBase.h"

class txScene;
class txCamera;
class ECEditor;
class ECAxis;
class ECGrid;
class ECScene : public txComponentOwner, public EditorCoreBase
{
public:
	ECScene(const std::string& name);
	virtual ~ECScene(){ destroy(); }
	virtual void initComponents(){}
	virtual void update(float elapsedTime);
	void destroy();
	void readSceneFile(bool binary);
	void readAdvanceScene(bool async);
	bool rename(const std::string& newName);
	void renderHelper(txCamera* camera);
	ECEditor* createEditor(const std::string& name);
	txMap<std::string, ECEditor*>& getEditorList(){ return mEditorList; }
	void destroyEditor(const std::string& name);
	void destroyEditor(ECEditor* editor);
	ECEditor* getEditor(const std::string& name)
	{
		auto iter = mEditorList.find(name);
		if (iter != mEditorList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	void saveAdvanceScene();
	void setSceneVisible(bool visible);
	bool getSceneVisible() { return mSceneVisible; }
	void calculateSceneHeightMap(int precision = 1);
	bool isIgnoreEntity(const std::string& entityName){ return mIgnoreEntityName.find(entityName) != mIgnoreEntityName.end(); }
	void pushIgnoreEntity(const std::string& entityName) { mIgnoreEntityName.insert(entityName); }
	void clearIgnoreEntityList() { mIgnoreEntityName.clear(); }
	float* getHeightMap() { return mHeightMap; }
	bool* getHeightFlag() { return mHeightFlag; }
	int getHeightIndex() { return mHeightIndex; }
	txScene* getScene() { return mScene; }
	ECAxis* getEditorAxis() { return mEditorAxis; }
	ECGrid* getEditorGrid() { return mEditorGrid; }
	ECGrid* getSceneGrid() { return mSceneGrid; }
	void setRectDirty(bool dirty) { mSceneRectDirty = dirty; }
	bool isRectDirty() { return mSceneRectDirty; }
	virtual bool notifyEditorRenamed(const std::string& oldName, ECEditor* editor);
	virtual void notifyEditorComponentProertyChanged(txComponent* component, const std::string& propertyName, const std::string& propertyValue);
	virtual void notifyEditorDeleteComponent(txComponent* component);
	void generateSceneRect();
protected:
	void closeScene();
protected:
	ECAxis* mEditorAxis;
	ECGrid* mEditorGrid;
	ECGrid* mSceneGrid;
	txMap<std::string, ECEditor*> mEditorList;		// 该场景中的编辑体列表
	txScene* mScene;
	// 生成高度图时需要忽略的实体名
	txSet<std::string> mIgnoreEntityName;
	int mHeightIndex;
	float* mHeightMap;
	bool* mHeightFlag;
	bool mSceneVisible;
	bool mEditorAxisVisible;
	bool mEditorGridVisible;
	bool mSceneGridVisible;
	bool mSceneRectDirty;
};

#endif