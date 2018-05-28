#ifndef _EC_UNDO_CHANGE_COMPONENT_PROPERTY_H_
#define _EC_UNDO_CHANGE_COMPONENT_PROPERTY_H_

#include "ECUndo.h"

// 撤销属性改变
class ECUndoChangeComponentProperty : public ECUndo
{
public:
	virtual void undo();
public:
	std::string mEditorName;		// 编辑体名
	std::string mComponentName;		// 组件名
	std::string mPropertyName;		// 属性名
	std::string mPropertyValue;		// 原来的属性值
};

#endif