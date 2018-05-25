#include "WindowHeader.h"
#include "Layout.h"
#include "WindowProperty.h"
#include "LayoutManager.h"

#include "ECUndoCutWindow.h"
#include "LayoutEditorCore.h"

void ECUndoCutWindow::undo()
{
	// 撤销剪切窗口,就是重新将窗口挂接到原来的父窗口中
	Layout* layout = mLayoutEditorCore->getCurLayout();
	if (layout == NULL)
	{
		return;
	}
	// 父窗口
	Window* parentWindow = layout->getWindow(mOldParentName);
	mLayoutEditorCore->copyWindow(mWindowName, true, true);
	mLayoutEditorCore->pasteWindow(mOldParentName, mOldWindowPos);
}