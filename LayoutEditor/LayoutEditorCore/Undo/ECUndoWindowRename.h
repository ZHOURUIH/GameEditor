#ifndef _EC_UNDO_WINDOW_RENAME_H_
#define _EC_UNDO_WINDOW_RENAME_H_

#include "ECUndo.h"

// 撤销重命名
class ECUndoWindowRename : public ECUndo
{
public:
	ECUndoWindowRename(){}
	virtual ~ECUndoWindowRename(){}
	void setName(const std::string& name) { mName = name; }
	void setWindowName(const std::string& name) { mWindowName = name; }
	virtual void undo();
protected:
	std::string mName;			// 改名前的窗口名
	std::string mWindowName;	// 窗口名
};

#endif