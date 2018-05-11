#ifndef _TX_MATRIX4_H_
#define _TX_MATRIX4_H_

namespace txMath
{
	class txVector3;
	class txMatrix3;
	class txVector4;
	class txQuaternion;
	// 列矩阵
	class txMatrix4
	{
	public:
		txMatrix4();
		txMatrix4(const txMatrix4& other);
		txMatrix4(txVector4 v1, txVector4 v2, txVector4 v3, txVector4 v4);
		~txMatrix4();
		void initWithMatrix3(txMatrix3 other);
		txMatrix3 toMatrix3() const; // 转化为3阶矩阵
		txQuaternion toQuaternion(); // 转化为四元数
		txVector4 getCol(int col) const;
		txVector4 getRow(int row) const;
		txMatrix4 inverse(); // 求逆矩阵
		txMatrix4 transpose(); // 求转置矩阵
		txVector3 getScale(); // 得到矩阵中的缩放值
		txMatrix3 getRotation(); // 得到矩阵中的旋转值
		txVector3 getTransform(); // 得到矩阵中的平移值
		void setScale(txVector3 scale); // 设置缩放值,不改变旋转值和平移值
		void setRotation(txMatrix3 rotation); // 设置旋转值,不改变缩放值和平移值
		void setTransform(txVector3 transform); // 设置平移值,不改变缩放值和旋转值
		void scale(txVector3 scale); // 在原来缩放的基础上再次缩放
		void rotate(txMatrix3 rotation); // 在原来旋转的基础上再次旋转
		void transform(txVector3 transform); // 在原来平移的基础上再次平移

		txMatrix4& operator *= (const txMatrix4& other);
	public:
		float m[4][4];
		// 其排列如下
		//------------------------------------
		// m[0][0], m[1][0], m[2][0], m[3][0],
		// m[0][1], m[1][1], m[2][1], m[3][1],
		// m[0][2], m[1][2], m[2][2], m[3][2],
		// m[0][3], m[1][3], m[2][3], m[3][3],
		//------------------------------------
		// 其中m[0][3], m[1][3], m[2][3]为位移
	};
}

#endif