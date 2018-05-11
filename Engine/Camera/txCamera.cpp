#include "txCamera.h"
#include "txEngineRoot.h"
#include "txSceneManager.h"
#include "txBillboardManager.h"
#include "txMagicParticleSystem.h"
#include "txViewport.h"
#include "txEntity.h"
#include "txEngineLog.h"
#include "txSubMesh.h"
#include "txScene.h"
#include "txSkyBox.h"
#include "txRouteRender.h"
#include "txODEPhysics.h"
#include "txRendererManager.h"

txCamera::txCamera() 
:
mActive(true), 
pDirLight(NULL), 
bOpenShadowMap(false)
{
	glGenTextures(1, &shadowMapId);
	glGenFramebuffers(1, &mFbo);
	mViewport = TRACE_NEW(txViewport, mViewport, this);	
}

txCamera::~txCamera()
{
	TRACE_DELETE(mViewport);
	glDeleteTextures(1, &shadowMapId);
	glDeleteFramebuffers(1, &mFbo);
}

void txCamera::render(txSceneManager* pSceneManager)
{
	// 在摄像机渲染之前应用视口参数
	mViewport->applyViewport();

	if(pSceneManager == NULL)
	{
		return;
	}

	bOpenShadowMap = pSceneManager->getOpenShadowMap();

	if(bOpenShadowMap)
	{
		mTEXTURE_SIZE = pSceneManager->getShadowSize();

		mLightProjectionMatrix = pSceneManager->getLightOrthoMatrix();
		const VECTOR3& LightPosition = pSceneManager->getLightPosition();
		const VECTOR3& LightDirection = pSceneManager->getLightDirection();

		VECTOR3 eye, center, up;
		center = LightPosition;
		eye = center - LightDirection * 100.0f;
		up = txMath::AXIS_Y;
		mLightViewMatrix = txMath::lookAt(eye, center, up);

		GenerateDepthMap();
		DrawDepthMap(pSceneManager);
	}

	// 渲染天空盒
	txSkyBox* skybox = pSceneManager->getSkyBox();
	if (skybox != NULL)
	{
		skybox->render(this);
	}

	//普通渲染
	VECTOR3 viewpos;
	viewpos.x = mViewMatrix[3].x;
	viewpos.y = mViewMatrix[3].y;
	viewpos.z = mViewMatrix[3].z;
	Light_RenderNode(pSceneManager->getRootNode(), viewpos);

	//半透明渲染
	RenderTransparentEntity();

	// 渲染场景高度图
	renderHeightMap(pSceneManager);

	// 渲染场景路线图
	renderSceneRoute(pSceneManager);

	// 渲染公告板
	mBillboardManager->RenderAllBillboard();
	// 渲染魔法粒子系统
	mMagicParticleSystem->RenderParticles(this);
	// 渲染物理碰撞体
	mODEPhysics->render(this);
}

void txCamera::renderSceneRoute(txSceneManager* pSceneManager)
{
	auto& sceneList = pSceneManager->getSceneList();
	auto iterScene = sceneList.begin();
	auto iterSceneEnd = sceneList.end();
	FOR_STL(sceneList, ; iterScene != iterSceneEnd; ++iterScene)
	{
		auto& routeRenderList = iterScene->second->getRouteRenderList();
		auto iterRoute = routeRenderList.begin();
		auto iterRouteEnd = routeRenderList.end();
		FOR_STL(routeRenderList, ; iterRoute != iterRouteEnd; ++iterRoute)
		{
			txRouteRender* routeRender = iterRoute->second;
			if (routeRender->isVisible())
			{
				routeRender->render(this);
			}
		}
		END_FOR_STL(routeRenderList);
	}
	END_FOR_STL(sceneList);
}

void txCamera::renderHeightMap(txSceneManager* pSceneManager)
{
	auto& sceneList = pSceneManager->getSceneList();
	auto iterScene = sceneList.begin();
	auto iterSceneEnd = sceneList.end();
	FOR_STL(sceneList, ; iterScene != iterSceneEnd; ++iterScene)
	{
		if (iterScene->second->getShowHeightMap())
		{
			if (iterScene->second->getHeightMapVertices(0) == NULL)
			{
				iterScene->second->generateRenderHeightMap();
			}
			VECTOR3 pos = iterScene->second->getSceneOrigin();
			pos.y = 0.0f;
			MATRIX4 localMat4 = txMath::translate(txMath::MAT4_IDENTITY, pos);
			int bufferCount = iterScene->second->getHeightMapBufferCount();
			for (int i = 0; i < bufferCount; ++i)
			{
				GLushort* indexBuffer = iterScene->second->getHeightMapIndices(i);
				GLfloat* vertexBuffer = iterScene->second->getHeightMapVertices(i);
				int indicesCount = iterScene->second->getHeightMapIndicesCount(i);
				if (vertexBuffer != NULL && indexBuffer != NULL && indicesCount > 0)
				{
					txRendererInstance::mColourRender->render(VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), mPerspectiveMatrix * mViewMatrix * localMat4,
						vertexBuffer, indexBuffer, indicesCount, 1.0f);
				}
			}
		}
	}
	END_FOR_STL(sceneList);
}

void txCamera::renderStaticMeshAABB(txMesh* pMesh, const MATRIX4& transMat, const VECTOR4& colour)
{
	if (pMesh == NULL)
	{
		return;
	}
	GLfloat* vertexBuffer = pMesh->getAABBVertexBuffer();
	GLushort* indexBuffer = pMesh->getAABBIndexBuffer();
	int indicesCount = pMesh->getAABBIndexBufferSize();
	if (vertexBuffer == NULL || indexBuffer == NULL || indicesCount == 0)
	{
		return;
	}
	txRendererInstance::mColourRender->render(colour, mPerspectiveMatrix * mViewMatrix * transMat, vertexBuffer, indexBuffer, indicesCount);
}

void txCamera::renderDynamicMeshAABB(txMesh* pMesh, const MATRIX4& transMat, const VECTOR4& colour)
{
	if (pMesh == NULL)
	{
		return;
	}
	GLfloat* vertexBuffer = pMesh->get3DMaxAABBVertexBuffer();
	GLushort* indexBuffer = pMesh->get3DMaxAABBIndexBuffer();
	int indicesCount = pMesh->get3DMaxAABBIndexBufferSize();
	if (vertexBuffer == NULL || indexBuffer == NULL || indicesCount == 0)
	{
		return;
	}
	txRendererInstance::mColourRender->render(colour, mPerspectiveMatrix * mViewMatrix * transMat, vertexBuffer, indexBuffer, indicesCount);
}

void txCamera::setViewport(const float& topX, const float& topY, const float& width, const float& height)
{
	mViewport->setViewport((int)topX, (int)topY, (int)width, (int)height);
}

void txCamera::setPerspectiveParameters(const float& angle, const float& ratio, const float& nearp, const float& farp)
{
	mAngle = angle;
	mRatio = ratio;
	mNearp = nearp;
	mFarp = farp;
	frustum.setCamInternals(mAngle, mRatio, mNearp, mFarp);
	setPerspectiveMatrix(txMath::perspective(mAngle, mRatio, mNearp, mFarp)); //Set perspective matrix
}

void txCamera::setAspect(const float& aspect)
{
	mRatio = aspect;
	frustum.setCamInternals(mAngle, aspect, mNearp, mFarp);
	setPerspectiveMatrix(txMath::perspective(mAngle, mRatio, mNearp, mFarp)); //Set perspective matrix
}

void txCamera::RenderTransparentEntity()
{
	int nTransparentSize = mTransparentEntity.size();
	FOR_STL(mTransparentEntity, int nIndex = nTransparentSize - 1; nIndex >= 0; --nIndex)
	{
		txSubEntity* pSubEntity = mTransparentEntity[nIndex].mpSubEntity;
		//AABB
		const MATRIX4& MMatrix = pSubEntity->GetParent()->GetParent()->GetFinalMatrix();
		MATRIX4 FFMatrix = mPerspectiveMatrix * mViewMatrix * MMatrix;
		float txFFMat[16];
		for(int i = 0; i < 4; ++i)
		{
			txFFMat[i * 4 + 0] = FFMatrix[i].x;
			txFFMat[i * 4 + 1] = FFMatrix[i].y;
			txFFMat[i * 4 + 2] = FFMatrix[i].z;
			txFFMat[i * 4 + 3] = FFMatrix[i].w;
		}
		frustum.setFrustum(txFFMat);
		VECTOR3 viewpos;
		viewpos.x = mViewMatrix[3].x;
		viewpos.y = mViewMatrix[3].y;
		viewpos.z = mViewMatrix[3].z;
		Entity_RenderSubEntity(pSubEntity, pDirLight, viewpos);
	}
	END_FOR_STL(mTransparentEntity);

	mTransparentEntity.clear();
}

void txCamera::PushOneTransparentEntity(txEntity* pEntity)
{
	txVector<txSubEntity*>& subEntityList = pEntity->getSubEntityList();
	int listSize = subEntityList.size();
	FOR_STL(subEntityList, int i = 0; i < listSize; ++i)
	{
		txSubEntity* pSubEntity = subEntityList[i];
		if (pSubEntity)
		{
			txMesh* pOrgiMesh = pSubEntity->GetOriMesh();
			//利用Mesh的AABB盒
			txAABox* pMeshAABox = pOrgiMesh->GetAABox();	//局部的AABB盒
			//节点的世界变换矩阵
			const MATRIX4& worldMatrix = pEntity->GetParent()->GetFinalMatrix();
			VECTOR3 centerPos = pMeshAABox->getCenter();
			VECTOR3 worldCenterPos = txMath::transformVector3(centerPos, worldMatrix);

			VECTOR3 viewpos;
			viewpos.x = mViewMatrix[3].x;
			viewpos.y = mViewMatrix[3].y;
			viewpos.z = mViewMatrix[3].z;
			VECTOR3 depthDirection = viewpos - worldCenterPos;
			float entityDepth = sqrt(depthDirection.x * depthDirection.x + depthDirection.y * depthDirection.y + depthDirection.z * depthDirection.z);

			//插入到深度渲染队列中，按照深度升序排列（插入排序）
			int nTransparentSize = mTransparentEntity.size();
			RenderStruct renderDepthStruct(entityDepth, pSubEntity);
			mTransparentEntity.push_back(renderDepthStruct);

			int nIndex = 0;
			for (nIndex = nTransparentSize; nIndex >= 0; --nIndex)
			{
				RenderStruct renderStruct = mTransparentEntity[nIndex];
				float renderDepth = renderStruct.mEntityDepth;
				if (renderDepth < entityDepth)
				{
					break;
				}
			}
			//往后移动
			for (int nBigIndex = nTransparentSize; nBigIndex > nIndex + 1; --nBigIndex)
			{
				mTransparentEntity[nBigIndex] = mTransparentEntity[nBigIndex - 1];
			}

			mTransparentEntity[nIndex + 1] = renderDepthStruct;
		}
	}
	END_FOR_STL(subEntityList);
}

void txCamera::Entity_RenderSubEntity(txSubEntity *pSubEntity, txDirLight *light, const VECTOR3& lightvpos)
{
	txEntity* parentEntity = pSubEntity->GetParent();
	const MATRIX4& entityFinalMatrix = parentEntity->GetParent()->GetFinalMatrix();
	MATRIX4 MVMatrix = mViewMatrix * entityFinalMatrix;
	MATRIX4 MVPMatrix = mPerspectiveMatrix * mViewMatrix * entityFinalMatrix;
	MATRIX4 shadowMapMVPMatrix = mLightProjectionMatrix * mLightViewMatrix * entityFinalMatrix;
	txMesh* pOrgiMesh = pSubEntity->GetOriMesh();
	GLfloat* meshVertexBuffer = pOrgiMesh->GetVertexBuffer();
	GLfloat* meshTexBuffer = pOrgiMesh->GetTexCoordBuffer();
	GLfloat* normalBuffer = pOrgiMesh->GetNormalBuffer();
	float alpha = parentEntity->getAlpha();
	if(pOrgiMesh->isSkeletonMesh)
	{
		MATRIX4* boneMatrixArray = pSubEntity->GetBoneMatrixArray();
		int boneMatrixNum = pSubEntity->GetBoneMatrixNum();
		int subMeshCount = pOrgiMesh->getSubMeshNum();
		GLfloat* boneIndicesArray = pSubEntity->GetBoneIndicesArray();
		GLfloat* boneWeightsArray = pSubEntity->GetBoneWeightsArray();
		int nIndex = 0;
		while (nIndex < subMeshCount)
		{
			txSubMesh* pSubMesh = pOrgiMesh->getSubMesh(nIndex);
			if (pSubMesh)
			{
				if (bOpenShadowMap)
				{
					txRendererInstance::mShadowMapSkinAnimRender->render(
						pSubMesh->getMaterial(), MVMatrix, MVPMatrix,
						boneMatrixArray, boneMatrixNum, shadowMapMVPMatrix, shadowMapId,
						meshVertexBuffer, meshTexBuffer, boneIndicesArray, boneWeightsArray,
						pSubMesh->getIndexPtr(), pSubMesh->getIndexCount());
				}
				else
				{
					txRendererInstance::mSkinAnimationRender->render(
						pSubMesh->getMaterial(), MVMatrix, MVPMatrix,
						boneMatrixArray, boneMatrixNum, meshVertexBuffer, meshTexBuffer, boneIndicesArray, boneWeightsArray,
						pSubMesh->getIndexPtr(), pSubMesh->getIndexCount(), alpha);
				}
			}
			++nIndex;
		}
	}
	else
	{
		int subMeshCount = pOrgiMesh->getSubMeshNum();
		int nIndex = 0;
		while (nIndex < subMeshCount)
		{
			txSubMesh* pSubMesh = pOrgiMesh->getSubMesh(nIndex);
			if (pSubMesh)
			{
				if (bOpenShadowMap)
				{
					txRendererInstance::mShadowMapLightRender->render(
						pSubMesh->getMaterial(), light, lightvpos, MVMatrix, MVPMatrix,
						shadowMapMVPMatrix, shadowMapId, meshVertexBuffer, normalBuffer, meshTexBuffer,
						pSubMesh->getIndexPtr(), pSubMesh->getIndexCount());
				}
				else
				{
					txRendererInstance::mLightRender->render(
						pSubMesh->getMaterial(), light, lightvpos, MVMatrix, MVPMatrix,
						meshVertexBuffer, normalBuffer, meshTexBuffer, pSubMesh->getIndexPtr(), pSubMesh->getIndexCount(), alpha);
				}
			}
			++nIndex;
		}
	}
}

void txCamera::Entity_RenderEntity(txEntity* ent, txDirLight* light, const VECTOR3& lightvpos)
{
	if (!ent->isVisible())
	{
		return;
	}

	if (ent->isTransparent())	//半透明的物体要深度排序后统一绘制
	{
		PushOneTransparentEntity(ent);
		return;
	}

	// 判断此时的Mesh是否在视锥体内（利用Mesh的AABB）如果可以看见该实体,才进行渲染
	const txAABox& entAABB = ent->getAABB();
	if (frustum.boxInFrustum(entAABB) != txFrustumR::OUTSIDE)
	{
		ent->setInView(true);
		const MATRIX4& finalMatrix = ent->GetParent()->GetFinalMatrix();
		bool isSkeletonEntity = (ent->GetSkeleton() != NULL && ent->GetSkeleton()->getBoneCount() > 0);
		txVector<txSubEntity*>& subEntityList = ent->getSubEntityList();
		int listSize = subEntityList.size();
		FOR_STL(subEntityList, int i = 0; i < listSize; ++i)
		{
			txSubEntity* pSubEntity = subEntityList[i];
			if (pSubEntity != NULL)
			{
				Entity_RenderSubEntity(pSubEntity, light, lightvpos);
				if (ent->getShowAABB())
				{
					if (isSkeletonEntity)
					{
						renderDynamicMeshAABB(pSubEntity->GetOriMesh(), finalMatrix);
					}
					else
					{
						renderStaticMeshAABB(pSubEntity->GetOriMesh(), finalMatrix);
					}
				}
			}
		}
		END_FOR_STL(subEntityList);
	}
	else
	{
		ent->setInView(false);
	}
}

void txCamera::Entity_RenderNode(txNode* node, txDirLight* light, const VECTOR3& lightvpos)
{
	//AABB
	const MATRIX4& MMatrix = node->GetFinalMatrix();
	MATRIX4 FFMatrix = mPerspectiveMatrix * mViewMatrix * MMatrix;
	float txFFMat[16];
	for(int i = 0; i < 4; ++i)
	{
		txFFMat[i * 4 + 0] = FFMatrix[i].x;
		txFFMat[i * 4 + 1] = FFMatrix[i].y;
		txFFMat[i * 4 + 2] = FFMatrix[i].z;
		txFFMat[i * 4 + 3] = FFMatrix[i].w;
	}
	frustum.setFrustum(txFFMat);

	//渲染entity
	if (node->getObject() != NULL && node->getObject()->getType() == OT_ENTITY)
	{
		Entity_RenderEntity(static_cast<txEntity*>(node->getObject()), light, lightvpos);
	}

	//渲染子node
	auto& childSet = node->getChildren();
	auto iterNode = childSet.begin();
	auto iterNodeEnd = childSet.end();
	FOR_STL(childSet, ; iterNode != iterNodeEnd; ++iterNode)
	{
		Entity_RenderNode(*iterNode, light, lightvpos);
	}
	END_FOR_STL(childSet);
}

void txCamera::Light_RenderDirLight(txNode* sceneNode, txDirLight* light, const VECTOR3& lightvpos)
{
	Entity_RenderNode(sceneNode, light, lightvpos);
}

void txCamera::setDirLight(txDirLight* dirLight)
{
	pDirLight = dirLight;
}

void txCamera::Light_RenderNode(txNode* node, const VECTOR3& lightvpos)
{
	Light_RenderDirLight(node, pDirLight, lightvpos);
}

void txCamera::SetShadowMapSize(GLsizei TEXTURE_SIZE)
{
	mTEXTURE_SIZE = TEXTURE_SIZE;
}

//直接将深度值设置到颜色缓冲区上得到输出纹理
void txCamera::GenerateDepthMap()
{
	//渲染到纹理生成深度图
	// create a texture object
	//--------------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, shadowMapId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTEXTURE_SIZE, mTEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 创建FBO对象
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowMapId, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//-----------------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//得到渲染出的深度值
void txCamera::DrawDepthMap(txSceneManager* pSceneManager)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	glViewport(0, 0, mTEXTURE_SIZE, mTEXTURE_SIZE);
	glBindTexture(GL_TEXTURE_2D, 0);
	// draw a scene depthMent to a texture directly
	DrawSceneToTexture(pSceneManager);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//以光源点为视点渲染场景到纹理
void txCamera::DrawSceneToTexture(txSceneManager* pSceneManager)
{
	Entity_RenderNode_ShadowMap(pSceneManager->getRootNode());
}

void txCamera::Entity_RenderNode_ShadowMap(txNode* node)
{
	//渲染entity
	if (node->getObject() != NULL && node->getObject()->getType() == OT_ENTITY)
	{
		MATRIX4 MVPMatrix = mLightProjectionMatrix * mLightViewMatrix * node->GetFinalMatrix();
		//render
		txEntity* pEntity = static_cast<txEntity*>(node->getObject());
		txVector<txSubEntity*>& subEntityList = pEntity->getSubEntityList();
		int subEntityCount = subEntityList.size();
		FOR_STL(subEntityList, int i = 0; i < subEntityCount; ++i)
		{
			txSubEntity* pSubEntity = subEntityList[i];
			if (pSubEntity != NULL)
			{
				txMesh* pOrgiMesh = pSubEntity->GetOriMesh();
				if (pOrgiMesh != NULL && !pOrgiMesh->isSkeletonMesh)
				{
					int subMeshCount = pOrgiMesh->getSubMeshNum();

					int nIndex = 0;
					while (nIndex < subMeshCount)
					{
						txSubMesh* pSubMesh = pOrgiMesh->getSubMesh(nIndex);
						if (pSubMesh != NULL)
						{
							txRendererInstance::mShadowMapRender->render(
								pSubMesh->getMaterial(), MVPMatrix, pOrgiMesh->GetVertexBuffer(),
								pOrgiMesh->GetTexCoordBuffer(), pSubMesh->getIndexPtr(), pSubMesh->getIndexCount());
						}
						++nIndex;
					}
				}
			}
		}
		END_FOR_STL(subEntityList);
	}

	// 渲染子node
	auto& childSet = node->getChildren();
	auto iterNode = childSet.begin();
	auto iterNodeEnd = childSet.end();
	FOR_STL(childSet, ; iterNode != iterNodeEnd; ++iterNode)
	{
		Entity_RenderNode_ShadowMap(*iterNode);
	}
	END_FOR_STL(childSet);
}

//将深度图输出到文件中
void txCamera::OutputShadowMapToFile()
{
#define WIDTHBYTES(bits) (((bits)+31)/32*4)

#ifdef _USE_SAFE_API
	FILE* srcFile = NULL;
	fopen_s(&srcFile, "/sdcard/txtj/ShadowMapFiles/Penguins.bmp", "rb");
#else
	FILE* srcFile = fopen("/sdcard/txtj/ShadowMapFiles/Penguins.bmp", "rb");
#endif
	if (srcFile == NULL)
	{
		return;
	}

	unsigned char bmpHeader[100];
	fread(bmpHeader, 1, 54, srcFile);

#ifdef _USE_SAFE_API
	FILE* dstFile = NULL;
	fopen_s(&dstFile, "/sdcard/txtj/ShadowMapFiles/ShadowMap.bmp", "wb");
#else
	FILE* dstFile = fopen("/sdcard/txtj/ShadowMapFiles/ShadowMap.bmp", "wb");
#endif
	if (dstFile == NULL)
	{
		fclose(srcFile);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	txRendererInstance::checkGlError("OutputShadowMapToFile 1");

	int nLength = WIDTHBYTES(mTEXTURE_SIZE * 32);
	int nDataLength = nLength * mTEXTURE_SIZE;
	unsigned char* pImageData = (unsigned char*)malloc(nDataLength + 4);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, mTEXTURE_SIZE, mTEXTURE_SIZE, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
	txRendererInstance::checkGlError("glReadPixels 2");

	nDataLength += 54;

	fwrite(bmpHeader,54,1,dstFile);

	fseek(dstFile,0x02,SEEK_SET);
	fwrite(&nDataLength,4,1,dstFile);	//位图文件大小

	int nOffset = 54;
	fseek(dstFile,0x0A,SEEK_SET);
	fwrite(&nOffset,4,1,dstFile);	//位图数据偏移

	fseek(dstFile,0x12,SEEK_SET);
	fwrite(&mTEXTURE_SIZE,4,1,dstFile);	//位图的宽度

	int height = -mTEXTURE_SIZE;
	fseek(dstFile,0x16,SEEK_SET);
	fwrite(&height,4,1,dstFile);	//位图的高度

	unsigned short bits = 32;
	fseek(dstFile,0x1C,SEEK_SET);
	fwrite(&bits,2,1,dstFile);	//位图的高度

	nDataLength -= 54;
	fseek(dstFile,0x22,SEEK_SET);
	fwrite(&nDataLength,4,1,dstFile);	//位图数据大小

	fseek(dstFile,0x36,SEEK_SET);
	for(int iIndex = 0; iIndex < mTEXTURE_SIZE; ++iIndex)
	{
		for(int jIndex = 0; jIndex < nLength; ++jIndex)
		{
			unsigned char* nImageData = pImageData + nLength * iIndex + jIndex + 2;
			fwrite(nImageData, 1, 1, dstFile);

			nImageData = pImageData + nLength * iIndex + jIndex + 1;
			fwrite(nImageData, 1, 1, dstFile);

			nImageData = pImageData + nLength * iIndex + jIndex + 0;
			fwrite(nImageData, 1, 1, dstFile);

			nImageData = pImageData + nLength * iIndex + jIndex + 3;
			fwrite(nImageData, 1, 1, dstFile);

			jIndex += 3;
		}
	}

	free(pImageData);
	fclose(srcFile);
	fclose(dstFile);
}//350 * 241