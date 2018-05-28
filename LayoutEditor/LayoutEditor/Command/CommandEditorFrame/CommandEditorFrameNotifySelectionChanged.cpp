#include "WindowHeader.h"

#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "LayoutHierarchy.h"
#include "EditorCommandHeader.h"
#include "InnerChildHierarchy.h"
#include "ComponentViewer.h"

void CommandEditorFrameNotifySelectionChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// 选择项改变时,需要显示选择项属性,并且要确定在树形列表窗口中选中该项
	txVector<Window*>& selections = mLayoutEditorCore->getSelections();
	if (selections.size() > 0)
	{
		mPropertyWindow->showPropertyT(selections);
		txVector<std::string> selectionNameList;
		int selectionCount = selections.size();
		FOR_STL(selections, int i = 0; i < selectionCount; ++i)
		{
			selectionNameList.push_back(selections[i]->getName());
		}
		END_FOR_STL(selections);
		mLayoutHierarchy->setSelection(selectionNameList);
		// 由于内部子窗口列表不允许多选,所以只要是多选,就一定要清空内部子窗口视图
		if (selections.size() == 1)
		{
			// 有三种情况,选中了一个复合窗口
			if (selections[0]->isComplexWindow())
			{
				mInnerChildHierarchy->displayInnerChild(static_cast<ComplexWindow*>(selections[0]));
			}
			else
			{
				// 选中了一个非复合窗口并且非内部子窗口
				if (!selections[0]->isInnerChild())
				{
					mInnerChildHierarchy->displayInnerChild(NULL);
				}
				// 选中了一个内部子窗口,什么也不做
				else
				{
					;
				}
			}
		}
		else
		{
			mInnerChildHierarchy->displayInnerChild(NULL);
		}
	}
	else
	{
		mPropertyWindow->showReceiverProperty(NULL);
		mLayoutHierarchy->unSelectAll();
		mInnerChildHierarchy->displayInnerChild(NULL);
	}
	mComponentViewer->refreshList();
}

std::string CommandEditorFrameNotifySelectionChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}