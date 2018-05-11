#ifndef _TX_PLANE_H_
#define _TX_PLANE_H_

#include "txMathUtility.h"

class txPlane
{
public:
	// 平面法线
	VECTOR3 mNormal;
	// 平面朝法线方向平移的距离
	float mDis;
public:
	txPlane(const VECTOR3& normal, const float& dis);
	txPlane(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3);
	txPlane(void);
	virtual ~txPlane(){}

	// 顺时针为正面
	void set3Points(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3&v3);
	void setNormalAndPoint(const VECTOR3& normal, const VECTOR3& point)
	{
		mNormal = txMath::normalize(normal);
		mDis = txMath::dot(mNormal, point);
	}
	void setCoefficients(const float& a, const float& b, const float& c, const float& d);
	float distance(const VECTOR3& p)
	{
		return txMath::dot(mNormal, p) - mDis;
	}
	// 法线是否朝向指定点
	bool isFacePoint(const VECTOR3& point)
	{
		return distance(point) > 0.0f;
	}
	// 平面与射线作相交检测
	bool rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& point);
};

#endif