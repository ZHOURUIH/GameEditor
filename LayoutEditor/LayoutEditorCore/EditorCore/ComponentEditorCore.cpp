#include "ComponentEditorCore.h"
#include "EditorCoreRoot.h"

ComponentEditorCore::ComponentEditorCore()
{
	;
}

void ComponentEditorCore::selectComponent(txComponent* component)
{
	// 不能重复设置选中项,避免无限循环的递归调用
	if (mComponentSelections.size() == 1 && mComponentSelections[0] == component)
	{
		return;
	}

	clearComponentSelection(false);
	if (component != NULL)
	{
		mComponentSelections.push_back(component);
		mComponentSelSet.insert(component);
	}

	mEditorCoreRoot->sendEvent(CE_COMPONENT_SELECTION_CHANGED);
}

void ComponentEditorCore::selectComponent(txVector<txComponent*>& componentSelections)
{
	bool sameSelection = true;
	// 不能重复设置选中项,避免无限循环的递归调用
	if (mComponentSelections.size() == componentSelections.size())
	{
		int selectionCount = mComponentSelections.size();
		FOR_STL(mComponentSelections, int i = 0; i < selectionCount; ++i)
		{
			if (mComponentSelections[i] != componentSelections[i])
			{
				sameSelection = false;
				break;
			}
		}
		END_FOR_STL(mComponentSelections);
	}
	else
	{
		sameSelection = false;
	}
	if (sameSelection)
	{
		return;
	}

	clearComponentSelection(false);
	int selCount = componentSelections.size();
	FOR_STL(componentSelections, int i = 0; i < selCount; ++i)
	{
		auto iter = mComponentSelSet.find(componentSelections[i]);
		if (iter == mComponentSelSet.end())
		{
			mComponentSelections.push_back(componentSelections[i]);
			mComponentSelSet.insert(componentSelections[i]);
		}
	}
	END_FOR_STL(componentSelections);

	mEditorCoreRoot->sendEvent(CE_COMPONENT_SELECTION_CHANGED);
}

void ComponentEditorCore::reselectComponent()
{
	mEditorCoreRoot->sendEvent(CE_COMPONENT_SELECTION_CHANGED);
}

void ComponentEditorCore::clearComponentSelection(const bool& sendEvt)
{
	mComponentSelections.clear();
	mComponentSelSet.clear();
	if (sendEvt)
	{
		mEditorCoreRoot->sendEvent(CE_COMPONENT_SELECTION_CHANGED);
	}
}