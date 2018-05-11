#include "txSubMesh.h"
#include "txMaterial.h"
#include "txMesh.h"
#include "txTextureManager.h"
#include "txMathUtility.h"
#include "txTexture.h"

txSubMesh::txSubMesh(txMesh* parent)
:
mParent(parent),
m_subMaterial(NULL),
mAutoDestroyTexture(true)
{
	;
}

txSubMesh::~txSubMesh()
{
	if (m_subMaterial != NULL)
	{
		if (mAutoDestroyTexture && m_subMaterial->getDiffuseTexture() != EMPTY_STRING)
		{
			mTextureManager->destroyTexture(m_subMaterial->getDiffuseTexture());
		}
		TRACE_DELETE(m_subMaterial);
	}
}

void txSubMesh::setMaterial(const txMaterial& material)
{
	TRACE_DELETE(m_subMaterial);
	m_subMaterial = TRACE_NEW(txMaterial, m_subMaterial);
	*m_subMaterial = material;
}

void txSubMesh::GenerateTexture()
{
	if (m_subMaterial != NULL)
	{
		txTexture* pTexture = mTextureManager->getTexture(m_subMaterial->getDiffuseTexture());
		pTexture->initTexture();
	}
}

void txSubMesh::CopySubMesh(txSubMesh* pSubMesh)
{
	int indicesCount = m_indices.size();
	FOR_STL(m_indices, int i = 0; i < indicesCount; ++i)
	{
		pSubMesh->m_indices.push_back(m_indices[i]);
	}
	END_FOR_STL(m_indices);
	pSubMesh->m_subMaterial = TRACE_NEW(txMaterial, pSubMesh->m_subMaterial);
	*(pSubMesh->m_subMaterial) = *m_subMaterial;
	pSubMesh->m_nNumTris = m_nNumTris;
	pSubMesh->m_materialIndex = m_materialIndex;
}

bool txSubMesh::rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& point)
{
	if (mParent == NULL)
	{
		return false;
	}
	bool ret = false;
	GLfloat* vertexBuffer = mParent->GetVertexBuffer();
	int vertexBufferSize = mParent->GetVertexBufferSize();
	int vertexCount = vertexBufferSize / 3;
	// 先将所有顶点数据放入三维向量列表中组合成一个个三维顶点
	VECTOR3* vertexList = TRACE_NEW_ARRAY(VECTOR3, vertexBufferSize / 3, vertexList);
	for (int i = 0; i < vertexCount; ++i)
	{
		vertexList[i] = VECTOR3(vertexBuffer[i * 3 + 0], vertexBuffer[i * 3 + 1], vertexBuffer[i * 3 + 2]);
	}
	VECTOR3 interPoint;
	int triangleCount = m_indices.size() / 3;
	FOR_STL(m_indices, int i = 0; i < triangleCount; ++i)
	{
		const VECTOR3& vertex0 = vertexList[m_indices[3 * i + 0]];
		const VECTOR3& vertex1 = vertexList[m_indices[3 * i + 1]];
		const VECTOR3& vertex2 = vertexList[m_indices[3 * i + 2]];
		if (txMath::intersectRayTriangle(ori, dir, vertex0, vertex1, vertex2, interPoint))
		{
			if (!ret || txMath::getLength(interPoint - ori) < txMath::getLength(point - ori))
			{
				point = interPoint;
			}
			ret = true;
		}
	}
	END_FOR_STL(m_indices);
	TRACE_DELETE_ARRAY(vertexList);
	return ret;
}