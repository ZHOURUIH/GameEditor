#include "txVector4.h"
#include "txVector3.h"
#include "MathUtility.h"
#include <math.h>

namespace txMath
{
	txVector4::txVector4()
		:
		x(0.0f),
		y(0.0f),
		z(0.0f),
		w(0.0f)
	{
		;
	}

	txVector4::txVector4(float fx, float fy, float fz, float fw)
		:
		x(fx),
		y(fy),
		z(fz),
		w(fw)
	{
		;
	}

	txVector4::txVector4(const txVector3& other, float fw)
		:
		x(other.x),
		y(other.y),
		z(other.z),
		w(fw)
	{
		;
	}

	txVector4::txVector4(const txVector4& other)
		:
		x(other.x),
		y(other.y),
		z(other.z),
		w(other.w)
	{
		;
	}

	txVector4::~txVector4()
	{
		;
	}

	txVector3 txVector4::toWVector3() const
	{
		if (isFloatZero(w))
		{
			return txVector3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			return txVector3(x / w, y / w, z / w);
		}
	}

	txVector3 txVector4::toDirectVector3() const
	{
		return txVector3(x, y, z);
	}

	float txVector4::dot(txVector4 other)
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	txVector4 txVector4::product(txVector4 other)
	{
		return txVector4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	float txVector4::length() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

	void txVector4::normalize()
	{
		float len = length();
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}

	txVector4& txVector4::operator = (const txVector4& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	txVector4& txVector4::operator = (const txVector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = 0.0f;
		return *this;
	}

	txVector4& txVector4::operator += (const txVector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	txVector4& txVector4::operator -= (const txVector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	txVector4& txVector4::operator *= (float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;
		return *this;
	}

	txVector4& txVector4::operator /= (float scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
		w /= scale;
		return *this;
	}

	txVector4 txVector4::operator * (float scale)
	{
		return txVector4(x * scale, y * scale, z * scale, w * scale);
	}

	txVector4 txVector4::operator / (float scale)
	{
		return txVector4(x / scale, y / scale, z / scale, w / scale);
	}
}