#include "MathHeader.h"

namespace txMath
{
	txMatrix3::txMatrix3()
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
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

	txMatrix3::txMatrix3(const txMatrix3& other)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				m[i][j] = other.m[i][j];
			}
		}
	}

	txMatrix3::txMatrix3(txVector3 xAxis, txVector3 yAxis, txVector3 zAxis)
	{
		m[0][0] = xAxis.x; m[1][0] = yAxis.x; m[2][0] = zAxis.x;
		m[0][1] = xAxis.y; m[1][1] = yAxis.y; m[2][1] = zAxis.y;
		m[0][2] = xAxis.z; m[1][2] = yAxis.z; m[2][2] = zAxis.z;
	}

	txMatrix3::~txMatrix3()
	{
		;
	}

	txVector3 txMatrix3::getCol(int col) const
	{
		return txVector3(m[col][0], m[col][1], m[col][2]);
	}

	txVector3 txMatrix3::getRow(int row) const
	{
		return txVector3(m[0][row], m[1][row], m[2][row]);
	}

	txMatrix3 txMatrix3::inverse()
	{
		float OneOverDeterminant = 1.0f / (
			+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
			- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
			+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

		txMatrix3 Inverse;
		Inverse.m[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * OneOverDeterminant;
		Inverse.m[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * OneOverDeterminant;
		Inverse.m[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * OneOverDeterminant;
		Inverse.m[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * OneOverDeterminant;
		Inverse.m[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * OneOverDeterminant;
		Inverse.m[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * OneOverDeterminant;
		Inverse.m[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * OneOverDeterminant;
		Inverse.m[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * OneOverDeterminant;
		Inverse.m[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * OneOverDeterminant;

		return Inverse;
	}

	void txMatrix3::initWithMatrix4(txMatrix4 other)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				m[i][j] = other.m[i][j];
			}
		}
	}

	void txMatrix3::normalizeAxis()
	{
		txVector3 v1 = getCol(0);
		txVector3 v2 = getCol(1);
		txVector3 v3 = getCol(2);
		v1.normalize();
		v2.normalize();
		v3.normalize();
		*this = txMatrix3(v1, v2, v3);
	}

	txVector3 txMatrix3::getScale() const
	{
		txVector3 v1 = getCol(0);
		txVector3 v2 = getCol(1);
		txVector3 v3 = getCol(2);
		return txVector3(v1.length(), v2.length(), v3.length());
	}

	void txMatrix3::setScale(txVector3 scale)
	{
		normalizeAxis();
		this->scale(scale);
	}

	void txMatrix3::scale(txVector3 scale)
	{
		m[0][0] *= scale.x;
		m[0][1] *= scale.x;
		m[0][2] *= scale.x;
		m[1][0] *= scale.y;
		m[1][1] *= scale.y;
		m[1][2] *= scale.y;
		m[2][0] *= scale.z;
		m[2][1] *= scale.z;
		m[2][2] *= scale.z;
	}

	txMatrix3& txMatrix3::operator *= (const txMatrix3& other)
	{
		txVector3 thisRow0 = getRow(0);
		txVector3 thisRow1 = getRow(1);
		txVector3 thisRow2 = getRow(2);
		txVector3 otherCol0 = other.getCol(0);
		txVector3 otherCol1 = other.getCol(1);
		txVector3 otherCol2 = other.getCol(2);

		m[0][0] = thisRow0.dot(otherCol0);
		m[0][1] = thisRow1.dot(otherCol0);
		m[0][2] = thisRow2.dot(otherCol0);
		m[1][0] = thisRow0.dot(otherCol1);
		m[1][1] = thisRow1.dot(otherCol1);
		m[1][2] = thisRow2.dot(otherCol1);
		m[2][0] = thisRow0.dot(otherCol2);
		m[2][1] = thisRow1.dot(otherCol2);
		m[2][2] = thisRow2.dot(otherCol2);
		return *this;
	}
}