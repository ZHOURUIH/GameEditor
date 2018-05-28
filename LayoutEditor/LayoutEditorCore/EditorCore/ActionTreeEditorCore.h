#ifndef _ACTION_TREE_EDITOR_CORE_H_
#define _ACTION_TREE_EDITOR_CORE_H_

#include "CommonDefine.h"
#include "txEngineBase.h"
#include "EditorCoreBase.h"

class txActionTree;
class txAction;
class ActionTreeEditorCore : public txEngineBase, public EditorCoreBase
{
public:
	ActionTreeEditorCore();
	virtual void init(){}
	virtual void update(float elapsedTime){}
	void loadActionTree(const std::string& tree);
	void createActionTree(const std::string& name = EMPTY_STRING, const bool& createRoot = true);
	void deleteActionTree(const std::string& name);
	void selectActionTree(const std::string& name);
	void addAction(const std::string& actionType, const std::string& actionName, const std::string& parentName);
	void deleteAction(const std::string& actionName);
	void setActionTreeModified(const bool& modified, const bool& force = false, const bool& sendEvt = true);
	void saveActionTree();
	void selectAction(const std::string& actioNname);
	void reselectAction();
	void clearActionSelection(const bool& sendEvt = true);
	txAction* getCurAction()					{ return mCurAction; }
	txActionTree* getCurActionTree()	{ return mCurActionTree; }
protected:
	bool askSaveActionTree(const std::string& info);
protected:
	txAction*		mCurAction;				// 当前选中的行为
	txActionTree*	mCurActionTree;			// 当前选中的行为树
	bool			mActionTreeModified;	// 行为树是否被修改
};

#endif