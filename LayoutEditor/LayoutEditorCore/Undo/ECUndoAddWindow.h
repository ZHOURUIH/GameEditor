#ifndef _EC_UNDO_ADD_WINDOW_H_
#define _EC_UNDO_ADD_WINDOW_H_

#include "ECUndo.h"

class ECUndoAddWindow : public ECUndo
{
public:
	ECUndoAddWindow(){}
	virtual ~ECUndoAddWindow(){}
	void setLayoutName(const std::string& name) { mLayoutName = name; }
	void setWindowName(const std::string& name) { mWindowName = name; }
	virtual void undo();
protected:
	std::string mLayoutName;
	std::string mWindowName;
};

#endif