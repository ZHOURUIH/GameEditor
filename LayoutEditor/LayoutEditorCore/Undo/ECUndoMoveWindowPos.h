#ifndef _EC_UNDO_MOVE_WINDOW_POS_H_
#define _EC_UNDO_MOVE_WINDOW_POS_H_

#include "ECUndo.h"

class ECUndoMoveWindowPos : public ECUndo
{
public:
	ECUndoMoveWindowPos()
		:
		mWindowName(EMPTY_STRING),
		mOldPos(-1)
	{}
	virtual ~ECUndoMoveWindowPos(){}
	void setWindowName(const std::string& name) { mWindowName = name; }
	void setPos(int pos) { mOldPos = pos; }
	virtual void undo();
protected:
	std::string mWindowName;
	int mOldPos;
};

#endif