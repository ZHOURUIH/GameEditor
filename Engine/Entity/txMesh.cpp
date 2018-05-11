#include "txMesh.h"
#include "txSkin.h"
#include "txSkeleton.h"
#include "txMaterial.h"
#include "txSubMesh.h"

txMesh::txMesh()
:
pVBuf(NULL),
pNBuf(NULL),
pTBuf(NULL),
VSize(0),
ISize(0),
NSize(0),
TSize(0),
nNumTris(0),
isSkeletonMesh(false),
mAABBVertexBufferSize(0),
mAABBIndexBufferSize(0),
mAABBBufferV(NULL),
mAABBBufferN(NULL),
m3DMaxAABBVertexBufferSize(0),
m3DMaxAABBIndexBufferSize(0),
m3DMaxAABBBufferV(NULL),
m3DMaxAABBBufferN(NULL)
{
	m_pAABox = TRACE_NEW(txAABox, m_pAABox);
	m3DMaxAABB = TRACE_NEW(txAABox, m3DMaxAABB);
}

txMesh::~txMesh()
{
	TRACE_DELETE_ARRAY(pVBuf);
	TRACE_DELETE_ARRAY(pNBuf);
	TRACE_DELETE_ARRAY(pTBuf);
	TRACE_DELETE(m_pAABox);
	TRACE_DELETE_ARRAY(mAABBBufferV);
	mAABBVertexBufferSize = 0;
	TRACE_DELETE_ARRAY(mAABBBufferN);
	mAABBIndexBufferSize = 0;
	TRACE_DELETE(m3DMaxAABB);
	TRACE_DELETE_ARRAY(m3DMaxAABBBufferV);
	m3DMaxAABBVertexBufferSize = 0;
	TRACE_DELETE_ARRAY(m3DMaxAABBBufferN);
	m3DMaxAABBIndexBufferSize = 0;

	// 释放子模型
	int subMeshCount = mSubMeshes.size();
	FOR_STL(mSubMeshes, int i = 0; i < subMeshCount; ++i)
	{
		TRACE_DELETE(mSubMeshes[i]);
	}
	END_FOR_STL(mSubMeshes);
	mSubMeshes.clear();
}

void txMesh::CopyMesh(txMesh* pMesh)
{
	// 拷贝顶点数据
	pMesh->MakeVertexBuffer(VSize);
	pMesh->MakeNormalBuffer(NSize);
	pMesh->MakeTexCoordBuffer(TSize);
	memcpy(pMesh->GetVertexBuffer(), pVBuf, VSize * sizeof(GLfloat));
	memcpy(pMesh->GetNormalBuffer(), pNBuf, NSize * sizeof(GLfloat));
	memcpy(pMesh->GetTexCoordBuffer(), pTBuf, TSize * sizeof(GLfloat));

	// 设置三角形数量
	pMesh->SetNumTris(nNumTris);

	// 设置包围盒
	txAABox pAABB;
	pAABB.setMin(m_pAABox->getMin());
	pAABB.setMax(m_pAABox->getMax());
	pMesh->SetAABox(pAABB);

	// 创建子模型
	int subMeshCount = mSubMeshes.size();
	FOR_STL(mSubMeshes, int i = 0; i < subMeshCount; ++i)
	{
		txSubMesh* pSubMesh = pMesh->CreateSubMesh();
		mSubMeshes[i]->CopySubMesh(pSubMesh);
	}
	END_FOR_STL(mSubMeshes);
	pMesh->isSkeletonMesh = isSkeletonMesh;
}

txVector<txMaterial*> txMesh::getAllMaterials()
{
	txVector<txMaterial*> materialList;
	int subMeshCount = mSubMeshes.size();
	FOR_STL(mSubMeshes, int i = 0; i < subMeshCount; ++i)
	{
		materialList.push_back(mSubMeshes[i]->getMaterial());
	}
	END_FOR_STL(mSubMeshes);
	return materialList;
}

void txMesh::GenerateTextures()
{
	int subMeshCount = mSubMeshes.size();
	FOR_STL(mSubMeshes, int i = 0; i < subMeshCount; ++i)
	{
		mSubMeshes[i]->GenerateTexture();
	}
	END_FOR_STL(mSubMeshes);
}

void txMesh::MakeVertexBuffer(const int& len)
{
	if (pVBuf != NULL)
	{
		return;
	}
	VSize = len;
	pVBuf = TRACE_NEW_ARRAY(GLfloat, VSize, pVBuf);
}

void txMesh::MakeNormalBuffer(const int& len)
{
	if (pNBuf != NULL)
	{
		return;
	}
	NSize = len;
	pNBuf = TRACE_NEW_ARRAY(GLfloat, NSize, pNBuf);
}

void txMesh::MakeTexCoordBuffer(const int& len)
{
	if (pTBuf != NULL)
	{
		return;
	}
	TSize = len;
	pTBuf = TRACE_NEW_ARRAY(GLfloat, TSize, pTBuf);
}

bool txMesh::rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint, const MATRIX4& aabbTransform)
{
	// 首先用包围盒作碰撞检测,如果没有与包围盒碰撞,则直接返回false
	if (m_pAABox == NULL)
	{
		return false;
	}
	bool ret = false;
	if (m_pAABox->intersects(ori, dir, aabbTransform))
	{
		VECTOR3 point;
		int subMeshCount = mSubMeshes.size();
		FOR_STL(mSubMeshes, int i = 0; i < subMeshCount; ++i)
		{
			if (mSubMeshes[i]->rayIntersect(ori, dir, point))
			{
				// 只保存距离最近的相交点
				if (!ret || txMath::getLength(point - ori) < txMath::getLength(intersectPoint - ori))
				{
					intersectPoint = point;
				}
				ret = true;
			}
		}
		END_FOR_STL(mSubMeshes);
	}
	return ret;
}

void txMesh::SetAABox(const txAABox& pAABox)
{
	*m_pAABox = pAABox;
	createAABBRenderBuffer(*m_pAABox);

	// 计算调整后的包围盒
	const VECTOR3& minPos = m_pAABox->getMin();
	VECTOR3 newMinPos = minPos;
	float temp = minPos.z;
	newMinPos.z = minPos.y;
	newMinPos.y = temp;
	newMinPos.x = -minPos.x;
	newMinPos.y = -newMinPos.y;

	const VECTOR3& maxPos = m_pAABox->getMax();
	VECTOR3 newMaxPos = maxPos;
	temp = maxPos.z;
	newMaxPos.z = maxPos.y;
	newMaxPos.y = temp;
	newMaxPos.x = -newMaxPos.x;
	newMaxPos.y = -newMaxPos.y;

	VECTOR3 newMin;
	newMin.x = newMinPos.x < newMaxPos.x ? newMinPos.x : newMaxPos.x;
	newMin.y = newMinPos.y < newMaxPos.y ? newMinPos.y : newMaxPos.y;
	newMin.z = newMinPos.z < newMaxPos.z ? newMinPos.z : newMaxPos.z;

	VECTOR3 newMax;
	newMax.x = newMaxPos.x > newMinPos.x ? newMaxPos.x : newMinPos.x;
	newMax.y = newMaxPos.y > newMinPos.y ? newMaxPos.y : newMinPos.y;
	newMax.z = newMaxPos.z > newMinPos.z ? newMaxPos.z : newMinPos.z;

	m3DMaxAABB->setMin(newMin);
	m3DMaxAABB->setMax(newMax);

	create3DMaxAABBRenderBuffer(*m3DMaxAABB);
}

txSubMesh* txMesh::CreateSubMesh()
{
	txSubMesh* pSubMesh = TRACE_NEW(txSubMesh, pSubMesh, this);
	mSubMeshes.push_back(pSubMesh);
	return pSubMesh;
}

txSubMesh* txMesh::getSubMesh(const int& nIndex)
{
	if (!txMath::isInRange(nIndex, 0, (int)mSubMeshes.size() - 1))
	{
		return NULL;
	}
	return mSubMeshes[nIndex];
}

txSubMesh* txMesh::getMaterialSubMesh(const int& nMaterialIndex)
{
	txSubMesh* subMesh = NULL;
	int subMeshCount = mSubMeshes.size();
	FOR_STL(mSubMeshes, int i = 0; i < subMeshCount; ++i)
	{
		if (mSubMeshes[i]->getMaterialIndex() == nMaterialIndex)
		{
			subMesh = mSubMeshes[i];
			break;
		}
	}
	END_FOR_STL(mSubMeshes);
	return subMesh;
}

void txMesh::setAutoDestroyTexture(const bool& aotuDestroy)
{
	int subMeshCount = mSubMeshes.size();
	FOR_STL(mSubMeshes, int i = 0; i < subMeshCount; ++i)
	{
		mSubMeshes[i]->setAutoDestroyTexture(aotuDestroy);
	}
	END_FOR_STL(mSubMeshes);
}

void txMesh::createAABBRenderBuffer(const txAABox& aabb)
{
	TRACE_DELETE_ARRAY(mAABBBufferV);
	TRACE_DELETE_ARRAY(mAABBBufferN);

	// 顶点数据
	mAABBVertexBufferSize = 8 * 3;
	mAABBBufferV = TRACE_NEW_ARRAY(GLfloat, mAABBVertexBufferSize, mAABBBufferV);
	int vertexIndex = 0;
	const VECTOR3& aabbMin = aabb.getMin();
	const VECTOR3& aabbMax = aabb.getMax();
	// 下底面,从最小点开始,顺时针
	mAABBBufferV[vertexIndex * 3 + 0] = aabbMin.x;
	mAABBBufferV[vertexIndex * 3 + 1] = aabbMin.y;
	mAABBBufferV[vertexIndex * 3 + 2] = aabbMin.z;
	++vertexIndex;

	mAABBBufferV[vertexIndex * 3 + 0] = aabbMax.x;
	mAABBBufferV[vertexIndex * 3 + 1] = aabbMin.y;
	mAABBBufferV[vertexIndex * 3 + 2] = aabbMin.z;
	++vertexIndex;

	mAABBBufferV[vertexIndex * 3 + 0] = aabbMax.x;
	mAABBBufferV[vertexIndex * 3 + 1] = aabbMin.y;
	mAABBBufferV[vertexIndex * 3 + 2] = aabbMax.z;
	++vertexIndex;

	mAABBBufferV[vertexIndex * 3 + 0] = aabbMin.x;
	mAABBBufferV[vertexIndex * 3 + 1] = aabbMin.y;
	mAABBBufferV[vertexIndex * 3 + 2] = aabbMax.z;
	++vertexIndex;

	// 上底面,从最大点开始,顺时针
	mAABBBufferV[vertexIndex * 3 + 0] = aabbMax.x;
	mAABBBufferV[vertexIndex * 3 + 1] = aabbMax.y;
	mAABBBufferV[vertexIndex * 3 + 2] = aabbMax.z;
	++vertexIndex;

	mAABBBufferV[vertexIndex * 3 + 0] = aabbMin.x;
	mAABBBufferV[vertexIndex * 3 + 1] = aabbMax.y;
	mAABBBufferV[vertexIndex * 3 + 2] = aabbMax.z;
	++vertexIndex;

	mAABBBufferV[vertexIndex * 3 + 0] = aabbMin.x;
	mAABBBufferV[vertexIndex * 3 + 1] = aabbMax.y;
	mAABBBufferV[vertexIndex * 3 + 2] = aabbMin.z;
	++vertexIndex;

	mAABBBufferV[vertexIndex * 3 + 0] = aabbMax.x;
	mAABBBufferV[vertexIndex * 3 + 1] = aabbMax.y;
	mAABBBufferV[vertexIndex * 3 + 2] = aabbMin.z;
	++vertexIndex;

	// 索引数据
	mAABBIndexBufferSize = 12 * 2;
	mAABBBufferN = TRACE_NEW_ARRAY(GLushort, mAABBIndexBufferSize, mAABBBufferN);

	int lineIndex = 0;
	// 上底面的4条线,以最大点为起点,顺时针
	mAABBBufferN[lineIndex * 2 + 0] = 4;
	mAABBBufferN[lineIndex * 2 + 1] = 5;
	++lineIndex;

	mAABBBufferN[lineIndex * 2 + 0] = 5;
	mAABBBufferN[lineIndex * 2 + 1] = 6;
	++lineIndex;

	mAABBBufferN[lineIndex * 2 + 0] = 6;
	mAABBBufferN[lineIndex * 2 + 1] = 7;
	++lineIndex;

	mAABBBufferN[lineIndex * 2 + 0] = 7;
	mAABBBufferN[lineIndex * 2 + 1] = 4;
	++lineIndex;

	// 竖直的4条线,从最大点开始,顺时针
	mAABBBufferN[lineIndex * 2 + 0] = 4;
	mAABBBufferN[lineIndex * 2 + 1] = 2;
	++lineIndex;

	mAABBBufferN[lineIndex * 2 + 0] = 5;
	mAABBBufferN[lineIndex * 2 + 1] = 3;
	++lineIndex;

	mAABBBufferN[lineIndex * 2 + 0] = 6;
	mAABBBufferN[lineIndex * 2 + 1] = 0;
	++lineIndex;

	mAABBBufferN[lineIndex * 2 + 0] = 7;
	mAABBBufferN[lineIndex * 2 + 1] = 1;
	++lineIndex;

	// 下底面的4条线,从最小点开始,顺时针
	mAABBBufferN[lineIndex * 2 + 0] = 0;
	mAABBBufferN[lineIndex * 2 + 1] = 1;
	++lineIndex;

	mAABBBufferN[lineIndex * 2 + 0] = 1;
	mAABBBufferN[lineIndex * 2 + 1] = 2;
	++lineIndex;

	mAABBBufferN[lineIndex * 2 + 0] = 2;
	mAABBBufferN[lineIndex * 2 + 1] = 3;
	++lineIndex;

	mAABBBufferN[lineIndex * 2 + 0] = 3;
	mAABBBufferN[lineIndex * 2 + 1] = 0;
	++lineIndex;
}

void txMesh::create3DMaxAABBRenderBuffer(const txAABox& aabb)
{
	TRACE_DELETE_ARRAY(m3DMaxAABBBufferV);
	TRACE_DELETE_ARRAY(m3DMaxAABBBufferN);

	// 顶点数据
	m3DMaxAABBVertexBufferSize = 8 * 3;
	m3DMaxAABBBufferV = TRACE_NEW_ARRAY(GLfloat, m3DMaxAABBVertexBufferSize, m3DMaxAABBBufferV);
	int vertexIndex = 0;
	const VECTOR3& aabbMin = aabb.getMin();
	const VECTOR3& aabbMax = aabb.getMax();
	// 下底面,从最小点开始,顺时针
	m3DMaxAABBBufferV[vertexIndex * 3 + 0] = aabbMin.x;
	m3DMaxAABBBufferV[vertexIndex * 3 + 1] = aabbMin.y;
	m3DMaxAABBBufferV[vertexIndex * 3 + 2] = aabbMin.z;
	++vertexIndex;

	m3DMaxAABBBufferV[vertexIndex * 3 + 0] = aabbMax.x;
	m3DMaxAABBBufferV[vertexIndex * 3 + 1] = aabbMin.y;
	m3DMaxAABBBufferV[vertexIndex * 3 + 2] = aabbMin.z;
	++vertexIndex;

	m3DMaxAABBBufferV[vertexIndex * 3 + 0] = aabbMax.x;
	m3DMaxAABBBufferV[vertexIndex * 3 + 1] = aabbMin.y;
	m3DMaxAABBBufferV[vertexIndex * 3 + 2] = aabbMax.z;
	++vertexIndex;

	m3DMaxAABBBufferV[vertexIndex * 3 + 0] = aabbMin.x;
	m3DMaxAABBBufferV[vertexIndex * 3 + 1] = aabbMin.y;
	m3DMaxAABBBufferV[vertexIndex * 3 + 2] = aabbMax.z;
	++vertexIndex;

	// 上底面,从最大点开始,顺时针
	m3DMaxAABBBufferV[vertexIndex * 3 + 0] = aabbMax.x;
	m3DMaxAABBBufferV[vertexIndex * 3 + 1] = aabbMax.y;
	m3DMaxAABBBufferV[vertexIndex * 3 + 2] = aabbMax.z;
	++vertexIndex;

	m3DMaxAABBBufferV[vertexIndex * 3 + 0] = aabbMin.x;
	m3DMaxAABBBufferV[vertexIndex * 3 + 1] = aabbMax.y;
	m3DMaxAABBBufferV[vertexIndex * 3 + 2] = aabbMax.z;
	++vertexIndex;

	m3DMaxAABBBufferV[vertexIndex * 3 + 0] = aabbMin.x;
	m3DMaxAABBBufferV[vertexIndex * 3 + 1] = aabbMax.y;
	m3DMaxAABBBufferV[vertexIndex * 3 + 2] = aabbMin.z;
	++vertexIndex;

	m3DMaxAABBBufferV[vertexIndex * 3 + 0] = aabbMax.x;
	m3DMaxAABBBufferV[vertexIndex * 3 + 1] = aabbMax.y;
	m3DMaxAABBBufferV[vertexIndex * 3 + 2] = aabbMin.z;
	++vertexIndex;

	// 索引数据
	m3DMaxAABBIndexBufferSize = 12 * 2;
	m3DMaxAABBBufferN = TRACE_NEW_ARRAY(GLushort, m3DMaxAABBIndexBufferSize, m3DMaxAABBBufferN);

	int lineIndex = 0;
	// 上底面的4条线,以最大点为起点,顺时针
	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 4;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 5;
	++lineIndex;

	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 5;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 6;
	++lineIndex;

	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 6;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 7;
	++lineIndex;

	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 7;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 4;
	++lineIndex;

	// 竖直的4条线,从最大点开始,顺时针
	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 4;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 2;
	++lineIndex;

	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 5;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 3;
	++lineIndex;

	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 6;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 0;
	++lineIndex;

	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 7;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 1;
	++lineIndex;

	// 下底面的4条线,从最小点开始,顺时针
	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 0;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 1;
	++lineIndex;

	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 1;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 2;
	++lineIndex;

	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 2;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 3;
	++lineIndex;

	m3DMaxAABBBufferN[lineIndex * 2 + 0] = 3;
	m3DMaxAABBBufferN[lineIndex * 2 + 1] = 0;
	++lineIndex;
}