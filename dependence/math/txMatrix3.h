#ifndef _TX_MATRIX3_H_
#define _TX_MATRIX3_H_

namespace txMath
{
	class txVector3;
	class txMatrix4;
	// 列矩阵
	class txMatrix3
	{
	public:
		txMatrix3();
		txMatrix3(const txMatrix3& other);
		txMatrix3(txVector3 xAxis, txVector3 yAxis, txVector3 zAxis);
		~txMatrix3();
		txMatrix3 inverse();
		void initWithMatrix4(txMatrix4 other);
		void normalizeAxis(); // 去除矩阵中的缩放值
		txVector3 getNormalizedAxisX() const; // 得到单位化以后的X轴
		txVector3 getNormalizedAxisY() const; // 得到单位化以后的Y轴
		txVector3 getNormalizedAxisZ() const; // 得到单位化以后的Z轴
		txVector3 getScale() const; // 得到矩阵中的缩放值
		void setScale(txVector3 scale); // 设置矩阵中的缩放值
		void scale(txVector3 scale); // 设置矩阵中的缩放值为原缩放值乘以传入的缩放值
		txMatrix3 getNormalizedRotation(); // 得到矩阵中的旋转值,不包含缩放
		txVector3 getCol(int col) const;
		txVector3 getRow(int row) const;

		txMatrix3& operator *= (const txMatrix3& other);
	public:
		float m[3][3];
		// 其排列如下
		//------------------------------
		// m[0][0], m[1][0], m[2][0],
		// m[0][1], m[1][1], m[2][1],
		// m[0][2], m[1][2], m[2][2],
		//------------------------------
		// 其中m[0][0], m[0][1], m[0][2]为x轴
	};
}

#endif