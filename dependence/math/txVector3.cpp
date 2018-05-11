#include "MathHeader.h"

namespace txMath
{
	txVector3::txVector3()
		:
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{
		;
	}

	txVector3::txVector3(float fx, float fy, float fz)
		:
		x(fx),
		y(fy),
		z(fz)
	{
		;
	}

	txVector3::txVector3(const txVector3& other)
		:
		x(other.x),
		y(other.y),
		z(other.z)
	{
		;
	}

	txVector3::~txVector3()
	{
		;
	}

	txVector3& txVector3::operator = (const txVector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	txVector3& txVector3::operator += (const txVector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	txVector3& txVector3::operator -= (const txVector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	txVector3& txVector3::operator *= (float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	txVector3& txVector3::operator /= (float scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
		return *this;
	}

	float txVector3::dot(txVector3 other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	txVector3 txVector3::cross(txVector3 other) const
	{
		return txVector3(y * other.z - other.y * z, z * other.x - other.z * x, x * other.y - other.x * y);
	}

	float txVector3::length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	void txVector3::normalize()
	{
		float len = length();
		x /= len;
		y /= len;
		z /= len;
	}

	txVector3 txVector3::product(txVector3 other) const
	{
		return txVector3(x * other.x, y * other.y, z * other.z);
	}
}