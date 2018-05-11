#include "MathHeader.h"
#include "MathUtility.h"

namespace txMath
{
	txMatrix4::txMatrix4()
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i != j)
				{
					m[i][j] = 0.0f;
				}
				else
				{
					m[i][j] = 1.0f;
				}
			}
		}
	}

	txMatrix4::txMatrix4(const txMatrix4& other)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m[i][j] = other.m[i][j];
			}
		}
	}

	txMatrix4::txMatrix4(txVector4 v1, txVector4 v2, txVector4 v3, txVector4 v4)
	{
		m[0][0] = v1.x; m[1][0] = v2.x; m[2][0] = v3.x; m[3][0] = v4.x;
		m[0][1] = v1.y; m[1][1] = v2.y; m[2][1] = v3.y; m[3][1] = v4.y;
		m[0][2] = v1.z; m[1][2] = v2.z; m[2][2] = v3.z; m[3][2] = v4.z;
		m[0][3] = v1.w; m[1][3] = v2.w; m[2][3] = v3.w; m[3][3] = v4.w;
	}

	txMatrix4::~txMatrix4()
	{
		;
	}

	void txMatrix4::initWithMatrix3(txMatrix3 m3)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i < 3 && j < 3)
				{
					m[i][j] = m3.m[i][j];
				}
				else
				{
					if (i != 3 || j != 3)
					{
						m[i][j] = 0.0f;
					}
					else
					{
						m[i][j] = 1.0f;
					}
				}
			}
		}
	}

	txMatrix3 txMatrix4::toMatrix3() const
	{
		txMatrix3 temp;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				temp.m[i][j] = m[i][j];
			}
		}
		return temp;
	}

	txQuaternion txMatrix4::toQuaternion()
	{
		return matrix3ToQuaternion(toMatrix3());
	}

	txVector4 txMatrix4::getCol(int col) const
	{
		return txVector4(m[col][0], m[col][1], m[col][2], m[col][3]);
	}

	txVector4 txMatrix4::getRow(int row) const
	{
		return txVector4(m[0][row], m[1][row], m[3][row], m[3][row]);
	}

	txMatrix4 txMatrix4::inverse()
	{
		float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		txVector4 Fac0(Coef00, Coef00, Coef02, Coef03);
		txVector4 Fac1(Coef04, Coef04, Coef06, Coef07);
		txVector4 Fac2(Coef08, Coef08, Coef10, Coef11);
		txVector4 Fac3(Coef12, Coef12, Coef14, Coef15);
		txVector4 Fac4(Coef16, Coef16, Coef18, Coef19);
		txVector4 Fac5(Coef20, Coef20, Coef22, Coef23);

		txVector4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		txVector4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		txVector4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		txVector4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		txVector4 Inv0(Vec1.product(Fac0) - Vec2.product(Fac1) + Vec3.product(Fac2));
		txVector4 Inv1(Vec0.product(Fac0) - Vec2.product(Fac3) + Vec3.product(Fac4));
		txVector4 Inv2(Vec0.product(Fac1) - Vec1.product(Fac3) + Vec3.product(Fac5));
		txVector4 Inv3(Vec0.product(Fac2) - Vec1.product(Fac4) + Vec2.product(Fac5));

		txVector4 SignA(+1, -1, +1, -1);
		txVector4 SignB(-1, +1, -1, +1);
		txMatrix4 Inverse(Inv0.product(SignA), Inv1.product(SignB), Inv2.product(SignA), Inv3.product(SignB));

		txVector4 Row0(Inverse.m[0][0], Inverse.m[1][0], Inverse.m[2][0], Inverse.m[3][0]);
		txVector4 Col0(m[0][0], m[0][1], m[0][2], m[0][3]);
		txVector4 Dot0(Col0.product(Row0));
		float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);
		float OneOverDeterminant = 1.0f / Dot1;

		return Inverse * OneOverDeterminant;
	}

	txMatrix4 txMatrix4::transpose()
	{
		txMatrix4 mat4(*this);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = i + 1; j < 4; ++j)
			{
				float temp = m[i][j];
				m[i][j] = m[j][i];
				m[j][i] = temp;
			}
		}
		return mat4;
	}

	txVector3 txMatrix4::getScale()
	{
		txVector3 v1(m[0][0], m[0][1], m[0][2]);
		txVector3 v2(m[1][0], m[1][1], m[1][2]);
		txVector3 v3(m[2][0], m[2][1], m[2][2]);
		return txVector3(v1.length(), v2.length(), v3.length());
	}

	txMatrix3 txMatrix4::getRotation()
	{
		txVector3 v1(m[0][0], m[0][1], m[0][2]);
		txVector3 v2(m[1][0], m[1][1], m[1][2]);
		txVector3 v3(m[2][0], m[2][1], m[2][2]);
		v1.normalize();
		v2.normalize();
		v3.normalize();
		txMatrix3 rotation;
		rotation.m[0][0] = v1.x;
		rotation.m[0][1] = v1.y;
		rotation.m[0][2] = v1.z;
		rotation.m[1][0] = v2.x;
		rotation.m[1][1] = v2.y;
		rotation.m[1][2] = v2.z;
		rotation.m[2][0] = v3.x;
		rotation.m[2][1] = v3.y;
		rotation.m[2][2] = v3.z;
		return rotation;
	}

	txVector3 txMatrix4::getTransform()
	{
		return txVector3(m[0][3], m[1][3], m[2][3]);
	}

	void txMatrix4::setScale(txVector3 scale)
	{
		txVector3 v1(m[0][0], m[0][1], m[0][2]);
		txVector3 v2(m[1][0], m[1][1], m[1][2]);
		txVector3 v3(m[2][0], m[2][1], m[2][2]);
		v1.normalize();
		v2.normalize();
		v3.normalize();
		v1 *= scale.x;
		v2 *= scale.y;
		v3 *= scale.z;
		m[0][0] = v1.x;
		m[0][1] = v1.y;
		m[0][2] = v1.z;
		m[1][0] = v2.x;
		m[1][1] = v2.y;
		m[1][2] = v2.z;
		m[2][0] = v3.x;
		m[2][1] = v3.y;
		m[2][2] = v3.z;
	}

	void txMatrix4::setRotation(txMatrix3 rotation)
	{
		txVector3 scale = getScale();
		rotation.setScale(scale);
		m[0][0] = rotation.m[0][0];
		m[0][1] = rotation.m[0][1];
		m[0][2] = rotation.m[0][2];
		m[1][0] = rotation.m[1][0];
		m[1][1] = rotation.m[1][1];
		m[1][2] = rotation.m[1][2];
		m[2][0] = rotation.m[2][0];
		m[2][1] = rotation.m[2][1];
		m[2][2] = rotation.m[2][2];
	}

	void txMatrix4::setTransform(txVector3 transform)
	{
		m[0][3] = transform.x;
		m[1][3] = transform.y;
		m[2][3] = transform.z;
	}

	void txMatrix4::scale(txVector3 scale)
	{
		m[0][0] *= scale.x;
		m[0][1] *= scale.x;
		m[0][2] *= scale.x;
		m[1][0] *= scale.y;
		m[1][1] *= scale.y;
		m[1][2] *= scale.y;
		m[2][0] *= scale.z;
		m[2][1] *= scale.z;
		m[3][2] *= scale.z;
	}

	void txMatrix4::rotate(txMatrix3 rotation)
	{
		txMatrix3 thisRot = getRotation();
		thisRot *= rotation;
		setRotation(thisRot);
	}

	void txMatrix4::transform(txVector3 transform)
	{
		m[0][3] += transform.x;
		m[1][3] += transform.y;
		m[2][3] += transform.z;
	}

	txMatrix4& txMatrix4::operator *= (const txMatrix4& other)
	{
		*this = *this * other;
		return *this;
	}
}