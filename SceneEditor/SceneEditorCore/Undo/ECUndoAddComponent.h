#ifndef _EC_UNDO_ADD_COMPONENT_H_
#define _EC_UNDO_ADD_COMPONENT_H_

#include "ECUndo.h"

class ECUndoAddComponent : public ECUndo
{
public:
	virtual void undo();
public:
	std::string mComponentName;
	std::string mEditorName;
};

#endif