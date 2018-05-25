#ifndef _EC_UNDO_H_
#define _EC_UNDO_H_

#include "txEngineBase.h"
#include "txEngineDefine.h"

#include "EditorCoreBase.h"

class ECUndo : public EditorCoreBase, public txEngineBase
{
public:
	ECUndo(){}
	virtual ~ECUndo(){}
	virtual void undo() = 0;
protected:
};

#endif