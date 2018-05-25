#ifndef _COMPONENT_EDITOR_CORE_H_
#define _COMPONENT_EDITOR_CORE_H_

#include "CommonDefine.h"
#include "EditorCoreBase.h"
#include "txEngineBase.h"

class txComponent;
class ComponentEditorCore : public EditorCoreBase, public txEngineBase
{
public:
	ComponentEditorCore(){}
	~ComponentEditorCore(){}
	virtual void init(){}
	virtual void update(const float& elapsedTime){}
	virtual void render(){}
	txVector<txComponent*>& getComponentSelections() { return mComponentMultiSelections; }
	void setComponentSelection(txComponent* component);
	void setComponentSelection(txVector<txComponent*>& selections);
	void reselectComponent();
	void clearComponentSelections();
protected:
	void notifyComponentSelectionChanged(const bool& notifyEvent = true);
protected:
	txVector<txComponent*> mComponentMultiSelections;	// 所有被选中的组件,并且有顺序之分
	txSet<txComponent*> mComponentMultiSelSet;			// 所有被选中的组件,仅用作判断是否选中了相同的组件
};

#endif