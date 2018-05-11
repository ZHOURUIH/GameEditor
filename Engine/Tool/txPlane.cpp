#include "txPlane.h"

txPlane::txPlane(const VECTOR3& normal, const float& dis)
:
mNormal(normal),
mDis(dis)
{
	mNormal = txMath::normalize(mNormal);
}

txPlane::txPlane(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3)
{
	set3Points(v1, v2, v3);
}

txPlane::txPlane() 
:
mNormal(txMath::AXIS_Y),
mDis(0.0f)
{}

void txPlane::set3Points(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3)
{
	VECTOR3 aux1 = v1 - v2;
	VECTOR3 aux2 = v3 - v2;

	mNormal = txMath::normalize(txMath::cross(aux1, aux2));
	mDis = txMath::dot(mNormal, v2);
}

void txPlane::setCoefficients(const float& a, const float& b, const float& c, const float& d)
{
	// set the normal vector
	mNormal = VECTOR3(a, b, c);
	//compute the lenght of the vector
	float len = txMath::getLength(mNormal);
	// normalize the vector
	mNormal = txMath::normalize(mNormal);
	// and divide d by th length as well
	mDis = -d / len;
}

bool txPlane::rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& point)
{
	VECTOR3 normalizedDir = txMath::normalize(dir);
	// 射线起点在平面正面,且法线与方向夹角为钝角
	if (txMath::dot(mNormal, ori) > mDis && txMath::dot(mNormal, normalizedDir) < 0)
	{
		// 起点到交点的向量与法线向量
		float oriToPlaneDis = distance(ori);
		float cosAngle = txMath::dot(-mNormal, normalizedDir);
		float t = oriToPlaneDis / cosAngle;
		point = ori + normalizedDir * t;
		return true;
	}
	// 射线起点在平面背面,且法线与方向夹角为锐角
	if (txMath::dot(mNormal, ori) < mDis && txMath::dot(mNormal, normalizedDir) > 0)
	{
		float oriToPlaneDis = -distance(ori);
		float cosAngle = txMath::dot(mNormal, normalizedDir);
		float t = oriToPlaneDis / cosAngle;
		point = ori + normalizedDir * t;
		return true;
	}
	return false;
}