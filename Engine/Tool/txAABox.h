#ifndef _TX_AABOX_H_
#define _TX_AABOX_H_

#include "txEngineDefine.h"
#include "Utility.h"

class txPlane;
class txAABox
{
public:
	txAABox()
	{
		setEmpty();
	}
	virtual ~txAABox(){}
	txAABox(const txAABox& other)
		:
		mMin(other.mMin),
		mMax(other.mMax)
	{}
	txAABox(const VECTOR3& vMin, const VECTOR3& vMax)
	{
		setMin(vMin);
		setMax(vMax);
	}
	
	txAABox(const VECTOR3& corner, const float& x, const float& y, const float& z)
	{
		mMin = corner;
		mMax = mMin + VECTOR3(x, y, z);
	}

	txAABox& operator=(const txAABox& that)
	{
		mMin = that.mMin;
		mMax = that.mMax;
		return *this;
	}

	VECTOR3 getCenter()const{ return (mMin + mMax) / 2.0f; }

	// for use in frustum computations
	VECTOR3 getVertexP(const VECTOR3& normal) const;
	VECTOR3 getVertexN(const VECTOR3& normal) const;

	void setMin(const VECTOR3& min){ mMin = min; }
	const VECTOR3& getMin() const { return mMin; }

	void setMax(const VECTOR3& max){ mMax = max; }
	const VECTOR3& getMax() const { return mMax; }

	void setEmpty()
	{
		mMin = VECTOR3(99999.0f, 99999.0f, 99999.0f);
		mMax = VECTOR3(-99999.0f, -99999.0f, -99999.0f);
	}
	bool isEmpty() const { return mMin.x > mMax.x || mMin.y > mMax.y || mMin.z > mMax.z; }
	// 将aabb合并到当前包围盒
	void merge(const txAABox& aabb);
	// 得到从中心发出一条指定方向的射线,与所有从顶点到中心的连线夹角最小的顶点
	VECTOR3 getNearestCorner(const VECTOR3& dir);
	// 检测两个碰撞盒是否有接触
	bool intersects(const txAABox& aabb, const MATRIX4& thisTransform, const MATRIX4& thatTransform, txAABox* intersectAABB = NULL);
	// 检测射线是否与包围盒相交,face是指定与正面检测还是与背面检测,needContainPoint指定交点是否必须在包围盒上才算相交,interPlane是返回的相交平面
	bool intersects(const VECTOR3& ori, const VECTOR3& dir, const MATRIX4& thisTransform, const BOX_INTERSECT_FACE& face = BIF_FRONT, const bool& needContainPoint = true, VECTOR3* interPoint = NULL);
	// 检测点是否在碰撞盒中
	bool containPoint(const VECTOR3& point) const
	{
		if (point.x < mMin.x || point.y < mMin.y || point.z < mMin.z ||
			point.x > mMax.x || point.y > mMax.y || point.z > mMax.z)
		{
			return false;
		}
		return true;
	}
	// 使用矩阵来变换包围盒
	txAABox transform(const MATRIX4& transMat) const;
	// 往某个方向延伸包围盒,可以是任意向量
	txAABox expand(const VECTOR3& expandVec) const;
	// 得到包围盒上的点中心最远的距离
	float getMaxCornerDis() const
	{
		VECTOR3 corner = mMax - getCenter();
		return txMath::getMax(corner.x, (txMath::getMax(corner.y, corner.z)));
	}
protected:
	VECTOR3 mMin;
	VECTOR3 mMax;
};

#endif