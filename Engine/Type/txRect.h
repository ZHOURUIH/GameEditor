#ifndef _TX_RECT_H_
#define _TX_RECT_H_

#include "txEngineDefine.h"
#include "txMathUtility.h"

class txRect
{
public:
	txRect()
	{
		setEmpty();
	}
	txRect(const VECTOR2& min, const VECTOR2& max)
	:
	mMin(min),
	mMax(max)
	{}
	virtual ~txRect(){}
	void setEmpty()
	{
		mMin = VECTOR2(9999.0f, 9999.0f);
		mMax = VECTOR2(-9999.0f, -9999.0f);
	}
	bool isEmpty() const
	{
		return txMath::vectorLess(mMax, mMin);
	}
	void merge(const txRect& that)
	{
		// 如果自己是空的,则直接设置
		if (isEmpty())
		{
			mMin = that.mMin;
			mMax = that.mMax;
			return;
		}
		mMin.x = txMath::getMin(mMin.x, that.mMin.x);
		mMin.y = txMath::getMin(mMin.y, that.mMin.y);
		mMax.x = txMath::getMax(mMax.x, that.mMax.x);
		mMax.y = txMath::getMax(mMax.y, that.mMax.y);
	}
	bool isPointInRect(const VECTOR2& point) const
	{
		return point.x > mMin.x && point.x < mMax.x && point.y > mMin.y && point.y < mMax.y;
	}
public:
	VECTOR2 mMin;
	VECTOR2 mMax;
};

#endif