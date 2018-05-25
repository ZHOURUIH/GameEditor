#ifndef _COMPONENT_EDITOR_CORE_H_
#define _COMPONENT_EDITOR_CORE_H_

#include "CommonDefine.h"
#include "txEngineBase.h"
#include "EditorCoreBase.h"

class txComponent;
class ComponentEditorCore : public txEngineBase, public EditorCoreBase
{
public:
	ComponentEditorCore();
	virtual void init(){}
	virtual void update(float elapsedTime){}
	txVector<txComponent*>& getComponentSelections() { return mComponentSelections; }
	void selectComponent(txComponent* component);
	void selectComponent(txVector<txComponent*>& componentSelections);
	void reselectComponent();
	void clearComponentSelection(const bool& sendEvt = true);
protected:
	txVector<txComponent*>	mComponentSelections;	// 所有被选中的组件,并且有顺序之分
	txSet<txComponent*>		mComponentSelSet;		// 所有被选中的组件,仅用作判断是否选中了相同的组件
};

#endif