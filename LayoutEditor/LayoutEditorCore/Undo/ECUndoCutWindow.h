#ifndef _EC_UNDO_CUT_WINDOW_H_
#define _EC_UNDO_CUT_WINDOW_H_

#include "ECUndo.h"

class Window;
class Layout;
class ECUndoCutWindow : public ECUndo
{
public:
	ECUndoCutWindow()
		:
		mOldWindowPos(-1)
	{}
	virtual ~ECUndoCutWindow(){}
	void setCutWindowInfo(const std::string& windowName, const std::string& parentName, const int& pos)
	{
		mWindowName = windowName;
		mOldParentName = parentName;
		mOldWindowPos = pos;
	}
	virtual void undo();
protected:
	std::string mWindowName;
	std::string mOldParentName;	// 被剪切窗口的父窗口的名字
	int mOldWindowPos;			// 被剪切的窗口在父窗口中的顺序下标
};

#endif