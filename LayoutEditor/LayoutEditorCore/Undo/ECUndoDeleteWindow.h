#ifndef _EC_UNDO_DELETE_WINDOW_H_
#define _EC_UNDO_DELETE_WINDOW_H_

#include "ECUndo.h"

class Window;
class Layout;
class ECUndoDeleteWindow : public ECUndo
{
public:
	ECUndoDeleteWindow()
		:
		mWindow(NULL),
		mTempLayout(NULL)
	{}
	virtual ~ECUndoDeleteWindow();
	void setWindow(Window* window);
	virtual void undo();
protected:
	Window* mWindow;
	Layout* mTempLayout;
	std::string mOldLayoutName; // 被删除窗口的所属布局的名字
	std::string mOldParentName;	// 被删除窗口的父窗口的名字
	int mOldWindowPos;			// 被删除的窗口在父窗口中的顺序下标
	static int mDeleteLayoutCount;
};

#endif