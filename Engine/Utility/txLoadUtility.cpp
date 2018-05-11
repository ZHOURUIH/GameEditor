#include "txLoadUtility.h"
#include "txEngineLog.h"
#include "txMesh.h"
#include "txSubMesh.h"
#include "txBone.h"
#include "txSkeleton.h"
#include "txSkin.h"
#include "txAnimation.h"
#include "txAnimationNode.h"
#include "txMaterial.h"
#include "txSceneManager.h"
#include "txTextureManager.h"
#include "txEngineRoot.h"
#include "txEntity.h"
#include "json/json.h"
#include "txSerializer.h"
#include "txTexture.h"

bool txLoadUtility::loadSkeletonMeshFromFile(const std::string& fileName, txVector<SkinMeshPair>& skinMeshPairList, txEntity* pEntity, const bool& loadTexture, txVector<txTexture*>* textureList)
{
	int dotPos = fileName.find_last_of('.');
	std::string suffix = fileName.substr(dotPos + 1, fileName.length() - dotPos - 1);
	if (suffix == "json")
	{
		if (!loadJsonSkeletonMesh(fileName, skinMeshPairList, pEntity, loadTexture, textureList))
		{
			return false;
		}
	}
	else if (suffix == "skinmesh")
	{
		if (!loadBinarySkeletonMesh(fileName, skinMeshPairList, pEntity, loadTexture, textureList))
		{
			return false;
		}
	}
	return true;
}

bool txLoadUtility::loadJsonSkeletonMesh(const std::string& fileName, txVector<SkinMeshPair>& skinMeshPairList, txEntity* pEntity, const bool& loadTexture, txVector<txTexture*>* textureList)
{
	// 发送事件通知实体开始加载文件,无参数
	mEngineRoot->sendEvent(EE_START_LOAD_DYNAMIC_ENTITY, pEntity->getName());
	std::string meshFileName = fileName.substr(fileName.find_last_of('/') + 1, fileName.length() - fileName.find_last_of('/') - 1);
	pEntity->setMeshFileName(meshFileName);
	std::string skeletonDir = fileName.substr(0, fileName.find_last_of('/'));
	std::string skeletonFileName;
	std::string skeletonFullFileName;
	Json::Reader jRead;
	Json::Value arrVal;
	std::string animationFile;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::objectValue)
		{
			animationFile = arrVal["AnimationFile"].asString();

			// 记录下骨骼的文件名
			skeletonFileName = arrVal["SkeletonFile"].asString();
			skeletonFullFileName = skeletonDir + "/" + skeletonFileName;

			txVector<txMaterial> materials;
			if (!arrVal["materials"].isNull())
			{
				auto iTer = arrVal["materials"].begin();
				auto materialTerEnd = arrVal["materials"].end();
				while (iTer != materialTerEnd)
				{
					int nMaterialIndex = (*iTer)["DbgIndex"].asInt();
					bool wireframe = (*iTer)["wireframe"].asBool();
					std::string shadingMode = (*iTer)["shading"].asString();
					std::string materialName = (*iTer)["DbgName"].asString();
					float mapLightAnisotropy = (*iTer)["mapLightAnisotropy"].asFloat();
					std::string blending = (*iTer)["blending"].asString();
					VECTOR3 colorDiffuse = VECTOR3((*iTer)["colorDiffuse"][0].asFloat(), (*iTer)["colorDiffuse"][1].asFloat(), (*iTer)["colorDiffuse"][2].asFloat());
					bool depthTest = (*iTer)["depthTest"].asBool();
					VECTOR3 colorEmissive = VECTOR3((*iTer)["colorEmissive"][0].asFloat(), (*iTer)["colorEmissive"][1].asFloat(), (*iTer)["colorEmissive"][2].asFloat());
					bool depthWrite = (*iTer)["depthWrite"].asBool();
					float opacity = (*iTer)["opacity"].asFloat();
					VECTOR3 colorAmbient = VECTOR3((*iTer)["colorAmbient"][0].asFloat(), (*iTer)["colorAmbient"][1].asFloat(), (*iTer)["colorAmbient"][2].asFloat());
					std::string diffuseTexture = (*iTer)["mapLight"].asString();
					std::string wrapU = (*iTer)["mapLightWrap"][0].asString();
					std::string wrapV = (*iTer)["mapLightWrap"][1].asString();
					int repeatU = (*iTer)["mapLightRepeat"][0].asInt();
					int repeatV = (*iTer)["mapLightRepeat"][1].asInt();
					bool transparent = (*iTer)["DbgIndex"].asBool();
					int uvLayer = (*iTer)["DbgIndex"].asInt();

					diffuseTexture = P_MODEL_TEXTURE_PATH + diffuseTexture.substr(0, diffuseTexture.find_last_of('.')) + ".swi";
					txTexture* pTexture = mTextureManager->createTexture(diffuseTexture);
					if (loadTexture)
					{
						pTexture->initTexture();
					}
					if (textureList != NULL)
					{
						textureList->push_back(pTexture);
					}

					txMaterial material;
					material.setMaterialIndex(nMaterialIndex);
					material.setWireframe(wireframe);
					material.setShadingMode(shadingMode);
					material.setName(materialName);
					material.setAnisotropy(mapLightAnisotropy);
					material.setBlending(blending);
					material.setDiffuseColour(colorDiffuse);
					material.setDepthTest(depthTest);
					material.setEmissiveColour(colorEmissive);
					material.setDepthWrite(depthWrite);
					material.setOpacity(opacity);
					material.setAmbientColour(colorAmbient);
					material.setDiffuseTexture(pTexture->getName());
					material.setWrapU(wrapU);
					material.setWrapV(wrapV);
					material.setRepeatU(repeatU);
					material.setRepeatV(repeatV);
					material.setTransparent(transparent);
					material.setUVLayer(uvLayer);

					materials.push_back(material);
					++iTer;
				}
			}

			if (!arrVal["meshes"].isNull())
			{
				auto meshTer = arrVal["meshes"].begin();
				auto meshTerEnd = arrVal["meshes"].end();
				while (meshTer != meshTerEnd)
				{
					int nMaterialIndex = (*meshTer)["Material"].asInt();
					txMesh* pMesh = TRACE_NEW(txMesh, pMesh);

					float min_x = 0.0f, min_y = 0.0f, min_z = 0.0f, max_x = 0.0f, max_y = 0.0f, max_z = 0.0f;
					//-------------------------------------------

					txSubMesh* pSubMesh = pMesh->CreateSubMesh();
					txSkin* pSkin = TRACE_NEW(txSkin, pSkin);

					SkinMeshPair skinMeshPair;
					skinMeshPair.mMesh = pMesh;
					skinMeshPair.mSkin = pSkin;
					skinMeshPairList.push_back(skinMeshPair);

					if (nMaterialIndex < (int)materials.size())
					{
						pSubMesh->setMaterial(materials[nMaterialIndex]);
					}
					else
					{
						ENGINE_ERROR("error : [txJsonMeshLoader::LoadSkeletonMeshFromFile] nMaterialIndex < materials.size()[%d]", materials.size());
					}

					if (!(*meshTer)["bones"].isNull())
					{
						if ((*meshTer)["bones"].isArray())
						{
							auto memTIte = (*meshTer)["bones"].begin();
							auto memTIteEnd = (*meshTer)["bones"].end();
							while (memTIte != memTIteEnd)
							{
								if ((*memTIte).type() == Json::objectValue)
								{
									MATRIX4 offsetMatrix;
									for (int iIndex = 0; iIndex < 4; ++iIndex)
									{
										for (int jIndex = 0; jIndex < 4; ++jIndex)
										{
											offsetMatrix[jIndex][iIndex] = ((*memTIte)["offsetMatrix"])[iIndex * 4 + jIndex].asFloat();
										}
									}
									pSkin->setOffsetMatrix((*memTIte)["name"].asString().c_str(), offsetMatrix);

									int nVertexIndiceSize = (*memTIte)["vertexIndices"].size();
									int nVertexWeightSize = (*memTIte)["vertexWeights"].size();
									if (nVertexIndiceSize == nVertexWeightSize)
									{
										for (int index = 0; index < nVertexIndiceSize; ++index)
										{
											int nVertexIndex = ((*memTIte)["vertexIndices"])[index].asInt();
											float fWeight = ((*memTIte)["vertexWeights"])[index].asFloat();
											if (fWeight >= 0.001)
												pSkin->setWeight((*memTIte)["name"].asString().c_str(), nVertexIndex, fWeight);
										}
									}
								}
								++memTIte;
							}
						}
					}
					//------------------------------------------------------------------------------

					int nVertexSize = (*meshTer)["Vertices"].size();
					pMesh->MakeVertexBuffer(nVertexSize);
					int vertexIndex = 0;
					while (vertexIndex < nVertexSize)
					{
						float nVerticeX = ((*meshTer)["Vertices"])[vertexIndex].asFloat();
						float nVerticeY = 0.0;
						float nVerticeZ = 0.0;

						nVerticeY = ((*meshTer)["Vertices"])[vertexIndex + 1].asFloat();
						nVerticeZ = ((*meshTer)["Vertices"])[vertexIndex + 2].asFloat();
						*(pMesh->GetVertexBuffer() + vertexIndex + 0) = nVerticeX;
						*(pMesh->GetVertexBuffer() + vertexIndex + 1) = nVerticeY;
						*(pMesh->GetVertexBuffer() + vertexIndex + 2) = nVerticeZ;
						vertexIndex = vertexIndex + 3;

						//顶点信息对比得到最小点和最大点
						min_x = txMath::getMin(min_x, nVerticeX);
						max_x = txMath::getMax(max_x, nVerticeX);
						min_y = txMath::getMax(min_y, nVerticeY);
						max_y = txMath::getMax(max_y, nVerticeY);
						min_z = txMath::getMax(min_z, nVerticeZ);
						max_z = txMath::getMax(max_z, nVerticeZ);
					}
					//设置AABB包围盒信息
					pMesh->SetAABox(txAABox(VECTOR3(min_x, min_y, min_z), VECTOR3(max_x, max_y, max_z)));

					pMesh->MakeNormalBuffer((*meshTer)["Normals"].size());

					auto normalIte = (*meshTer)["Normals"].begin();
					auto normalIteEnd = (*meshTer)["Normals"].end();
					int normalIndex = 0;
					while (normalIte != normalIteEnd)
					{
						*(pMesh->GetNormalBuffer() + normalIndex) = (*normalIte).asFloat();
						++normalIte;
						++normalIndex;
					}

					int nUVSize = (*meshTer)["UVs"].size();
					pMesh->MakeTexCoordBuffer(nUVSize);

					for (int i = 0; i < nUVSize; ++i)
					{
						*(pMesh->GetTexCoordBuffer() + i) = ((*meshTer)["UVs"])[i].asFloat();
					}

					auto faceIte = (*meshTer)["indices"].begin();
					auto faceIteEnd = (*meshTer)["indices"].end();
					while (faceIte != faceIteEnd)
					{
						pSubMesh->pushIndex((*faceIte).asInt());
						++faceIte;
					}

					++meshTer;
				}
			}
		}
	}
	else
	{
		ENGINE_ERROR("error : [txJsonMeshLoader::LoadMeshFile] load[%s] err. info : %s", fileName.c_str(), jRead.getFormattedErrorMessages().c_str());
	}

	// 通知实体模型文件加载完成
	txVector<std::string> paramList;
	paramList.push_back(pEntity->getName());
	paramList.push_back(meshFileName);
	mEngineRoot->sendEvent(EE_DYNAMIC_ENTITY_MODEL_LOADED, paramList);
	paramList.clear();

	if (skeletonFullFileName != EMPTY_STRING)
	{
		loadJsonSkeleton(skeletonFullFileName, pEntity->GetSkeleton());
	}

	// 通知实体骨骼文件加载完成
	paramList.push_back(pEntity->getName());
	paramList.push_back(skeletonFileName);
	mEngineRoot->sendEvent(EE_DYNAMIC_ENTITY_SKELETON_LOADED, paramList);
	paramList.clear();

	// 遍历骨骼所在目录,查找该骨骼的所有动作文件
	std::string meshFileNameNoSuffix = meshFileName.substr(0, meshFileName.find_last_of('.'));
	txVector<std::string> fileList;
	txFileUtility::findFiles(skeletonDir, fileList, "_animation.json");
	// 该实体的动画文件列表,first是动画文件名,second是动画名
	txMap<std::string, std::string> animList;
	int animationFileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < animationFileCount; ++i)
	{
		std::string animationFileName = fileList[i].substr(fileList[i].find_last_of('/') + 1, fileList[i].length() - fileList[i].find_last_of('/') - 1);
		if (animationFileName.length() > meshFileNameNoSuffix.length() && animationFileName.substr(0, meshFileNameNoSuffix.length()) == meshFileNameNoSuffix)
		{
			// 去掉模型名前缀
			std::string animationName = animationFileName.substr(meshFileNameNoSuffix.length() + strlen("_"), animationFileName.length() - meshFileNameNoSuffix.length() - strlen("_"));
			// 再去掉_animation.json后缀
			int lastPos = animationName.find_last_of('_');
			if (lastPos != -1)
			{
				animationName = animationName.substr(0, lastPos);
				animList.insert(fileList[i], animationName);
			}
		}
	}
	END_FOR_STL(fileList);

	// 发送事件通知实体开始加载动画
	paramList.push_back(pEntity->getName());
	paramList.push_back(txStringUtility::intToString(animList.size()));
	mEngineRoot->sendEvent(EE_START_LOAD_ENTITY_ANIMATION, paramList);
	paramList.clear();

	auto iterAnim = animList.begin();
	auto iterAnimEnd = animList.end();
	FOR_STL(animList, ; iterAnim != iterAnimEnd; ++iterAnim)
	{
		txAnimation* animation = pEntity->CreateAnimation(iterAnim->second);
		loadJsonAnimation(iterAnim->first, animation);
		// 通知实体一个动画文件加载完成
		paramList.push_back(pEntity->getName());
		paramList.push_back(iterAnim->second);
		paramList.push_back(iterAnim->first);
		mEngineRoot->sendEvent(EE_ONE_ENTITY_ANIMATION_LOADED, paramList);
		paramList.clear();
	}
	END_FOR_STL(animList);

	// 通知实体动画文件全部加载完成
	paramList.push_back(pEntity->getName());
	paramList.push_back(txStringUtility::intToString(animList.size()));
	mEngineRoot->sendEvent(EE_ENTITY_ALL_ANIMATION_LOADED, paramList);
	paramList.clear();

	// 发出实体加载完成的通知事件
	mEngineRoot->sendEvent(EE_END_DYNAMIC_ENTITY_LOAD, pEntity->getName());
	return ret;
}

bool txLoadUtility::loadBinarySkeletonMesh(const std::string& fileName, txVector<SkinMeshPair>& skinMeshPairList, txEntity* pEntity, const bool& loadTexture, txVector<txTexture*>* textureList)
{
	// 发送事件通知实体开始加载文件,无参数
	mEngineRoot->sendEvent(EE_START_LOAD_DYNAMIC_ENTITY, pEntity->getName());

	int fileSize = 0;
	char* buffer = txFileUtility::openBinaryFile(fileName, &fileSize);
	txSerializer serializer(buffer, fileSize);

	std::string meshFileName = fileName.substr(fileName.find_last_of('/') + 1, fileName.length() - fileName.find_last_of('/') - 1);
	pEntity->setMeshFileName(meshFileName);
	std::string skeletonDir = fileName.substr(0, fileName.find_last_of('/'));
	
	// 读取骨骼文件名
	char skeletonFile[128];
	serializer.readString(skeletonFile, 128);
	// 记录下骨骼的文件名
	std::string skeletonFileName = skeletonFile;

	// 读取材质数量
	int materialCount;
	serializer.read(materialCount);
	txVector<txMaterial> materials;
	for(int i = 0; i < materialCount; ++i)
	{
		int nMaterialIndex;
		serializer.read(nMaterialIndex);
		bool wireframe;
		serializer.read(wireframe);
		char shadingMode[128];
		serializer.readString(shadingMode, 128);
		char materialName[128];
		serializer.readString(materialName, 128);
		float mapLightAnisotropy;
		serializer.read(mapLightAnisotropy);
		char blending[128];
		serializer.readString(blending, 128);
		VECTOR3 colorDiffuse;
		serializer.readVector3(colorDiffuse);
		bool depthTest;
		serializer.read(depthTest);
		VECTOR3 colorEmissive;
		serializer.readVector3(colorEmissive);
		bool depthWrite;
		serializer.read(depthWrite);
		float opacity;
		serializer.read(opacity);
		VECTOR3 colorAmbient;
		serializer.readVector3(colorAmbient);
		char diffuseTexture[128];
		serializer.readString(diffuseTexture, 128);
		char wrapU[128];
		serializer.readString(wrapU, 128);
		char wrapV[128];
		serializer.readString(wrapV, 128);
		int repeatU;
		serializer.read(repeatU);
		int repeatV;
		serializer.read(repeatV);
		bool transparent;
		serializer.read(transparent);
		int uvLayer;
		serializer.read(uvLayer);

		txTexture* pTexture = mTextureManager->createTexture(diffuseTexture);
		if (loadTexture)
		{
			pTexture->initTexture();
		}
		if (textureList != NULL)
		{
			textureList->push_back(pTexture);
		}

		txMaterial material;
		material.setMaterialIndex(nMaterialIndex);
		material.setWireframe(wireframe);
		material.setShadingMode(shadingMode);
		material.setName(materialName);
		material.setAnisotropy(mapLightAnisotropy);
		material.setBlending(blending);
		material.setDiffuseColour(colorDiffuse);
		material.setDepthTest(depthTest);
		material.setEmissiveColour(colorEmissive);
		material.setDepthWrite(depthWrite);
		material.setOpacity(opacity);
		material.setAmbientColour(colorAmbient);
		material.setDiffuseTexture(pTexture->getName());
		material.setWrapU(wrapU);
		material.setWrapV(wrapV);
		material.setRepeatU(repeatU);
		material.setRepeatV(repeatV);
		material.setTransparent(transparent);
		material.setUVLayer(uvLayer);
		materials.push_back(material);
	}

	// 读取mesh的数量
	int meshCount;
	serializer.read(meshCount);
	for (int i = 0; i < meshCount; ++i)
	{
		txMesh* pMesh = TRACE_NEW(txMesh, pMesh);
		txSubMesh* pSubMesh = pMesh->CreateSubMesh();
		txSkin* pSkin = TRACE_NEW(txSkin, pSkin);
		SkinMeshPair skinMeshPair;
		skinMeshPair.mMesh = pMesh;
		skinMeshPair.mSkin = pSkin;
		skinMeshPairList.push_back(skinMeshPair);
				
		// 读取材质索引
		int nMaterialIndex;
		serializer.read(nMaterialIndex);
		pSubMesh->setMaterial(materials[nMaterialIndex]);

		// 读取蒙皮骨骼数量
		int skinBoneCount;
		serializer.read(skinBoneCount);
		for (int j = 0; j < skinBoneCount; ++j)
		{
			// 读取蒙皮骨骼名字
			char skinBoneName[128];
			serializer.readString(skinBoneName, 128);
			// 读取偏移矩阵
			MATRIX4 offsetMatrix;
			serializer.readMatrix4(offsetMatrix);
			pSkin->setOffsetMatrix(skinBoneName, offsetMatrix);
			// 读取顶点索引数量
			int indexCount;
			serializer.read(indexCount);
			for (int k = 0; k < indexCount; ++k)
			{
				// 读取顶点索引
				int nVertexIndex;
				serializer.read(nVertexIndex);
				// 读取顶点权重
				float fWeight;
				serializer.read(fWeight);
				if (fWeight >= 0.001)
				{
					pSkin->setWeight(skinBoneName, nVertexIndex, fWeight);
				}
			}
		}
		// 读取顶点位置信息
		int vertexCount;
		serializer.read(vertexCount);
		pMesh->MakeVertexBuffer(vertexCount);
		GLfloat* vertexBuffer = pMesh->GetVertexBuffer();
		int vertexBufferSize = sizeof(vertexBuffer[0]) * vertexCount;
		serializer.readBuffer((char*)(vertexBuffer), vertexBufferSize, vertexBufferSize);

		// 读取法线
		int normalCount;
		serializer.read(normalCount);
		pMesh->MakeNormalBuffer(normalCount);
		GLfloat* normalBuffer = pMesh->GetNormalBuffer();
		int normalBufferSize = sizeof(normalBuffer[0]) * normalCount;
		serializer.readBuffer((char*)(normalBuffer), normalBufferSize, normalBufferSize);

		// 读取纹理坐标
		int uvCount;
		serializer.read(uvCount);
		pMesh->MakeTexCoordBuffer(uvCount);
		GLfloat* uvBuffer = pMesh->GetTexCoordBuffer();
		int uvBufferSize = sizeof(uvBuffer[0]) * uvCount;
		serializer.readBuffer((char*)(uvBuffer), uvBufferSize, uvBufferSize);

		// 读取索引数据
		int indexCount;
		serializer.read(indexCount);
		pSubMesh->makeIndicesSize(indexCount);
		GLushort* tempIndexBuffer = pSubMesh->getIndexPtr();
		serializer.readBuffer((char*)tempIndexBuffer, indexCount * sizeof(GLushort), indexCount * sizeof(GLushort));

		// 读取包围盒数据
		VECTOR3 aabbMin, aabbMax;
		serializer.readVector3(aabbMin);
		serializer.readVector3(aabbMax);
		pMesh->SetAABox(txAABox(aabbMin, aabbMax));
	}

	TRACE_DELETE_ARRAY(buffer);

	// 通知实体模型文件加载完成
	txVector<std::string> paramList;
	paramList.push_back(pEntity->getName());
	paramList.push_back(meshFileName);
	mEngineRoot->sendEvent(EE_DYNAMIC_ENTITY_MODEL_LOADED, paramList);
	paramList.clear();

	std::string skeletonFullFileName = skeletonDir + "/" + skeletonFileName;
	loadBinarySkeleton(skeletonFullFileName, pEntity->GetSkeleton());

	// 通知实体骨骼文件加载完成
	paramList.push_back(pEntity->getName());
	paramList.push_back(skeletonFileName);
	mEngineRoot->sendEvent(EE_DYNAMIC_ENTITY_SKELETON_LOADED, paramList);
	paramList.clear();

	// 遍历骨骼所在目录,查找该骨骼的所有动作文件
	std::string meshFileNameNoSuffix = meshFileName.substr(0, meshFileName.find_last_of('.'));
	txVector<std::string> fileList;
	txFileUtility::findFiles(skeletonDir, fileList, "_animation.animation");
	// 该实体的动画文件列表,first是动画文件名,second是动画名
	txMap<std::string, std::string> animList;
	int animationFileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < animationFileCount; ++i)
	{
		std::string animationFileName = fileList[i].substr(fileList[i].find_last_of('/') + 1, fileList[i].length() - fileList[i].find_last_of('/') - 1);
		if (animationFileName.length() > meshFileNameNoSuffix.length() && animationFileName.substr(0, meshFileNameNoSuffix.length()) == meshFileNameNoSuffix)
		{
			// 去掉模型名前缀
			std::string animationName = animationFileName.substr(meshFileNameNoSuffix.length() + strlen("_"), animationFileName.length() - meshFileNameNoSuffix.length() - strlen("_"));
			// 再去掉_animation.animation后缀
			int lastPos = animationName.find_last_of('_');
			if (lastPos != -1)
			{
				animationName = animationName.substr(0, lastPos);
				animList.insert(fileList[i], animationName);
			}
		}
	}
	END_FOR_STL(fileList);

	// 发送事件通知实体开始加载动画
	paramList.push_back(pEntity->getName());
	paramList.push_back(txStringUtility::intToString(animList.size()));
	mEngineRoot->sendEvent(EE_START_LOAD_ENTITY_ANIMATION, paramList);
	paramList.clear();

	auto iterAnim = animList.begin();
	auto iterAnimEnd = animList.end();
	FOR_STL(animList, ; iterAnim != iterAnimEnd; ++iterAnim)
	{
		txAnimation* animation = pEntity->CreateAnimation(iterAnim->second);
		if (!loadBinaryAnimation(iterAnim->first, animation))
		{
			break;
		}
		// 通知实体一个动画文件加载完成
		paramList.push_back(pEntity->getName());
		paramList.push_back(iterAnim->second);
		paramList.push_back(iterAnim->first);
		mEngineRoot->sendEvent(EE_ONE_ENTITY_ANIMATION_LOADED, paramList);
		paramList.clear();
	}
	END_FOR_STL(animList);

	// 通知实体动画文件全部加载完成
	paramList.push_back(pEntity->getName());
	paramList.push_back(txStringUtility::intToString(animList.size()));
	mEngineRoot->sendEvent(EE_ENTITY_ALL_ANIMATION_LOADED, paramList);
	paramList.clear();

	// 发出实体加载完成的通知事件
	mEngineRoot->sendEvent(EE_END_DYNAMIC_ENTITY_LOAD, pEntity->getName());
	return true;
}

bool txLoadUtility::loadMeshFromFile(const std::string& fileName, txMesh* pMesh, const bool& loadTexture, txVector<txTexture*>* textureList)
{
	int dotPos = fileName.find_last_of('.');
	std::string suffix = fileName.substr(dotPos + 1, fileName.length() - dotPos - 1);
	if (suffix == "json")
	{
		return loadJsonMesh(fileName, pMesh, loadTexture, textureList);
	}
	else if (suffix == "mesh")
	{
		return loadBinaryMesh(fileName, pMesh, loadTexture, textureList);
	}
	return false;
}

// 必须是一个子集有一个材质,不允许多个子集公用一个材质,也不允许某些子集没有材质
bool txLoadUtility::loadJsonMesh(const std::string& fileName, txMesh* pMesh, const bool& loadTexture, txVector<txTexture*>* textureList)
{
	Json::Reader jRead;
    Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::objectValue)
		{
			//-------------------------------load material info---------------------------
			//Load the Material info in the Mesh (index and the material name relationship)
			if (!arrVal["materials"].isNull())
			{
				auto iTer = arrVal["materials"].begin();
				auto iTerEnd = arrVal["materials"].end();
				while (iTer != iTerEnd)
				{
					txSubMesh* pSubMesh = pMesh->CreateSubMesh();

					int nMaterialIndex = (*iTer)["DbgIndex"].asInt();
					bool wireframe = (*iTer)["wireframe"].asBool();
					std::string shadingMode = (*iTer)["shading"].asString();
					std::string materialName = (*iTer)["DbgName"].asString();
					float mapLightAnisotropy = (*iTer)["mapLightAnisotropy"].asFloat();
					std::string blending = (*iTer)["blending"].asString();
					VECTOR3 colorDiffuse = VECTOR3((*iTer)["colorDiffuse"][0].asFloat(), (*iTer)["colorDiffuse"][1].asFloat(), (*iTer)["colorDiffuse"][2].asFloat());
					bool depthTest = (*iTer)["depthTest"].asBool();
					VECTOR3 colorEmissive = VECTOR3((*iTer)["colorEmissive"][0].asFloat(), (*iTer)["colorEmissive"][1].asFloat(), (*iTer)["colorEmissive"][2].asFloat());
					bool depthWrite = (*iTer)["depthWrite"].asBool();
					float opacity = (*iTer)["opacity"].asFloat();
					VECTOR3 colorAmbient = VECTOR3((*iTer)["colorAmbient"][0].asFloat(), (*iTer)["colorAmbient"][1].asFloat(), (*iTer)["colorAmbient"][2].asFloat());
					std::string diffuseTexture = (*iTer)["mapLight"].asString();
					std::string wrapU = (*iTer)["mapLightWrap"][0].asString();
					std::string wrapV = (*iTer)["mapLightWrap"][1].asString();
					int repeatU = (*iTer)["mapLightRepeat"][0].asInt();
					int repeatV = (*iTer)["mapLightRepeat"][1].asInt();
					bool transparent = (*iTer)["DbgIndex"].asBool();
					int uvLayer = (*iTer)["DbgIndex"].asInt();

					diffuseTexture = P_MODEL_TEXTURE_PATH + diffuseTexture.substr(0, diffuseTexture.find_last_of('.')) + ".swi";
					txTexture* pTexture = mTextureManager->createTexture(diffuseTexture);
					if (loadTexture)
					{
						pTexture->initTexture();
					}
					if (textureList != NULL)
					{
						textureList->push_back(pTexture);
					}

					txMaterial material;
					material.setMaterialIndex(nMaterialIndex);
					material.setWireframe(wireframe);
					material.setShadingMode(shadingMode);
					material.setName(materialName);
					material.setAnisotropy(mapLightAnisotropy);
					material.setBlending(blending);
					material.setDiffuseColour(colorDiffuse);
					material.setDepthTest(depthTest);
					material.setEmissiveColour(colorEmissive);
					material.setDepthWrite(depthWrite);
					material.setOpacity(opacity);
					material.setAmbientColour(colorAmbient);
					material.setDiffuseTexture(pTexture->getName());
					material.setWrapU(wrapU);
					material.setWrapV(wrapV);
					material.setRepeatU(repeatU);
					material.setRepeatV(repeatV);
					material.setTransparent(transparent);
					material.setUVLayer(uvLayer);

					pSubMesh->setMaterial(material);
					pSubMesh->setMaterialIndex(nMaterialIndex);
					++iTer;
				}
			}
			
			int nVertices = arrVal["vertices"].size();
			pMesh->MakeVertexBuffer(nVertices);
			for (int i = 0; i < nVertices; ++i)
			{
				*(pMesh->GetVertexBuffer() + i) = (arrVal["vertices"])[i].asFloat();
			}

			int nUVs = arrVal["uvs"].size();
			pMesh->MakeTexCoordBuffer(nUVs);
			for (int i = 0; i < nUVs; ++i)
			{
				*(pMesh->GetTexCoordBuffer() + i) = (arrVal["uvs"])[i].asFloat();
			}

			int nNormals = arrVal["normals"].size();
			pMesh->MakeNormalBuffer(nNormals);
			for (int i = 0; i < nNormals; ++i)
			{
				*(pMesh->GetNormalBuffer() + i) = (arrVal["normals"])[i].asFloat();
			}
			float min_x = 0.0, min_y = 0.0, min_z = 0.0, max_x = 0.0, max_y = 0.0, max_z = 0.0;
			int nFaces = arrVal["faces"].size();
			for (int i = 0; i < nFaces; i += 3)
			{
				int nMaterialIndex = (arrVal["faces"])[i++].asInt(); //material index
				txSubMesh* pSubMesh = pMesh->getMaterialSubMesh(nMaterialIndex);
				for (int j = 0; j < 3; ++j)
				{
					int nVertice = (arrVal["faces"])[i + j].asInt();
					float nVerticeX = (arrVal["vertices"])[nVertice * 3 + 0].asFloat();
					float nVerticeY = (arrVal["vertices"])[nVertice * 3 + 1].asFloat();
					float nVerticeZ = (arrVal["vertices"])[nVertice * 3 + 2].asFloat();

					//顶点信息对比得到最小点和最大点
					min_x = txMath::getMin(min_x, nVerticeX);
					max_x = txMath::getMax(max_x, nVerticeX);
					min_y = txMath::getMin(min_y, nVerticeY);
					max_y = txMath::getMax(max_y, nVerticeY);
					min_z = txMath::getMin(min_z, nVerticeZ);
					max_z = txMath::getMax(max_z, nVerticeZ);
					pSubMesh->pushIndex(nVertice);
				}   
			}

			//设置AABB包围盒信息
			pMesh->SetAABox(txAABox(VECTOR3(min_x, min_y, min_z), VECTOR3(max_x, max_y, max_z)));
		}
	}
	else
	{
		ENGINE_ERROR("error : [txJsonMeshLoader::LoadMeshFile] load[%s] err. info : %s", fileName.c_str(), jRead.getFormattedErrorMessages().c_str());
	}
	return ret;
}

bool txLoadUtility::loadBinaryMesh(const std::string& fileName, txMesh* pMesh, const bool& loadTexture, txVector<txTexture*>* textureList)
{
	int fileSize = 0;
	char* buffer = txFileUtility::openBinaryFile(fileName, &fileSize);
	txSerializer serializer(buffer, fileSize);
	// 读取材质数量
	int materialCount;
	serializer.read(materialCount);
	for (int i = 0; i < materialCount; ++i)
	{
		txSubMesh* pSubMesh = pMesh->CreateSubMesh();
		int nMaterialIndex;
		serializer.read(nMaterialIndex);
		bool wireframe;
		serializer.read(wireframe);
		char shadingMode[128];
		serializer.readString(shadingMode, 128);
		char materialName[128];
		serializer.readString(materialName, 128);
		float mapLightAnisotropy;
		serializer.read(mapLightAnisotropy);
		char blending[128];
		serializer.readString(blending, 128);
		VECTOR3 colorDiffuse;
		serializer.readVector3(colorDiffuse);
		bool depthTest;
		serializer.read(depthTest);
		VECTOR3 colorEmissive;
		serializer.readVector3(colorEmissive);
		bool depthWrite;
		serializer.read(depthWrite);
		float opacity;
		serializer.read(opacity);
		VECTOR3 colorAmbient;
		serializer.readVector3(colorAmbient);
		char diffuseTexture[128];
		serializer.readString(diffuseTexture, 128);
		char wrapU[128];
		serializer.readString(wrapU, 128);
		char wrapV[128];
		serializer.readString(wrapV, 128);
		int repeatU;
		serializer.read(repeatU);
		int repeatV;
		serializer.read(repeatV);
		bool transparent;
		serializer.read(transparent);
		int uvLayer;
		serializer.read(uvLayer);

		txTexture* pTexture = mTextureManager->createTexture(diffuseTexture);
		if (loadTexture)
		{
			pTexture->initTexture();
		}
		if (textureList != NULL)
		{
			textureList->push_back(pTexture);
		}

		txMaterial material;
		material.setMaterialIndex(nMaterialIndex);
		material.setWireframe(wireframe);
		material.setShadingMode(shadingMode);
		material.setName(materialName);
		material.setAnisotropy(mapLightAnisotropy);
		material.setBlending(blending);
		material.setDiffuseColour(colorDiffuse);
		material.setDepthTest(depthTest);
		material.setEmissiveColour(colorEmissive);
		material.setDepthWrite(depthWrite);
		material.setOpacity(opacity);
		material.setAmbientColour(colorAmbient);
		material.setDiffuseTexture(pTexture->getName());
		material.setWrapU(wrapU);
		material.setWrapV(wrapV);
		material.setRepeatU(repeatU);
		material.setRepeatV(repeatV);
		material.setTransparent(transparent);
		material.setUVLayer(uvLayer);

		pSubMesh->setMaterial(material);
		pSubMesh->setMaterialIndex(nMaterialIndex);
	}

	// 读取顶点位置
	int nVertices;
	serializer.read(nVertices);
	pMesh->MakeVertexBuffer(nVertices);
	GLfloat* vertexBuffer = pMesh->GetVertexBuffer();
	int vertexBufferSize = sizeof(vertexBuffer[0]) * nVertices;
	serializer.readBuffer((char*)(vertexBuffer), vertexBufferSize, vertexBufferSize);
	
	// 读取纹理坐标
	int nUVs;
	serializer.read(nUVs);
	pMesh->MakeTexCoordBuffer(nUVs);
	GLfloat* texBuffer = pMesh->GetTexCoordBuffer();
	int texBufferSize = sizeof(texBuffer[0]) * nUVs;
	serializer.readBuffer((char*)(texBuffer), texBufferSize, texBufferSize);

	// 读取法线
	int nNormals;
	serializer.read(nNormals);
	pMesh->MakeNormalBuffer(nNormals);
	GLfloat* normalBuffer = pMesh->GetNormalBuffer();
	int normalBufferSize = sizeof(normalBuffer[0]) * nNormals;
	serializer.readBuffer((char*)(normalBuffer), normalBufferSize, normalBufferSize);

	// 读取索引缓冲数量
	int indexBufferCount;
	serializer.read(indexBufferCount);
	for (int i = 0; i < indexBufferCount; ++i)
	{
		// 读取材质索引
		int materialIndex;
		serializer.read(materialIndex);
		// 读索引数量
		int indexCount;
		serializer.read(indexCount);
		txSubMesh* pSubMesh = pMesh->getMaterialSubMesh(materialIndex);
		pSubMesh->makeIndicesSize(indexCount);
		serializer.readBuffer((char*)(pSubMesh->getIndexPtr()), indexCount * sizeof(GLushort), indexCount * sizeof(GLushort));
	}

	//设置AABB包围盒信息
	VECTOR3 aabbMin, aabbMax;
	serializer.readVector3(aabbMin);
	serializer.readVector3(aabbMax);
	pMesh->SetAABox(txAABox(aabbMin, aabbMax));

	TRACE_DELETE_ARRAY(buffer);
	return true;
}

bool txLoadUtility::loadJsonSkeleton(const std::string& fileName, txSkeleton* pSkeleton)
{
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::arrayValue)
		{
			auto valueIte = arrVal.begin();
			auto valueIteEnd = arrVal.end();
			while (valueIte != valueIteEnd)
			{
				std::string boneName = (*valueIte)["name"].asString();
				std::string parentName = (*valueIte)["parentName"].asString();
				txBone* pBone = pSkeleton->CreateBone(boneName.c_str());
				pBone->SetParentName(parentName.c_str());

				if (!(*valueIte)["position"].isNull())
				{
					pBone->SetPosition(VECTOR3(((*valueIte)["position"])[0].asFloat(), ((*valueIte)["position"])[1].asFloat(), ((*valueIte)["position"])[2].asFloat()));
				}
				if (!(*valueIte)["scale"].isNull())
				{
					pBone->SetScale(VECTOR3(((*valueIte)["scale"])[0].asFloat(), ((*valueIte)["scale"])[1].asFloat(), ((*valueIte)["scale"])[2].asFloat()));
				}
				if (!(*valueIte)["rotation"].isNull())
				{
					MATRIX3 rotateMatrix = txMath::quatToMatrix3(QUATERNION(((*valueIte)["rotation"])[0].asFloat(), ((*valueIte)["rotation"])[1].asFloat(), ((*valueIte)["rotation"])[2].asFloat(), ((*valueIte)["rotation"])[3].asFloat()));
					pBone->SetRotationMatrix(rotateMatrix);
				}
				++valueIte;
			}
			pSkeleton->SetBonesParent();
		}
	}
	else
	{
		ENGINE_ERROR("error : %s", jRead.getFormattedErrorMessages().c_str());
	}
	return ret;
}

bool txLoadUtility::loadBinarySkeleton(const std::string& fileName, txSkeleton* pSkeleton)
{
	int fileSize = 0;
	char* buffer = txFileUtility::openBinaryFile(fileName, &fileSize);
	txSerializer serializer(buffer, fileSize);
	// 读取骨头数量
	int boneCount;
	serializer.read(boneCount);
	for (int i = 0; i < boneCount; ++i)
	{
		// 读取骨头名
		char boneName[128];
		serializer.readString(boneName, 128);
		// 读取父骨头名
		char parentName[128];
		serializer.readString(parentName, 128);
		txBone* pBone = pSkeleton->CreateBone(boneName);
		pBone->SetParentName(parentName);
		// 读取骨头位置
		VECTOR3 pos;
		serializer.readVector3(pos);
		pBone->SetPosition(pos);
		// 读取骨头缩放
		VECTOR3 scale;
		serializer.readVector3(scale);
		pBone->SetScale(scale);
		// 读取骨头旋转
		MATRIX3 rotateMatrix;
		serializer.readMatrix3(rotateMatrix);
		pBone->SetRotationMatrix(rotateMatrix);
	}
	pSkeleton->SetBonesParent();

	TRACE_DELETE_ARRAY(buffer);
	return true;
}

bool txLoadUtility::loadJsonAnimation(const std::string& fileName, txAnimation* pAnimation)
{
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::arrayValue)
		{
			// 一般一个动画文件只存储一个动作，取数组第一个元素
			std::string animationName = (arrVal[0])["name"].asString();
			auto valueIte = (arrVal[0])["hierarchy"].begin();
			auto valueIteEnd = (arrVal[0])["hierarchy"].end();
			while (valueIte != valueIteEnd)
			{
				std::string boneName = (*valueIte)["bone"].asString();
				txAnimationNode* pNodeAnim = pAnimation->GetNodeAnim(boneName.c_str());

				//position information
				auto positionIte = ((*valueIte)["keys"])["position"].begin();
				auto positionIteEnd = ((*valueIte)["keys"])["position"].end();
				while (positionIte != positionIteEnd)
				{
					float fTime = (float)(*positionIte)["time"].asDouble();
					VECTOR3 framePos = VECTOR3(((*positionIte)["pos"])[0].asFloat(), ((*positionIte)["pos"])[1].asFloat(), ((*positionIte)["pos"])[2].asFloat());
					pNodeAnim->pushPositionKeyFrame(fTime, framePos);
					++positionIte;
				}

				//scale information
				auto scaleIte = ((*valueIte)["keys"])["scale"].begin();
				auto scaleIteEnd = ((*valueIte)["keys"])["scale"].end();
				while (scaleIte != scaleIteEnd)
				{
					float fTime = (float)(*scaleIte)["time"].asDouble();
					VECTOR3 frameScl = VECTOR3(((*scaleIte)["scl"])[0].asFloat(), ((*scaleIte)["scl"])[1].asFloat(), ((*scaleIte)["scl"])[2].asFloat());
					pNodeAnim->pushScaleKeyFrame(fTime, frameScl);
					++scaleIte;
				}

				//rotation information
				auto rotationIte = ((*valueIte)["keys"])["rotation"].begin();
				auto rotationIteEnd = ((*valueIte)["keys"])["rotation"].end();
				while (rotationIte != rotationIteEnd)
				{
					float fTime = (float)(*rotationIte)["time"].asDouble();
					QUATERNION rotateQuat(((*rotationIte)["rot"])[0].asFloat(), ((*rotationIte)["rot"])[1].asFloat(), ((*rotationIte)["rot"])[2].asFloat(), ((*rotationIte)["rot"])[3].asFloat());
					pNodeAnim->pushRotationKeyFrame(fTime, rotateQuat);
					++rotationIte;
				}
				++valueIte;
			}
			pAnimation->FinishCreateNodeAnim();
		}
	}
	else
	{
		ENGINE_ERROR("error : [txJsonMeshLoader::LoadMeshAnimation] load[%s] err. info : %s", fileName.c_str(), jRead.getFormattedErrorMessages().c_str());
	}
	return ret;
}

bool txLoadUtility::loadBinaryAnimation(const std::string& fileName, txAnimation* pAnimation)
{
	int fileSize = 0;
	char* buffer = txFileUtility::openBinaryFile(fileName, &fileSize);
	txSerializer serializer(buffer, fileSize);

	// 读取动作名,但是这里不使用这个动作名
	char animationName[128];
	serializer.readString(animationName, 128);
	// 读取骨头数量
	int boneCount;
	serializer.read(boneCount);
	for (int i = 0; i < boneCount; ++i)
	{
		// 读取骨头名
		char boneName[128];
		serializer.readString(boneName, 128);
		txAnimationNode* pNodeAnim = pAnimation->GetNodeAnim(boneName);

		// 读取位置数量
		int posCount;
		serializer.read(posCount);
		std::pair<float*, VECTOR3*> posKeyFrame = pNodeAnim->makePositionBuffer(posCount);
		serializer.readBuffer((char*)(posKeyFrame.first), posCount * sizeof(float), posCount * sizeof(float));
		serializer.readBuffer((char*)(posKeyFrame.second), posCount * sizeof(float) * 3, posCount * sizeof(float) * 3);

		// 读取缩放数量
		int scaleCount;
		serializer.read(scaleCount);
		std::pair<float*, VECTOR3*> scaleKeyFrame = pNodeAnim->makeScaleBuffer(scaleCount);
		serializer.readBuffer((char*)(scaleKeyFrame.first), scaleCount * sizeof(float), scaleCount * sizeof(float));
		serializer.readBuffer((char*)(scaleKeyFrame.second), scaleCount * sizeof(float) * 3, scaleCount * sizeof(float) * 3);

		// 读取旋转数量
		int rotCount;
		serializer.read(rotCount);
		std::pair<float*, QUATERNION*> rotKeyFrame = pNodeAnim->makeRotationBuffer(rotCount);
		serializer.readBuffer((char*)(rotKeyFrame.first), rotCount * sizeof(float), rotCount * sizeof(float));
		serializer.readBuffer((char*)(rotKeyFrame.second), rotCount * sizeof(float) * 4, rotCount * sizeof(float) * 4);
	}
	pAnimation->FinishCreateNodeAnim();

	TRACE_DELETE_ARRAY(buffer);
	return true;
}

bool txLoadUtility::jsonSceneFileToBinarySceneFile(const std::string& fileName, std::string& newFileName, const bool& transMeshFile)
{
	// 先将所有的\转换为/
	std::string jsonFileName = fileName;
	txStringUtility::rightToLeft(jsonFileName);
	std::string sceneName;
	int sceneFileDotPos = jsonFileName.find_last_of('.');
	int sceneFilePos0 = jsonFileName.find_last_of('/');
	if (sceneFileDotPos != -1 && sceneFilePos0 != -1)
	{
		sceneName = jsonFileName.substr(sceneFilePos0 + 1, sceneFileDotPos - sceneFilePos0 - 1);
	}
	std::string sceneMeshDir = sceneName + "/staticMesh/";

	txSerializer serializer;
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::objectValue)
		{
			// 读取geometry,获取模型文件名
			txMap<std::string, std::string> geometryMap;
			auto geometrysIte = arrVal["geometries"].begin();
			auto geometrysIteEnd = arrVal["geometries"].end();
			while (geometrysIte != geometrysIteEnd)
			{
				std::string key = (*geometrysIte)["uuid"].asString();
				std::string value = (*geometrysIte)["url"].asString();
				geometryMap.insert(key, value);
				++geometrysIte;
			}

			// 记录下加载实体之前的加载进度
			if ((arrVal["object"])["children"].type() == Json::arrayValue)
			{
				int objectCount = (arrVal["object"])["children"].size();
				// 写入物体的数量
				serializer.write(objectCount);
				auto valueIte = (arrVal["object"])["children"].begin();
				auto valueIteEnd = (arrVal["object"])["children"].end();
				while (valueIte != valueIteEnd)
				{
					std::string objectType = (*valueIte)["type"].asString();
					std::string objectName = (*valueIte)["name"].asString();
					VECTOR3 pos = VECTOR3(((*valueIte)["position"])[0].asFloat(), ((*valueIte)["position"])[1].asFloat(), ((*valueIte)["position"])[2].asFloat());
					MATRIX3 rotateMatrix = txMath::quatToMatrix3(QUATERNION(VECTOR3(((*valueIte)["rotation"])[0].asFloat(), ((*valueIte)["rotation"])[1].asFloat(), ((*valueIte)["rotation"])[2].asFloat())));
					VECTOR3 scale = VECTOR3(((*valueIte)["scale"])[0].asFloat(), ((*valueIte)["scale"])[1].asFloat(), ((*valueIte)["scale"])[2].asFloat());
					if (objectType == "Mesh")
					{
						// 写入物体类型
						serializer.writeString(objectType.c_str());
						// 写入物体名称
						serializer.writeString(objectName.c_str());
						// 写入位置
						serializer.writeVector3(pos);
						// 写入旋转
						serializer.writeMatrix3(rotateMatrix);
						// 写入缩放
						serializer.writeVector3(scale);
						// 写入文件名,需要将模型文件名的后缀改为.mesh
						std::string meshFile;
						auto iterGeom = geometryMap.find(((*valueIte))["geometry"].asString());
						if (iterGeom != geometryMap.end())
						{
							meshFile = iterGeom->second;
						}
						// 如果需要转换模型文件,则先将模型文件转换为二进制的
						if (transMeshFile)
						{
							std::string newFileName;
							jsonMeshFileToBinaryMeshFile(sceneMeshDir + meshFile, newFileName);
						}
						meshFile = meshFile.substr(0, meshFile.find_last_of('.')) + ".mesh";
						serializer.writeString(meshFile.c_str());
					}
					++valueIte;
				}
			}
		}
	}
	else
	{
		ENGINE_ERROR("error : [txSceneManager::LoadSceneFromFile] %s", jRead.getFormattedErrorMessages().c_str());
	}

	newFileName = jsonFileName.substr(0, jsonFileName.find_last_of('.')) + ".scene";
	serializer.writeToFile(newFileName);
	return ret;
}

bool txLoadUtility::jsonMeshFileToBinaryMeshFile(const std::string& fullFileName, std::string& newFileName)
{
	txSerializer serializer;
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fullFileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::objectValue)
		{
			//-------------------------------load material info---------------------------
			//Load the Material info in the Mesh (index and the material name relationship)
			if (!arrVal["materials"].isNull())
			{
				// 写入材质
				// 材质数量
				int materialCount = arrVal["materials"].size();
				serializer.write(materialCount);
				auto iTer = arrVal["materials"].begin();
				auto iTerEnd = arrVal["materials"].end();
				while (iTer != iTerEnd)
				{
					int nMaterialIndex = (*iTer)["DbgIndex"].asInt();
					bool wireframe = (*iTer)["wireframe"].asBool();
					std::string shadingMode = (*iTer)["shading"].asString();
					std::string materialName = (*iTer)["DbgName"].asString();
					float mapLightAnisotropy = (*iTer)["mapLightAnisotropy"].asFloat();
					std::string blending = (*iTer)["blending"].asString();
					VECTOR3 colorDiffuse = VECTOR3((*iTer)["colorDiffuse"][0].asFloat(), (*iTer)["colorDiffuse"][1].asFloat(), (*iTer)["colorDiffuse"][2].asFloat());
					bool depthTest = (*iTer)["depthTest"].asBool();
					VECTOR3 colorEmissive = VECTOR3((*iTer)["colorEmissive"][0].asFloat(), (*iTer)["colorEmissive"][1].asFloat(), (*iTer)["colorEmissive"][2].asFloat());
					bool depthWrite = (*iTer)["depthWrite"].asBool();
					float opacity = (*iTer)["opacity"].asFloat();
					VECTOR3 colorAmbient = VECTOR3((*iTer)["colorAmbient"][0].asFloat(), (*iTer)["colorAmbient"][1].asFloat(), (*iTer)["colorAmbient"][2].asFloat());
					std::string diffuseTexture = (*iTer)["mapLight"].asString();
					diffuseTexture = P_MODEL_TEXTURE_PATH + diffuseTexture.substr(0, diffuseTexture.find_last_of('.')) + ".swi";
					std::string wrapU = (*iTer)["mapLightWrap"][0].asString();
					std::string wrapV = (*iTer)["mapLightWrap"][1].asString();
					int repeatU = (*iTer)["mapLightRepeat"][0].asInt();
					int repeatV = (*iTer)["mapLightRepeat"][1].asInt();
					bool transparent = (*iTer)["DbgIndex"].asBool();
					int uvLayer = (*iTer)["DbgIndex"].asInt();

					// 材质属性
					serializer.write(nMaterialIndex);
					serializer.write(wireframe);
					serializer.writeString(shadingMode.c_str());
					serializer.writeString(materialName.c_str());
					serializer.write(mapLightAnisotropy);
					serializer.writeString(blending.c_str());
					serializer.writeVector3(colorDiffuse);
					serializer.write(depthTest);
					serializer.writeVector3(colorEmissive);
					serializer.write(depthWrite);
					serializer.write(opacity);
					serializer.writeVector3(colorAmbient);
					serializer.writeString(diffuseTexture.c_str());
					serializer.writeString(wrapU.c_str());
					serializer.writeString(wrapV.c_str());
					serializer.write(repeatU);
					serializer.write(repeatV);
					serializer.write(transparent);
					serializer.write(uvLayer);
					++iTer;
				}
			}
			else
			{
				serializer.write(0);
			}

			// 写入顶点位置
			int nVertices = arrVal["vertices"].size();
			// 写入顶点数量(实际是float的数量)
			serializer.write(nVertices);
			for (int i = 0; i < nVertices; ++i)
			{
				serializer.write((arrVal["vertices"])[i].asFloat());
			}

			// 写入纹理坐标
			int nUVs = arrVal["uvs"].size();
			// 写入纹理坐标数量(实际是float的数量)
			serializer.write(nUVs);
			for (int i = 0; i < nUVs; ++i)
			{
				serializer.write((arrVal["uvs"])[i].asFloat());
			}

			// 写入法线
			int nNormals = arrVal["normals"].size();
			// 写入法线数量(实际是float的数量)
			serializer.write(nNormals);
			for (int i = 0; i < nNormals; ++i)
			{
				serializer.write((arrVal["normals"])[i].asFloat());
			}

			// 写入三角面,需要先重新排列数据格式,然后再写入
			// 三角面的数量(实际是int的数量)
			int nFaces = arrVal["faces"].size();
			txMap<int, txVector<GLushort> > tempIndexBuffer;
			float min_x = 0.0, min_y = 0.0, min_z = 0.0, max_x = 0.0, max_y = 0.0, max_z = 0.0;
			for (int i = 0; i < nFaces; i += 3)
			{
				int nMaterialIndex = (arrVal["faces"])[i++].asInt(); //material index
				auto iterIndexBuffer = tempIndexBuffer.find(nMaterialIndex);
				if (iterIndexBuffer == tempIndexBuffer.end())
				{
					tempIndexBuffer.insert(nMaterialIndex, txVector<GLushort>());
					iterIndexBuffer = tempIndexBuffer.find(nMaterialIndex);
				}
				for (int j = 0; j < 3; ++j)
				{
					GLushort nVertice = (GLushort)(arrVal["faces"])[i + j].asInt();
					iterIndexBuffer->second.push_back(nVertice);
					float nVerticeX = (arrVal["vertices"])[nVertice * 3 + 0].asFloat();
					float nVerticeY = (arrVal["vertices"])[nVertice * 3 + 1].asFloat();
					float nVerticeZ = (arrVal["vertices"])[nVertice * 3 + 2].asFloat();

					//顶点信息对比得到最小点和最大点
					min_x = txMath::getMin(min_x, nVerticeX);
					max_x = txMath::getMax(max_x, nVerticeX);
					min_y = txMath::getMin(min_y, nVerticeY);
					max_y = txMath::getMax(max_y, nVerticeY);
					min_z = txMath::getMin(min_z, nVerticeZ);
					max_z = txMath::getMax(max_z, nVerticeZ);
				}
			}

			// 先写入索引缓冲区数量
			int indexBufferCount = tempIndexBuffer.size();
			serializer.write(indexBufferCount);
			auto iterIndexBuffer = tempIndexBuffer.begin();
			auto iterIndexBufferEnd = tempIndexBuffer.end();
			FOR_STL(tempIndexBuffer, ; iterIndexBuffer != iterIndexBufferEnd; ++iterIndexBuffer)
			{
				// 材质索引
				serializer.write(iterIndexBuffer->first);
				// 索引数量
				int indexCount = iterIndexBuffer->second.size();
				serializer.write(indexCount);
				// 索引缓冲数据
				serializer.writeBuffer((char*)(&(iterIndexBuffer->second[0])), indexCount * sizeof(GLushort));
			}
			END_FOR_STL(tempIndexBuffer);

			// 写入包围盒数据
			serializer.writeVector3(VECTOR3(min_x, min_y, min_z));
			serializer.writeVector3(VECTOR3(max_x, max_y, max_z));
		}
	}
	else
	{
		ENGINE_ERROR("error : %s", jRead.getFormattedErrorMessages().c_str());
	}

	newFileName = fullFileName.substr(0, fullFileName.find_last_of('.')) + ".mesh";
	serializer.writeToFile(newFileName);
	return ret;
}

bool txLoadUtility::jsonSkeletonMeshFileToBinarySkeletonMeshFile(const std::string& fullFileName, std::string& newFileName)
{
	txSerializer serializer;
	std::string meshDir = fullFileName.substr(0, fullFileName.find_last_of('/'));
	std::string skeletonFileName;
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fullFileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::objectValue)
		{
			// 记录下骨骼的文件名
			skeletonFileName = arrVal["SkeletonFile"].asString();
			// 写入骨骼文件名,写入前需要先将骨骼名改为二进制的文件名
			std::string binarySkeletonFileName = skeletonFileName.substr(0, skeletonFileName.find_last_of('.')) + ".skeleton";
			serializer.writeString(binarySkeletonFileName.c_str());

			txVector<txMaterial> materials;
			if (!arrVal["materials"].isNull())
			{
				// 写入材质
				// 材质数量
				int materialCount = arrVal["materials"].size();
				serializer.write(materialCount);
				auto iTer = arrVal["materials"].begin();
				auto iTerEnd = arrVal["materials"].end();
				while (iTer != iTerEnd)
				{
					int nMaterialIndex = (*iTer)["DbgIndex"].asInt();
					bool wireframe = (*iTer)["wireframe"].asBool();
					std::string shadingMode = (*iTer)["shading"].asString();
					std::string materialName = (*iTer)["DbgName"].asString();
					float mapLightAnisotropy = (*iTer)["mapLightAnisotropy"].asFloat();
					std::string blending = (*iTer)["blending"].asString();
					VECTOR3 colorDiffuse = VECTOR3((*iTer)["colorDiffuse"][0].asFloat(), (*iTer)["colorDiffuse"][1].asFloat(), (*iTer)["colorDiffuse"][2].asFloat());
					bool depthTest = (*iTer)["depthTest"].asBool();
					VECTOR3 colorEmissive = VECTOR3((*iTer)["colorEmissive"][0].asFloat(), (*iTer)["colorEmissive"][1].asFloat(), (*iTer)["colorEmissive"][2].asFloat());
					bool depthWrite = (*iTer)["depthWrite"].asBool();
					float opacity = (*iTer)["opacity"].asFloat();
					VECTOR3 colorAmbient = VECTOR3((*iTer)["colorAmbient"][0].asFloat(), (*iTer)["colorAmbient"][1].asFloat(), (*iTer)["colorAmbient"][2].asFloat());
					std::string diffuseTexture = (*iTer)["mapLight"].asString();
					diffuseTexture = "Entityimages/" + diffuseTexture.substr(0, diffuseTexture.find_last_of('.')) + ".swi";
					std::string wrapU = (*iTer)["mapLightWrap"][0].asString();
					std::string wrapV = (*iTer)["mapLightWrap"][1].asString();
					int repeatU = (*iTer)["mapLightRepeat"][0].asInt();
					int repeatV = (*iTer)["mapLightRepeat"][1].asInt();
					bool transparent = (*iTer)["DbgIndex"].asBool();
					int uvLayer = (*iTer)["DbgIndex"].asInt();

					// 材质属性
					serializer.write(nMaterialIndex);
					serializer.write(wireframe);
					serializer.writeString(shadingMode.c_str());
					serializer.writeString(materialName.c_str());
					serializer.write(mapLightAnisotropy);
					serializer.writeString(blending.c_str());
					serializer.writeVector3(colorDiffuse);
					serializer.write(depthTest);
					serializer.writeVector3(colorEmissive);
					serializer.write(depthWrite);
					serializer.write(opacity);
					serializer.writeVector3(colorAmbient);
					serializer.writeString(diffuseTexture.c_str());
					serializer.writeString(wrapU.c_str());
					serializer.writeString(wrapV.c_str());
					serializer.write(repeatU);
					serializer.write(repeatV);
					serializer.write(transparent);
					serializer.write(uvLayer);
					++iTer;
				}
			}
			else
			{
				serializer.write(0);
			}

			if (!arrVal["meshes"].isNull())
			{
				// 写入mesh的数量
				serializer.write(arrVal["meshes"].size());
				auto meshTer = arrVal["meshes"].begin();
				auto meshTerEnd = arrVal["meshes"].end();
				while (meshTer != meshTerEnd)
				{
					// 写入材质索引
					serializer.write((*meshTer)["Material"].asInt());

					float min_x = 0.0f, min_y = 0.0f, min_z = 0.0f, max_x = 0.0f, max_y = 0.0f, max_z = 0.0f;
					if (!(*meshTer)["bones"].isNull() && (*meshTer)["bones"].isArray())
					{
						// 写入蒙皮骨骼的数量
						serializer.write((*meshTer)["bones"].size());
						auto memTIte = (*meshTer)["bones"].begin();
						auto memTIteEnd = (*meshTer)["bones"].end();
						while (memTIte != memTIteEnd)
						{
							if ((*memTIte).type() == Json::objectValue)
							{
								MATRIX4 offsetMatrix;
								for (int iIndex = 0; iIndex < 4; ++iIndex)
								{
									for (int jIndex = 0; jIndex < 4; ++jIndex)
									{
										offsetMatrix[jIndex][iIndex] = ((*memTIte)["offsetMatrix"])[iIndex * 4 + jIndex].asFloat();
									}
								}
								// 写入蒙皮骨骼名字
								serializer.writeString((*memTIte)["name"].asString().c_str());
								// 写入偏移矩阵
								serializer.writeMatrix4(offsetMatrix);

								int nVertexIndiceSize = (*memTIte)["vertexIndices"].size();
								int nVertexWeightSize = (*memTIte)["vertexWeights"].size();
								if (nVertexIndiceSize != nVertexWeightSize)
								{
									ENGINE_ERROR("error : vertex indice size is not equal vertex weight size! indice count : %d, weight count : %d", nVertexIndiceSize, nVertexWeightSize);
								}
								// 写入顶点索引数量
								serializer.write(nVertexIndiceSize);
								for (int index = 0; index < nVertexIndiceSize; ++index)
								{
									// 写入顶点索引,顶点权重
									serializer.write(((*memTIte)["vertexIndices"])[index].asInt());
									serializer.write(((*memTIte)["vertexWeights"])[index].asFloat());
								}
							}
							else
							{
								ENGINE_ERROR("error : json type is not object!");
							}
							++memTIte;
						}
					}
					else
					{
						serializer.write(0);
					}
					//------------------------------------------------------------------------------

					// 写入顶点位置数量(实际是float的数量)
					int nVertexSize = (*meshTer)["Vertices"].size();
					serializer.write(nVertexSize);
					int vertexIndex = 0;
					while (vertexIndex < nVertexSize)
					{
						float nVerticeX = ((*meshTer)["Vertices"])[vertexIndex].asFloat();
						serializer.write(nVerticeX);
						float nVerticeY = ((*meshTer)["Vertices"])[vertexIndex + 1].asFloat();
						serializer.write(nVerticeY);
						float nVerticeZ = ((*meshTer)["Vertices"])[vertexIndex + 2].asFloat();
						serializer.write(nVerticeZ);
						vertexIndex += 3;

						//顶点信息对比得到最小点和最大点
						min_x = txMath::getMin(min_x, nVerticeX);
						max_x = txMath::getMax(max_x, nVerticeX);
						min_y = txMath::getMin(min_y, nVerticeY);
						max_y = txMath::getMax(max_y, nVerticeY);
						min_z = txMath::getMin(min_z, nVerticeZ);
						max_z = txMath::getMax(max_z, nVerticeZ);
					}

					// 写入法线数量(实际是float的数量)
					serializer.write((*meshTer)["Normals"].size());
					auto normalIte = (*meshTer)["Normals"].begin();
					auto normalIteEnd = (*meshTer)["Normals"].end();
					for (; normalIte != normalIteEnd; ++normalIte)
					{
						serializer.write((*normalIte).asFloat());
					}

					// 写入纹理坐标的数量(实际是float的数量)
					int nUVSize = (*meshTer)["UVs"].size();
					serializer.write(nUVSize);
					for (int i = 0; i < nUVSize; ++i)
					{
						serializer.write(((*meshTer)["UVs"])[i].asFloat());
					}

					// 写入顶点索引数量
					serializer.write((*meshTer)["indices"].size());
					auto faceIte = (*meshTer)["indices"].begin();
					auto faceIteEnd = (*meshTer)["indices"].end();
					for (; faceIte != faceIteEnd; ++faceIte)
					{
						serializer.write((GLushort)(*faceIte).asInt());
					}

					// 写入包围盒数据
					//设置AABB包围盒信息
					serializer.writeVector3(VECTOR3(min_x, min_y, min_z));
					serializer.writeVector3(VECTOR3(max_x, max_y, max_z));
					++meshTer;
				}
			}
			else
			{
				serializer.write(0);
			}
		}
	}
	else
	{
		ENGINE_ERROR("error : file name : %s, json error : %s, JsonSkeletonMeshFileToBinarySkeletonMeshFile", fullFileName.c_str(), jRead.getFormattedErrorMessages().c_str());
	}

	// 将转换后的数据写入文件
	newFileName = fullFileName.substr(0, fullFileName.find_last_of('.')) + ".skinmesh";
	serializer.writeToFile(newFileName);

	// 转换骨骼文件
	std::string newSkeletonFileName;
	std::string skeletonFullFileName = meshDir + "/" + skeletonFileName;
	jsonSkeletonFileToBinarySkeletonFile(skeletonFullFileName, newSkeletonFileName);

	// 遍历模型所在目录,查找该模型的所有动作文件
	std::string meshFileName = fullFileName.substr(fullFileName.find_last_of('/') + 1, fullFileName.length() - fullFileName.find_last_of('/') - 1);
	std::string meshFileNameNoSuffix = meshFileName.substr(0, meshFileName.find_last_of('.'));
	txVector<std::string> fileList;
	txFileUtility::findFiles(meshDir, fileList, "_animation.json");
	// 该实体的动画文件列表,first是动画文件名,second是动画名
	txMap<std::string, std::string> animList;
	int animationFileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < animationFileCount; ++i)
	{
		std::string animationFileName = fileList[i].substr(fileList[i].find_last_of('/') + 1, fileList[i].length() - fileList[i].find_last_of('/') - 1);
		if (animationFileName.length() > meshFileNameNoSuffix.length() && animationFileName.substr(0, meshFileNameNoSuffix.length()) == meshFileNameNoSuffix)
		{
			// 去掉模型名前缀
			std::string animationName = animationFileName.substr(meshFileNameNoSuffix.length() + strlen("_"), animationFileName.length() - meshFileNameNoSuffix.length() - strlen("_"));
			// 再去掉_animation.json后缀
			int lastPos = animationName.find_last_of('_');
			if (lastPos != -1)
			{
				animationName = animationName.substr(0, lastPos);
				animList.insert(fileList[i], animationName);
			}
		}
	}
	END_FOR_STL(fileList);

	// 转换动作文件
	auto iterAnim = animList.begin();
	auto iterAnimEnd = animList.end();
	FOR_STL(animList, ; iterAnim != iterAnimEnd; ++iterAnim)
	{
		std::string newAnimationFileName;
		jsonAnimationFileToBinaryAnimationFile(iterAnim->first, newAnimationFileName);
	}
	END_FOR_STL(animList);
	return ret;
}

bool txLoadUtility::jsonSkeletonFileToBinarySkeletonFile(const std::string& fullFileName, std::string& newFileName)
{
	txSerializer serializer;
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fullFileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::arrayValue)
		{
			// 写入骨骼数量
			serializer.write(arrVal.size());
			auto valueIte = arrVal.begin();
			auto valueIteEnd = arrVal.end();
			for (; valueIte != valueIteEnd; ++valueIte)
			{
				// 写入骨骼名
				serializer.writeString((*valueIte)["name"].asString().c_str());
				// 写入父骨骼名
				serializer.writeString((*valueIte)["parentName"].asString().c_str());
				// 写入骨骼位置
				if (!(*valueIte)["position"].isNull())
				{
					serializer.writeVector3(VECTOR3(((*valueIte)["position"])[0].asFloat(), ((*valueIte)["position"])[1].asFloat(), ((*valueIte)["position"])[2].asFloat()));
				}
				else
				{
					ENGINE_ERROR("error : file name : %s, no bone position! JsonSkeletonFileToBinarySkeletonFile", fullFileName.c_str());
				}

				// 写入骨骼缩放
				if (!(*valueIte)["scale"].isNull())
				{
					serializer.writeVector3(VECTOR3(((*valueIte)["scale"])[0].asFloat(), ((*valueIte)["scale"])[1].asFloat(), ((*valueIte)["scale"])[2].asFloat()));
				}
				else
				{
					ENGINE_ERROR("error : file name : %s,no bone scale! JsonSkeletonFileToBinarySkeletonFile", fullFileName.c_str());
				}

				// 写入骨骼旋转
				if (!(*valueIte)["rotation"].isNull())
				{
					MATRIX3 rotateMatrix = txMath::quatToMatrix3(QUATERNION(((*valueIte)["rotation"])[0].asFloat(), ((*valueIte)["rotation"])[1].asFloat(), ((*valueIte)["rotation"])[2].asFloat(), ((*valueIte)["rotation"])[3].asFloat()));
					serializer.writeMatrix3(rotateMatrix);
				}
				else
				{
					ENGINE_ERROR("error : file name : %s, no bone rotation! JsonSkeletonFileToBinarySkeletonFile", fullFileName.c_str());
				}
			}
		}
	}
	else
	{
		ENGINE_ERROR("error : JsonSkeletonFileToBinarySkeletonFile : info : %s", jRead.getFormattedErrorMessages().c_str());
	}
	// 将转换后的数据写入文件
	newFileName = fullFileName.substr(0, fullFileName.find_last_of('.')) + ".skeleton";
	serializer.writeToFile(newFileName);
	return ret;
}

bool txLoadUtility::jsonAnimationFileToBinaryAnimationFile(const std::string& fullFileName, std::string& newFileName)
{
	txSerializer serializer;
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fullFileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::arrayValue)
		{
			// 一般一个动画文件只存储一个动作，取数组第一个元素
			// 写入动作名
			serializer.writeString((arrVal[0])["name"].asString().c_str());

			// 写入骨骼的数量
			serializer.write((arrVal[0])["hierarchy"].size());
			auto valueIte = (arrVal[0])["hierarchy"].begin();
			auto valueIteEnd = (arrVal[0])["hierarchy"].end();
			while (valueIte != valueIteEnd)
			{
				// 写入骨骼名字
				serializer.writeString((*valueIte)["bone"].asString().c_str());

				// 写入关键帧位置数量
				serializer.write(((*valueIte)["keys"])["position"].size());

				// 写入位置的关键帧时间
				auto positionIte = ((*valueIte)["keys"])["position"].begin();
				auto positionIteEnd = ((*valueIte)["keys"])["position"].end();
				for (; positionIte != positionIteEnd; ++positionIte)
				{
					serializer.write((float)(*positionIte)["time"].asDouble());
				}

				// 写入位置的关键帧坐标
				positionIte = ((*valueIte)["keys"])["position"].begin();
				for (; positionIte != positionIteEnd; ++positionIte)
				{
					VECTOR3 framePos = VECTOR3(((*positionIte)["pos"])[0].asFloat(), ((*positionIte)["pos"])[1].asFloat(), ((*positionIte)["pos"])[2].asFloat());
					serializer.writeVector3(framePos);
				}

				// 写入关键帧缩放数量
				serializer.write(((*valueIte)["keys"])["scale"].size());

				// 写入缩放关键帧的时间
				auto scaleIte = ((*valueIte)["keys"])["scale"].begin();
				auto scaleIteEnd = ((*valueIte)["keys"])["scale"].end();
				for (; scaleIte != scaleIteEnd; ++scaleIte)
				{
					serializer.write((float)(*scaleIte)["time"].asDouble());
				}

				// 写入缩放关键帧的缩放值
				scaleIte = ((*valueIte)["keys"])["scale"].begin();
				for (; scaleIte != scaleIteEnd; ++scaleIte)
				{
					VECTOR3 frameScl = VECTOR3(((*scaleIte)["scl"])[0].asFloat(), ((*scaleIte)["scl"])[1].asFloat(), ((*scaleIte)["scl"])[2].asFloat());
					serializer.writeVector3(frameScl);
				}

				// 写入关键帧旋转数量
				serializer.write(((*valueIte)["keys"])["rotation"].size());
				// 写入旋转关键帧的时间
				auto rotationIte = ((*valueIte)["keys"])["rotation"].begin();
				auto rotationIteEnd = ((*valueIte)["keys"])["rotation"].end();
				for (; rotationIte != rotationIteEnd; ++rotationIte)
				{
					serializer.write((float)(*rotationIte)["time"].asDouble());
				}

				// 写入旋转关键帧的旋转值
				rotationIte = ((*valueIte)["keys"])["rotation"].begin();
				for (; rotationIte != rotationIteEnd; ++rotationIte)
				{
					QUATERNION rotateQuat(((*rotationIte)["rot"])[0].asFloat(), ((*rotationIte)["rot"])[1].asFloat(), ((*rotationIte)["rot"])[2].asFloat(), ((*rotationIte)["rot"])[3].asFloat());
					serializer.writeQuaternion(rotateQuat);
				}
				++valueIte;
			}
		}
	}
	else
	{
		ENGINE_ERROR("error : JsonAnimationFileToBinaryAnimationFile : info %s", jRead.getFormattedErrorMessages().c_str());
	}
	// 将转换后的数据写入文件
	newFileName = fullFileName.substr(0, fullFileName.find_last_of('.')) + ".animation";
	serializer.writeToFile(newFileName);
	return ret;
}