#ifndef _TX_IPOINT_H_
#define _TX_IPOINT_H_

class txIPoint
{
public:
	txIPoint()
	:
	x(0),
	y(0)
	{}
	txIPoint(const int& ix, const int& iy)
	:
	x(ix),
	y(iy)
	{}
	txIPoint(const txIPoint& that)
	:
	x(that.x),
	y(that.y)
	{}
	virtual ~txIPoint(){}
	txIPoint operator = (const txIPoint& that)
	{
		x = that.x;
		y = that.y;
		return *this;
	}
	txIPoint operator - (const txIPoint& that) const 
	{
		return txIPoint(x - that.x, y - that.y);
	}
	txIPoint operator + (const txIPoint& that) const
	{
		return txIPoint(x + that.x, y + that.y);
	}
	txIPoint operator * (const float& s) const
	{
		return txIPoint((int)(x * s), (int)(y * s));
	}
	txIPoint operator / (const float& s) const
	{
		return txIPoint((int)(x / s), (int)(y / s));
	}
	bool operator == (const txIPoint& that) const
	{
		return that.x == x && that.y == y;
	}
	bool operator != (const txIPoint& that) const
	{
		return that.x != x || that.y != y;
	}
	const txIPoint& operator+=(const txIPoint& d)
	{
		x += d.x;
		y += d.y;
		return *this;
	}
	const txIPoint& operator-=(const txIPoint& d)
	{
		x -= d.x;
		y -= d.y;
		return *this;
	}
	const txIPoint& operator*=(const float& d)
	{
		x = (int)(x * d);
		y = (int)(y * d);
		return *this;
	}
	const txIPoint& operator/=(const float& d)
	{
		x = (int)(x / d);
		y = (int)(y / d);
		return *this;
	}
public:
	int x;
	int y;
};

#endif