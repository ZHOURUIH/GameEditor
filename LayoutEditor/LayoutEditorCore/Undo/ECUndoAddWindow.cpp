#include "ECUndoAddWindow.h"
#include "LayoutEditorCore.h"

void ECUndoAddWindow::undo()
{
	// 撤销添加窗口,就是将添加的窗口删除
	mLayoutEditorCore->deleteWindow(mWindowName);
}