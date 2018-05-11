#include "txAABox.h"
#include "txPlane.h"
#include "txMathUtility.h"

VECTOR3 txAABox::getVertexP(const VECTOR3& normal) const
{
	VECTOR3 res = mMin;

	if (normal.x > 0)
		res.x = mMax.x;

	if (normal.y > 0)
		res.y = mMax.y;

	if (normal.z > 0)
		res.z = mMax.z;

	return (res);
}

VECTOR3 txAABox::getVertexN(const VECTOR3& normal) const
{
	VECTOR3 res = mMin;

	if (normal.x < 0)
		res.x = mMax.x;

	if (normal.y < 0)
		res.y = mMax.y;

	if (normal.z < 0)
		res.z = mMax.z;

	return (res);
}

void txAABox::merge(const txAABox& aabb)
{
	const VECTOR3& otherMin = aabb.getMin();
	const VECTOR3& otherMax = aabb.getMax();
	// 如果自己是空的,则直接设置为要合并的包围盒
	if (isEmpty())
	{
		setMin(otherMin);
		setMax(otherMax);
		return;
	}
	// 如果要合并的包围盒是空的,则不合并
	else if (aabb.isEmpty())
	{
		return;
	}
	// 如果都不为空,则合并
	else
	{
		if (mMin.x > otherMin.x)
		{
			mMin.x = otherMin.x;
		}
		if (mMin.y > otherMin.y)
		{
			mMin.y = otherMin.y;
		}
		if (mMin.z > otherMin.z)
		{
			mMin.z = otherMin.z;
		}
		if (mMax.x < otherMax.x)
		{
			mMax.x = otherMax.x;
		}
		if (mMax.y < otherMax.y)
		{
			mMax.y = otherMax.y;
		}
		if (mMax.z < otherMax.z)
		{
			mMax.z = otherMax.z;
		}
	}
}

VECTOR3 txAABox::getNearestCorner(const VECTOR3& dir)
{
	VECTOR3 corner[8];
	// 先放入下底面的4个顶点,从最小点开始,先放Z最小的两个点,并且在这两个点中是X最小的在前面,后面的两个点也需要先放X最小的
	corner[0] = VECTOR3(mMin.x, mMin.y, mMin.z);
	corner[1] = VECTOR3(mMax.x, mMin.y, mMin.z);
	corner[2] = VECTOR3(mMin.x, mMin.y, mMax.z);
	corner[3] = VECTOR3(mMax.x, mMin.y, mMax.z);
	// 放入上底面的4个顶点,从最小点正上方的点开始,先放Z最小的两个点,并且在这两个点中是X最小的在前面,后面的两个点也需要先放X最小的
	corner[4] = VECTOR3(mMin.x, mMax.y, mMin.z);
	corner[5] = VECTOR3(mMax.x, mMax.y, mMin.z);
	corner[6] = VECTOR3(mMin.x, mMax.y, mMax.z);
	corner[7] = VECTOR3(mMax.x, mMax.y, mMax.z);
	// 相当于将这8个点存放到二叉树上,然后进行二分查找
	int index = 0;
	if (dir.x > 0.0f)
	{
		index |= 0x00000001 << 0;
	}
	if (dir.z > 0.0f)
	{
		index |= 0x00000001 << 1;
	}
	if (dir.y > 0.0f)
	{
		index |= 0x00000001 << 2;
	}
	return corner[index];
}

bool txAABox::intersects(const txAABox& aabb, const MATRIX4& thisTransform, const MATRIX4& thatTransform, txAABox* intersectAABB)
{
	// 如果有任何一个包围盒是空的,直接返回false
	if (isEmpty() || aabb.isEmpty())
	{
		return false;
	}
	txAABox newThis, newThat;
	if (thisTransform == txMath::MAT4_IDENTITY)
	{
		newThis = *this;
	}
	else
	{
		newThis = transform(thisTransform);
	}
	if (thatTransform == txMath::MAT4_IDENTITY)
	{
		newThat = aabb;
	}
	else
	{
		newThat = aabb.transform(thatTransform);
	}

	// 如果this包围盒的最小点小于that包围盒的最大点,并且this包围盒的最大点大于that包围盒的最小点,则两个包围盒相交
	// 如果that包围盒的最小点小于this包围盒的最大点,并且that包围盒的最大点大于this包围盒的最小点,则两个包围盒相交
	bool collision = false;
	if (txMath::vectorLess(newThis.mMin, newThat.mMax) && txMath::vectorGreater(newThis.mMax, newThat.mMin)
		|| txMath::vectorLess(newThat.mMin, newThis.mMax) && txMath::vectorGreater(newThat.mMax, newThis.mMin))
	{
		collision = true;
	}
	if (collision)
	{
		//存储相交区域的大小
		if (intersectAABB != NULL)
		{
			intersectAABB->mMin.x = txMath::getMax(newThis.mMin.x, newThat.mMin.x);
			intersectAABB->mMin.y = txMath::getMax(newThis.mMin.y, newThat.mMin.y);
			intersectAABB->mMin.z = txMath::getMax(newThis.mMin.z, newThat.mMin.z);

			intersectAABB->mMax.x = txMath::getMin(newThis.mMax.x, newThat.mMax.x);
			intersectAABB->mMax.y = txMath::getMin(newThis.mMax.y, newThat.mMax.y);
			intersectAABB->mMax.z = txMath::getMin(newThis.mMax.z, newThat.mMax.z);
		}
	}
	
	return collision;
}

bool txAABox::intersects(const VECTOR3& ori, const VECTOR3& dir, const MATRIX4& thisTransform, const BOX_INTERSECT_FACE& face, const bool& needContainPoint, VECTOR3* interPoint)
{
	txAABox newThis;
	if (thisTransform == txMath::MAT4_IDENTITY)
	{
		newThis = *this;
	}
	else
	{
		newThis = transform(thisTransform);
	}
	// 需要先将射线转换到包围盒的坐标系中
	VECTOR3 center = newThis.getCenter();
	VECTOR3 newOri = ori - center;
	VECTOR3 newMin = newThis.mMin - center;
	VECTOR3 newMax = newThis.mMax - center;
	
	// 因为要区分平面的正反面,所以法线必须都朝向包围盒外面,这些都是以包围盒中心为原点,包围盒坐标系内的平面
	txPlane front(txMath::AXIS_Z, newMax.z);
	txPlane back(-txMath::AXIS_Z, -newMin.z);
	txPlane top(txMath::AXIS_Y, newMax.y);
	txPlane bottom(-txMath::AXIS_Y, -newMin.y);
	txPlane left(txMath::AXIS_X, newMax.x);
	txPlane right(-txMath::AXIS_X, -newMin.x);
	txPlane planeArray[6] = { front, back, top, bottom, left, right };
	for (int i = 0; i < 6; ++i)
	{
		// 分开写减少计算量
		if (face == BIF_BOTH)
		{
			VECTOR3 ret;
			if (planeArray[i].rayIntersect(newOri, dir, ret) && (containPoint(ret) || !needContainPoint))
			{
				if (interPoint != NULL)
				{
					*interPoint = ret + center;
				}
				return true;
			}
		}
		else
		{
			bool faceToPoint = planeArray[i].isFacePoint(newOri);
			// 如果只检测正面,并且当前平面就是正面,或者只检测背面,并且当前平面就是背面
			if (face == BIF_FRONT && faceToPoint || face == BIF_BACK && !faceToPoint)
			{
				VECTOR3 ret;
				if (planeArray[i].rayIntersect(newOri, dir, ret) && (containPoint(ret) || !needContainPoint))
				{
					if (interPoint != NULL)
					{
						*interPoint = ret + center;
					}
					return true;
				}
			}
		}
	}
	return false;
}

txAABox txAABox::transform(const MATRIX4& transMat) const
{
	// 如果包围盒是空的,则不进行变换
	if (isEmpty())
	{
		return *this;
	}

	// 如果变换矩阵是单位矩阵,则不进行变换
	if (transMat == txMath::MAT4_IDENTITY)
	{
		return *this;
	}

	txAABox newAABB;
	// 包围盒8个点都需要经过矩阵变换,然后取出其中最大的点和最小的点
	// 底面的4个点,从最小点开始顺时针列出
	VECTOR3 oldMin0 = mMin;
	VECTOR3 oldMin1 = VECTOR3(mMax.x, mMin.y, mMin.z);
	VECTOR3 oldMin2 = VECTOR3(mMax.x, mMin.y, mMax.z);
	VECTOR3 oldMin3 = VECTOR3(mMin.x, mMin.y, mMax.z);
	VECTOR3 oldMax0 = mMax;
	VECTOR3 oldMax1 = VECTOR3(mMax.x, mMax.y, mMax.z);
	VECTOR3 oldMax2 = VECTOR3(mMin.x, mMax.y, mMin.z);
	VECTOR3 oldMax3 = VECTOR3(mMax.x, mMax.y, mMin.z);
	VECTOR3 vertexArray[8] = { oldMin0, oldMin1, oldMin2, oldMin3, oldMax0, oldMax1, oldMax2, oldMax3 };
	VECTOR3 newVertexArray[8];
	for (int i = 0; i < 8; ++i)
	{
		newVertexArray[i] = txMath::transformVector3(vertexArray[i], transMat);
	}
	// 先使用第0个顶点初始化,然后再跟后面的顶点作比较
	float minx = newVertexArray[0].x;
	float miny = newVertexArray[0].y;
	float minz = newVertexArray[0].z;
	float maxx = newVertexArray[0].x;
	float maxy = newVertexArray[0].y;
	float maxz = newVertexArray[0].z;
	for (int i = 1; i < 8; ++i)
	{
		minx = newVertexArray[i].x < minx ? newVertexArray[i].x : minx;
		miny = newVertexArray[i].y < miny ? newVertexArray[i].y : miny;
		minz = newVertexArray[i].z < minz ? newVertexArray[i].z : minz;
		maxx = newVertexArray[i].x > maxx ? newVertexArray[i].x : maxx;
		maxy = newVertexArray[i].y > maxy ? newVertexArray[i].y : maxy;
		maxz = newVertexArray[i].z > maxz ? newVertexArray[i].z : maxz;
	}
	
	newAABB.setMin(VECTOR3(minx, miny, minz));
	newAABB.setMax(VECTOR3(maxx, maxy, maxz));
	return newAABB;
}

txAABox txAABox::expand(const VECTOR3& expandVec) const
{
	// 最大点和最小点可以看成是坐标系的6个方向
	// 最大点的三个分量分别是X,Y,Z轴正方向
	// 最小点的三个分量分别是X,Y,Z轴负方向
	// 然后只需要根据向量来将这6个方向延伸即可

	txAABox newAABB(mMin, mMax);
	// X轴
	// X方向上的延伸量为负,则表示将最小点再往X轴负方向延伸
	if (expandVec.x < 0.0f)
	{
		newAABB.mMin.x += expandVec.x;
	}
	// X方向上的延伸量为正,则表示将最大点再往X轴正方向延伸
	else
	{
		newAABB.mMax.x += expandVec.x;
	}
	// Y轴
	// Y方向上的延伸量为负,则表示将最小点再往Y轴负方向延伸
	if (expandVec.y < 0.0f)
	{
		newAABB.mMin.y += expandVec.y;
	}
	// Y方向上的延伸量为正,则表示将最大点再往Y轴正方向延伸
	else
	{
		newAABB.mMax.y += expandVec.y;
	}
	// Z轴
	// Z方向上的延伸量为负,则表示将最小点再往Z轴负方向延伸
	if (expandVec.z < 0.0f)
	{
		newAABB.mMin.z += expandVec.z;
	}
	// Z方向上的延伸量为正,则表示将最大点再往Z轴正方向延伸
	else
	{
		newAABB.mMax.z += expandVec.z;
	}
	return newAABB;
}