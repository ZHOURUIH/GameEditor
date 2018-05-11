#include "MathHeader.h"
#include "MathUtility.h"

namespace txMath
{
	txQuaternion::txQuaternion()
	:
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
	{
		;
	}

	txQuaternion::txQuaternion(float fx, float fy, float fz, float fw)
	:
	x(fx),
	y(fy),
	z(fz),
	w(fw)
	{
		;
	}

	txQuaternion::txQuaternion(float yaw, float pitch, float roll)
	{
		float fCosHRoll = cos(roll * 0.5f);
		float fSinHRoll = sin(roll * 0.5f);
		float fCosHPitch = cos(pitch * 0.5f);
		float fSinHPitch = sin(pitch * 0.5f);
		float fCosHYaw = cos(yaw * 0.5f);
		float fSinHYaw = sin(yaw * 0.5f);

		/// Cartesian coordinate System
		//w = fCosHRoll * fCosHPitch * fCosHYaw + fSinHRoll * fSinHPitch * fSinHYaw;
		//x = fSinHRoll * fCosHPitch * fCosHYaw - fCosHRoll * fSinHPitch * fSinHYaw;
		//y = fCosHRoll * fSinHPitch * fCosHYaw + fSinHRoll * fCosHPitch * fSinHYaw;
		//z = fCosHRoll * fCosHPitch * fSinHYaw - fSinHRoll * fSinHPitch * fCosHYaw;

		w = fCosHRoll * fCosHPitch * fCosHYaw + fSinHRoll * fSinHPitch * fSinHYaw;
		x = fCosHRoll * fSinHPitch * fCosHYaw + fSinHRoll * fCosHPitch * fSinHYaw;
		y = fCosHRoll * fCosHPitch * fSinHYaw - fSinHRoll * fSinHPitch * fCosHYaw;
		z = fSinHRoll * fCosHPitch * fCosHYaw - fCosHRoll * fSinHPitch * fSinHYaw;
	}

	txQuaternion::txQuaternion(const txQuaternion& other)
	:
	x(other.x),
	y(other.y),
	z(other.z),
	w(other.w)
	{
		;
	}

	txQuaternion::txQuaternion(const txMatrix3& other)
	{
		initWithMatrix3(other);
	}

	txQuaternion::txQuaternion(const txMatrix4& other)
	{
		initWithMatrix3(other.toMatrix3());
	}

	txQuaternion::~txQuaternion()
	{
		;
	}

	void txQuaternion::initWithMatrix3(txMatrix3 mat3)
	{
		*this = matrix3ToQuaternion(mat3);
	}

	txQuaternion& txQuaternion::operator *= (const txQuaternion& other)
	{
		txQuaternion temp(*this);
		this->w = temp.w * other.w - temp.x * other.x - temp.y * other.y - temp.z * other.z;
		this->x = temp.w * other.x + temp.x * other.w + temp.y * other.z - temp.z * other.y;
		this->y = temp.w * other.y + temp.y * other.w + temp.z * other.x - temp.x * other.z;
		this->z = temp.w * other.z + temp.z * other.w + temp.x * other.y - temp.y * other.x;
		return *this;
	}

	txQuaternion& txQuaternion::operator /= (float s)
	{
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}

	txQuaternion& txQuaternion::operator *= (float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	txQuaternion& txQuaternion::operator += (const txQuaternion& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	float txQuaternion::length()
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

	void txQuaternion::normalize()
	{
		float len = length();
		if (len <= 0.0f) // Problem
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 1.0f;
		}
		else
		{
			x /= len;
			y /= len;
			z /= len;
			w /= len;
		}
	}

	float txQuaternion::getRadianYaw()
	{
		return asin(-2.0f * (x * z - w * y));
	}

	float txQuaternion::getRadianPitch()
	{
		return atan2(2.0f * (y * z + w * x), w * w - x * x - y * y + z * z);
	}

	float txQuaternion::getRadianRoll()
	{
		return atan2(2.0f * (x * y + w * z), w * w + x * x - y * y - z * z);
	}

	txQuaternion txQuaternion::cross(txQuaternion other)
	{
		return txQuaternion(
			w * other.w - x * other.x - y * other.y - z * other.z,
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y + y * other.w + z * other.x - x * other.z,
			w * other.z + z * other.w + x * other.y - y * other.x);
	}

	txQuaternion txQuaternion::conjugate()
	{
		return txQuaternion(-x, -y, -z, w);
	}

	txQuaternion txQuaternion::inverse()
	{
		return conjugate() / dot(*this, *this);
	}

	txMatrix3 txQuaternion::toMatrix3()
	{
		return quaternionToMatrix3(*this);
	}
}