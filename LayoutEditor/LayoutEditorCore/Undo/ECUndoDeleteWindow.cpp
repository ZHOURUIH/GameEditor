#include "WindowHeader.h"
#include "Layout.h"
#include "WindowProperty.h"
#include "LayoutManager.h"

#include "ECUndoDeleteWindow.h"
#include "LayoutEditorCore.h"

int ECUndoDeleteWindow::mDeleteLayoutCount = 0;

ECUndoDeleteWindow::~ECUndoDeleteWindow()
{
	if (mTempLayout != NULL)
	{
		mLayoutManager->destroyTempLayout(mTempLayout->getName());
	}
}

void ECUndoDeleteWindow::setWindow(Window* window)
{
	if (mWindow != NULL)
	{
		return;
	}
	mTempLayout = mLayoutManager->createTempLayout("UndoDeleteWindow" + txStringUtility::intToString(mDeleteLayoutCount++));
	mWindow = mTempLayout->createWindow(window->getTypeName(), window->getName());
	// 先把窗口的所有属性拷贝到临时窗口,没有链接任何预设时,才拷贝子窗口
	window->copyProperty(mWindow);
	if (!window->hasLinkedPrefab(false))
	{
		// 然后把窗口的所有子窗口拷贝到临时窗口
		window->copyChildren(mWindow, false, EMPTY_STRING);
	}
	if (window->getParent() != NULL)
	{
		mOldParentName = window->getParent()->getName();
		mOldWindowPos = window->getParent()->getChildPos(window);
	}
	if (window->getLayout() != NULL)
	{
		mOldLayoutName = window->getLayout()->getName();
	}
}

void ECUndoDeleteWindow::undo()
{
	// 撤销删除窗口,就是重新将窗口添加上去
	// 首先查找所属布局
	Layout* oldLayout = mLayoutManager->getLayout(mOldLayoutName);
	if (oldLayout == NULL)
	{
		return;
	}
	// 父窗口
	Window* parentWindow = oldLayout->getWindow(mOldParentName);
	// 直接将临时窗口添加到原来的父窗口上,临时窗口下的所有子窗口也会一起添加
	mLayoutEditorCore->addWindow(oldLayout, parentWindow, mWindow, mOldWindowPos);
}