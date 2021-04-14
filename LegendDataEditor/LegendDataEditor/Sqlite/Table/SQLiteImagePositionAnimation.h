#ifndef _SQLITE_IMAGE_POSITION_ANIMATION_H_
#define _SQLITE_IMAGE_POSITION_ANIMATION_H_

#include "TDImagePositionAnimation.h"

class SQLiteImagePositionAnimation : public SQLiteTable<TDImagePositionAnimation>
{
public:
	void SQLiteImagePositionAnimation::query(const string& atlas, const string& animation, TDImagePositionAnimation& data);
	bool SQLiteImagePositionAnimation::updateData(const TDImagePositionAnimation& data);
};

#endif