#ifndef _TX_POINT_H_
#define _TX_POINT_H_

#include "txDim.h"

class txPoint
{
public:
	txPoint(){}
	txPoint(const txDim& _x, const txDim& _y)
    {
        x = _x;
        y = _y;
    }
	virtual ~txPoint(){}
	txPoint operator+(const txPoint& that) const
	{
		return txPoint(x + that.x, y + that.y);
	}
	txPoint operator-(const txPoint& that) const
	{
		return txPoint(x - that.x, y - that.y);
	}
	txPoint operator*(const float& z) const
	{
		return txPoint(x * z, y * z);
	}
	txPoint operator/(const float& value) const
	{
		return txPoint(x / value, y / value);
	}
	txPoint operator+=(const txPoint& that)
	{
		x += that.x;
		y += that.y;
		return *this;
	}
	txPoint operator-=(const txPoint& that)
	{
		x -= that.x;
		y -= that.y;
		return *this;
	}
	txPoint operator*=(const float& z)
	{
		x *= z;
		y *= z;
		return *this;
	}
public:
    txDim x;
    txDim y;
	static txPoint ZERO;
	static txPoint IDENTITY;
};

#endif