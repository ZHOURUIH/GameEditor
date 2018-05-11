#ifndef _TX_VECTOR4_H_
#define _TX_VECTOR4_H_

namespace txMath
{
	class txVector3;
	class txVector4
	{
	public:
		txVector4();
		txVector4(float fx, float fy, float fz, float fw);
		txVector4(const txVector3& other, float fw);
		txVector4(const txVector4& other);
		~txVector4();
		txVector3 toWVector3() const; // 齐次坐标转化为三维坐标
		txVector3 toDirectVector3() const; // 直接获得前三个分量
		float dot(txVector4 other); // 点乘
		txVector4 product(txVector4 other); // 4个分量分别相乘
		float length() const;
		void normalize(); // 单位化
		txVector4& operator = (const txVector4& other);
		txVector4& operator = (const txVector3& other);
		txVector4& operator += (const txVector4& other);
		txVector4& operator -= (const txVector4& other);
		txVector4& operator *= (float scale);
		txVector4& operator /= (float scale);
		txVector4 operator * (float scale);
		txVector4 operator / (float scale);
	public:
		float x;
		float y;
		float z;
		float w;
	};
}

#endif