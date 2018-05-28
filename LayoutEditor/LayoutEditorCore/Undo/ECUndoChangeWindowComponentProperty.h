#ifndef _EC_UNDO_CHANGE_WINDOW_COMPONENT_PROPERTY_H_
#define _EC_UNDO_CHANGE_WINDOW_COMPONENT_PROPERTY_H_

#include "ECUndo.h"

// 撤销属性改变
class ECUndoChangeWindowComponentProperty : public ECUndo
{
public:
	ECUndoChangeWindowComponentProperty(){}
	virtual ~ECUndoChangeWindowComponentProperty() {}
	virtual void undo();
public:
	std::string mWindowName;		// 窗口名
	std::string mComponentName;		// 组件名
	std::string mPropertyName;		// 属性名
	std::string mPropertyValue;		// 原来的属性值
};

#endif