#ifndef _EC_UNDO_CHANGE_WINDOW_PROPERTY_H_
#define _EC_UNDO_CHANGE_WINDOW_PROPERTY_H_

#include "ECUndo.h"

// 撤销属性改变
class ECUndoChangeWindowProperty : public ECUndo
{
public:
	ECUndoChangeWindowProperty(){}
	virtual ~ECUndoChangeWindowProperty() {}
	virtual void undo();
public:
	std::string mWindowName;		// 窗口名
	std::string mPropertyName;		// 属性名
	std::string mPropertyValue;		// 原来的属性值
};

#endif