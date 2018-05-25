#ifndef _EC_UNDO_LAYOUT_VISIBLE_H_
#define _EC_UNDO_LAYOUT_VISIBLE_H_

#include "ECUndo.h"

class Layout;
class ECUndoLayoutVisible : public ECUndo
{
public:
	ECUndoLayoutVisible()
		:
		mLayout(NULL),
		mVisible(true)
	{}
	virtual ~ECUndoLayoutVisible(){}
	void setLayout(Layout* layout){ mLayout = layout; }
	void setVisible(bool visible){ mVisible = visible; }
	virtual void undo();
protected:
	Layout* mLayout;
	bool mVisible;
};

#endif