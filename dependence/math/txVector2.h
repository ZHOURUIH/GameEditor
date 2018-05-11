#ifndef _TX_VECTOR2_H_
#define _TX_VECTOR2_H_

namespace txMath
{
	class txVector2
	{
	public:
		txVector2();
		txVector2(float fx, float fy);
		txVector2(const txVector2& other);
		~txVector2();
		txVector2& operator = (const txVector2& other);
		txVector2& operator += (const txVector2& other);
		txVector2& operator -= (const txVector2& other);
		txVector2& operator *= (float scale);
		txVector2& operator /= (float scale);
		float dot(txVector2 other); // 点乘
		float length();
		void normalize(); // 单位化
		txVector2 product(txVector2 other); // 向量的各个分量分别相乘

	public:
		float x;
		float y;
	};
}

#endif