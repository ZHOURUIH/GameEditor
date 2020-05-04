#ifndef _VECTOR2_I_H_
#define _VECTOR2_I_H_

struct Vector2i
{
public:
	int x;
	int y;
public:
	Vector2i()
	{
		x = 0;
		y = 0;
	}
	Vector2i(int xx, int yy)
	{
		x = xx;
		y = yy;
	}
	Vector2i operator*(int that)const
	{
		return Vector2i(x * that, y * that);
	}
	Vector2i operator/(int that)const
	{
		return Vector2i(x / that, y / that);
	}
	Vector2i operator+(const Vector2i& that)const
	{
		return Vector2i(x + that.x, y + that.y);
	}
	Vector2i operator-(const Vector2i& that)const
	{
		return Vector2i(x - that.x, y - that.y);
	}
	Vector2i& operator+=(const Vector2i& that)
	{
		x -= that.x;
		y -= that.y;
		return *this;
	}
	Vector2i& operator-=(const Vector2i& that)
	{
		x -= that.x;
		y -= that.y;
		return *this;
	}
};

#endif