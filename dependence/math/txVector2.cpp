#include "MathHeader.h"

namespace txMath
{
	txVector2::txVector2()
		:
		x(0.0f),
		y(0.0f)
	{
		;
	}

	txVector2::txVector2(float fx, float fy)
		:
		x(fx),
		y(fy)
	{
		;
	}

	txVector2::txVector2(const txVector2& other)
		:
		x(other.x),
		y(other.y)
	{
		;
	}

	txVector2::~txVector2()
	{
		;
	}

	txVector2& txVector2::operator = (const txVector2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	txVector2& txVector2::operator += (const txVector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	txVector2& txVector2::operator -= (const txVector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	txVector2& txVector2::operator *= (float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	txVector2& txVector2::operator /= (float scale)
	{
		x /= scale;
		y /= scale;
		return *this;
	}

	float txVector2::dot(txVector2 other)
	{
		return x * other.x + y * other.y;
	}

	float txVector2::length()
	{
		return sqrt(x * x + y * y);
	}

	void txVector2::normalize()
	{
		float len = length();
		x /= len;
		y /= len;
	}

	txVector2 txVector2::product(txVector2 other)
	{
		return txVector2(x * other.x, y * other.y);
	}
}