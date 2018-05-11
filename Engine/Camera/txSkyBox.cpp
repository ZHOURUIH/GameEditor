#include "txSkyBox.h"
#include "txEntity.h"
#include "txSubMesh.h"
#include "txSubEntity.h"
#include "txNode.h"
#include "txCamera.h"
#include "txTextureManager.h"
#include "txNodeManager.h"
#include "txEntityManager.h"
#include "txSceneManager.h"
#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txRendererManager.h"

txSkyBox::txSkyBox()
:
mBoxEntity(NULL)
{
	mBoxEntityName = "skyBox";
}

txSkyBox::~txSkyBox()
{
	mEntityManager->DestroyEntity(mBoxEntityName);
}

void txSkyBox::init(const float& fDistance, const float& fUpDistance, const float& fBottomDistance, const std::string& texturePath)
{
	const int PLANE_COUNT = 6;
	const int POINT_PER_PLANE = 4;
	std::string skyBoxs[PLANE_COUNT] = { "sky_F.swi", "sky_L.swi", "sky_B.swi", "sky_R.swi", "sky_T.swi", "sky_D.swi" };
	GLfloat vertexs[PLANE_COUNT * POINT_PER_PLANE * 3] =
	{ -fDistance, -fBottomDistance, -fDistance,
	fDistance, -fBottomDistance, -fDistance,
	fDistance, fUpDistance, -fDistance,
	-fDistance, fUpDistance, -fDistance,

	-fDistance, -fBottomDistance, fDistance,
	-fDistance, -fBottomDistance, -fDistance,
	-fDistance, fUpDistance,-fDistance, 
	-fDistance, fUpDistance, fDistance, 

	fDistance, -fBottomDistance, fDistance,
	-fDistance, -fBottomDistance, fDistance,
	-fDistance, fUpDistance, fDistance, 
	fDistance, fUpDistance, fDistance,

	fDistance, -fBottomDistance, -fDistance,
	fDistance, -fBottomDistance, fDistance,
	fDistance, fUpDistance, fDistance,
	fDistance, fUpDistance, -fDistance, 

	-fDistance, fUpDistance, -fDistance, 
	fDistance, fUpDistance, -fDistance,
	fDistance, fUpDistance, fDistance, 
	-fDistance, fUpDistance, fDistance, 

	-fDistance, -fBottomDistance, -fDistance,
	fDistance, -fBottomDistance, -fDistance,
	fDistance, -fBottomDistance, fDistance,
	-fDistance, -fBottomDistance, fDistance, };

	GLfloat texCoords[PLANE_COUNT * POINT_PER_PLANE * 2] =
	{ 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 
	0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 
	0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 
	0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 
	0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 
	0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, };
	txMesh* pMesh = TRACE_NEW(txMesh, pMesh);

	pMesh->MakeVertexBuffer(PLANE_COUNT * POINT_PER_PLANE * 3);
	pMesh->MakeTexCoordBuffer(PLANE_COUNT * POINT_PER_PLANE * 2);
	for (int nIndex = 0; nIndex < 24; ++nIndex)
	{
		*(pMesh->GetVertexBuffer() + 3 * nIndex) = vertexs[3 * nIndex];
		*(pMesh->GetVertexBuffer() + 3 * nIndex + 1) = vertexs[3 * nIndex + 1];
		*(pMesh->GetVertexBuffer() + 3 * nIndex + 2) = vertexs[3 * nIndex + 2];

		*(pMesh->GetTexCoordBuffer() + 2 * nIndex) = texCoords[2 * nIndex];
		*(pMesh->GetTexCoordBuffer() + 2 * nIndex + 1) = texCoords[2 * nIndex + 1];
	}

	for (int i = 0; i < PLANE_COUNT; ++i)
	{
		//create one subSet to the mesh
		txSubMesh* pSubMesh = pMesh->CreateSubMesh();

		std::string imageFile = texturePath + skyBoxs[i];

		txTexture* pTexture = mTextureManager->createTexture(imageFile);
		pTexture->initTexture();
		
		txMaterial material;
		material.setDiffuseTexture(pTexture->getName());
		pSubMesh->setMaterial(material);
		pSubMesh->pushIndex(4 * i + 0);
		pSubMesh->pushIndex(4 * i + 1);
		pSubMesh->pushIndex(4 * i + 2);

		pSubMesh->pushIndex(4 * i + 0);
		pSubMesh->pushIndex(4 * i + 2);
		pSubMesh->pushIndex(4 * i + 3);
	}
	mBoxEntity = mEntityManager->CreateManualEntity(mBoxEntityName, pMesh);
	setSkyboxPerspective(45.0f, 1.0f, 1000.0f);
}

void txSkyBox::setSkyboxPerspective(const float& fovY, const float& nearClip, const float& farClip)
{
	float ratio = mRenderWindow->getAspect();
	mFovY = fovY;
	mRatio = ratio;
	mNearClip = nearClip;
	mFarClip = farClip;
	mSkyboxPerspective = txMath::perspective(mFovY, mRatio, mNearClip, mFarClip);
}

void txSkyBox::render(txCamera* camera)
{
	if (mBoxEntity && camera != NULL)
	{
		MATRIX4 skyMatrix = camera->getViewMatrix();
		skyMatrix[3].x = 0.0;
		skyMatrix[3].y = 0.0;
		skyMatrix[3].z = 0.0;
		txVector<txSubEntity*>& subEntityList = mBoxEntity->getSubEntityList();
		int listSize = subEntityList.size();
		FOR_STL(subEntityList, int i = 0; i < listSize; ++i)
		{
			txSubEntity* pSubEntity = subEntityList[i];
			if (pSubEntity != NULL)
			{
				txMesh* pMesh = pSubEntity->GetOriMesh();
				if (pMesh != NULL)
				{
					int subMeshCount = pMesh->getSubMeshNum();
					int nIndex = 0;
					while (nIndex < subMeshCount)
					{
						txSubMesh* pSubMesh = pMesh->getSubMesh(nIndex);
						if (pSubMesh != NULL)
						{
							txRendererInstance::mSkyboxRender->render(pSubMesh->getMaterial(), mSkyboxPerspective * skyMatrix,
								pMesh->GetVertexBuffer(), pMesh->GetTexCoordBuffer(), pSubMesh->getIndexPtr(), pSubMesh->getIndexCount());
							++nIndex;
						}
					}
				}
			}
		}
		END_FOR_STL(subEntityList);
	}
}

void txSkyBox::destroy()
{
	if(mBoxEntity != NULL)
	{
		mEntityManager->DestroyEntity(mBoxEntity->getName());
		mBoxEntity = NULL;
	}
}
