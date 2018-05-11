#include "json/json.h"
#include "txScene.h"
#include "txEntity.h"
#include "txSceneManager.h"
#include "txEntityManager.h"
#include "txTextureManager.h"
#include "txNodeManager.h"
#include "txMesh.h"
#include "txSubMesh.h"
#include "txSubEntity.h"
#include "txMathUtility.h"
#include "txCamera.h"
#include "txEngineRoot.h"
#include "txSerializer.h"
#include "txRouteRender.h"
#include "txEngineLog.h"

int txScene::mRouteIndexSeed = 0;

txScene::txScene(const std::string& name)
:
mSceneName(name),
mSceneHeightMap(NULL),
mVisible(true),
mSceneWidth(0),
mSceneHeight(0),
mHeightMapPrecision(1),
mShowHeightMap(false),
mTerrainTypeMap(NULL),
mTerrainTypeMapPrecision(1),
mShowTerrainTypeMap(false),
mTerrainTypeHeightMap(NULL),
mTerrainTypeHeightMapPrecision(1),
mShowTerrainTypeHeightMap(false)
{
	mSceneOrigin = txMath::VEC3_ZERO;
	mSceneMove = mSceneOrigin;
}

txScene::~txScene()
{
	destroy();
}

bool txScene::rename(const std::string& newName)
{
	if (newName == mSceneName)
	{
		return false;
	}
	std::string oldName = mSceneName;
	mSceneName = newName;
	bool ret = mSceneManager->notifySceneRenamed(oldName, this);
	if (!ret)
	{
		mSceneName = oldName;
		return false;
	}
	return true;
}

bool txScene::loadScene(const bool& binary)
{
	if (!binary)
	{
		return loadJsonScene(mSceneName);
	}
	else
	{
		return loadBinaryScene(mSceneName);
	}
}

bool txScene::loadJsonScene(const std::string& sceneName)
{
	if (mSceneObjects.size() > 0 || mNoneAsyncSceneTexturs.size() > 0)
	{
		ENGINE_ERROR("error : scene is not empty! can not load! loaded scene name : %s, scene to load name : %s", mSceneName.c_str(), sceneName.c_str());
		return false;
	}
	std::string sceneDir = sceneName + "/";
	std::string sceneRootFile = sceneDir + sceneName + ".json";
	// 如果文件不存在,则直接返回
	if (!txFileUtility::isFileExist(sceneRootFile))
	{
		return false;
	}

	// 开始加载场景文件,发送开始加载的事件,需要立即执行
	mEngineRoot->sendEvent(EE_START_LOAD_SCENE, sceneName);

	// 当前加载进度为0
	float curProgress = 0.0f;
	mEngineRoot->sendEvent(EE_SCENE_LOAD_PROGRESS, txStringUtility::floatToString(curProgress, 3));

	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(sceneRootFile), arrVal);
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

			// geometries读取完后进度为0.1
			curProgress += 0.1f;
			mEngineRoot->sendEvent(EE_SCENE_LOAD_PROGRESS, txStringUtility::floatToString(curProgress, 3));

			// 记录下加载实体之前的加载进度
			float progressBefore = curProgress;
			if ((arrVal["object"])["children"].type() == Json::arrayValue)
			{
				int objectCount = (arrVal["object"])["children"].size();
				int loadCount = 0;
				auto valueIte = (arrVal["object"])["children"].begin();
				auto valueIteEnd = (arrVal["object"])["children"].end();
				std::string staticMeshsDir = sceneDir + "staticMesh/";
				while (valueIte != valueIteEnd)
				{
					std::string objectType = (*valueIte)["type"].asString();
					std::string objectName = (*valueIte)["name"].asString();

					//Create a Node
					txNode* pNode = mNodeManager->CreateNode();

					//set the node position,scale,rotation
					VECTOR3 pos = VECTOR3(((*valueIte)["position"])[0].asFloat(), ((*valueIte)["position"])[1].asFloat(), ((*valueIte)["position"])[2].asFloat());
					pNode->SetPosition(pos);

					MATRIX3 rotateMatrix = txMath::quatToMatrix3(
						QUATERNION(VECTOR3(((*valueIte)["rotation"])[0].asFloat(), ((*valueIte)["rotation"])[1].asFloat(), ((*valueIte)["rotation"])[2].asFloat())));
					pNode->SetRotationMatrix(rotateMatrix);

					VECTOR3 scale = VECTOR3(((*valueIte)["scale"])[0].asFloat(), ((*valueIte)["scale"])[1].asFloat(), ((*valueIte)["scale"])[2].asFloat());
					pNode->SetScale(scale);

					if (objectType == "Mesh")
					{
						std::string geometryUuid = ((*valueIte))["geometry"].asString();
						std::string MeshName = ((*valueIte))["name"].asString();
						mSceneManager->getRootNode()->AddChildNode(pNode);
						auto iterUUID = geometryMap.find(geometryUuid);
						std::string meshFile = iterUUID->second;
						meshFile = staticMeshsDir + meshFile;
						// 静态模型使用浅拷贝
						txVector<txTexture*> textureList;
						txEntity* pEntity = mEntityManager->CreateEntity(MeshName, meshFile, false, ECT_SIMPLE, false, &textureList);
						mSceneObjects.insert(pEntity->getName(), pEntity);
						pEntity->SetParent(pNode);
						int textureCount = textureList.size();
						FOR_STL(textureList, int i = 0; i < textureCount; ++i)
						{
							mNoneAsyncSceneTexturs.push_back(textureList[i]);
						}
						END_FOR_STL(textureList);
						textureList.clear();

						mEngineRoot->sendEvent(EE_LOAD_SCENE_OBJECT, MeshName);
					}
					++valueIte;
					++loadCount;

					// 由于加载实体之前已经过了0.1的进度,所以整个实体加载占0.9的进度
					curProgress = progressBefore + (1.0f - progressBefore) * (float)loadCount / (float)objectCount;
					mEngineRoot->sendEvent(EE_SCENE_LOAD_PROGRESS, txStringUtility::floatToString(curProgress, 3));
				}
			}
		}
	}
	else
	{
		ENGINE_ERROR("error : [txSceneManager::LoadSceneFromFile] load[%s] err. info : %s", sceneRootFile.c_str(), jRead.getFormattedErrorMessages().c_str());
	}
	// 加载场景的高度图
	loadHeightMapFile(sceneDir + sceneName + ".heightmap");

	// 场景文件加载结束,发送加载结束的事件,需要立即执行
	mEngineRoot->sendEvent(EE_END_LOAD_SCENE);
	mSceneName = sceneName;
	return ret;
}

bool txScene::loadBinaryScene(const std::string& sceneName)
{
	if (mSceneObjects.size() > 0 || mNoneAsyncSceneTexturs.size() > 0)
	{
		ENGINE_ERROR("error : scene is not empty! can not load! loaded scene name : %s, scene to load name : %s", mSceneName.c_str(), sceneName.c_str());
		return false;
	}
	std::string sceneDir = sceneName + "/";
	std::string sceneRootFile = sceneDir + sceneName + ".scene";
	if (!txFileUtility::isFileExist(sceneRootFile))
	{
		return false;
	}
	// 开始加载场景文件,发送开始加载的事件,需要立即执行
	mEngineRoot->sendEvent(EE_START_LOAD_SCENE, sceneName);

	// 当前加载进度为0
	float curProgress = 0.0f;
	mEngineRoot->sendEvent(EE_SCENE_LOAD_PROGRESS, txStringUtility::floatToString(curProgress, 3));
	
	int fileSize = 0;
	char* buffer = txFileUtility::openBinaryFile(sceneRootFile, &fileSize);
	if (buffer == NULL)
	{
		ENGINE_ERROR("error : can not load scene file! file name : %s", sceneRootFile.c_str());
		return false;
	}
	txSerializer serializer(buffer, fileSize);
	std::string staticMeshsDir = sceneDir + "staticMesh/";
	// 记录加载物体之前的加载进度
	float progressBefore = curProgress;
	// 读取物体数量
	int objectCount;
	serializer.read(objectCount);
	for (int i = 0; i < objectCount; ++i)
	{
		char objectType[128];
		serializer.readString(objectType, 128);
		if (std::string(objectType) == "Mesh")
		{
			char meshName[128];
			serializer.readString(meshName, 128);
			VECTOR3 pos;
			serializer.readVector3(pos);
			MATRIX3 rot;
			serializer.readMatrix3(rot);
			VECTOR3 scale;
			serializer.readVector3(scale);
			char meshFile[128];
			serializer.readString(meshFile, 128);

			txNode* pNode = mNodeManager->CreateNode();
			pNode->SetPosition(pos);
			pNode->SetRotationMatrix(rot);
			pNode->SetScale(scale);
			mSceneManager->getRootNode()->AddChildNode(pNode);
			std::string meshFileName = staticMeshsDir + meshFile;
			// 静态模型使用浅拷贝
			txVector<txTexture*> textureList;
			txEntity* pEntity = mEntityManager->CreateEntity(std::string(meshName), meshFileName, false, ECT_SIMPLE, false, &textureList);
			mSceneObjects.insert(pEntity->getName(), pEntity);
			pEntity->SetParent(pNode);
			int textureCount = textureList.size();
			FOR_STL(textureList, int j = 0; j < textureCount; ++j)
			{
				mNoneAsyncSceneTexturs.push_back(textureList[j]);
			}
			END_FOR_STL(textureList);
			textureList.clear();

			// 发送创建物体的事件
			mEngineRoot->sendEvent(EE_LOAD_SCENE_OBJECT, meshName);
		}

		// 发送加载进度改变的事件
		curProgress = progressBefore + (1.0f - progressBefore) * (float)i / (float)objectCount;
		mEngineRoot->sendEvent(EE_SCENE_LOAD_PROGRESS, txStringUtility::floatToString(curProgress, 3));
	}
	TRACE_DELETE_ARRAY(buffer);
	// 加载场景的高度图
	loadHeightMapFile(sceneDir + sceneName + ".heightmap");
	// 场景文件加载结束,发送加载结束的事件,需要立即执行
	mEngineRoot->sendEvent(EE_END_LOAD_SCENE);
	mSceneName = sceneName;
	return true;
}

void txScene::generaTextures()
{
	int textureCount = mNoneAsyncSceneTexturs.size();
	FOR_STL(mNoneAsyncSceneTexturs, int i = 0; i < textureCount; ++i)
	{
		txTexture* pTexture = mNoneAsyncSceneTexturs[i];
		if (pTexture != NULL)
		{
			pTexture->initTexture();
		}
	}
	END_FOR_STL(mNoneAsyncSceneTexturs);
	mNoneAsyncSceneTexturs.clear();
}

bool txScene::copySceneFrom(txScene* srcScene)
{
	if (srcScene == NULL)
	{
		return false;
	}
	auto& srcEntityList = srcScene->getObjectList();
	auto iter = srcEntityList.begin();
	auto iterEnd = srcEntityList.end();
	FOR_STL(srcEntityList, ; iter != iterEnd; ++iter)
	{
		if (iter->second->getType() != OT_ENTITY)
		{
			ENGINE_ERROR("error : scene object must be an entity!");
			break;
		}
		std::string newEntityName = std::string(iter->second->getName()) + "_copy";
		// 静态模型使用简单拷贝
		txEntity* newEntity = mEntityManager->CreateWithExistEntity(newEntityName, (txEntity*)(iter->second), ECT_SIMPLE);
		mSceneObjects.insert(newEntity->getName(), newEntity);

		// 实体拷贝完后将节点属性也一起拷贝
		txNode* oldEntityNode = iter->second->GetParent();
		txNode* node = mNodeManager->CreateNode();
		node->copyNode(oldEntityNode);
		newEntity->SetParent(node);
		mSceneManager->getRootNode()->AddChildNode(node);
	}
	END_FOR_STL(srcEntityList);

	mSceneMove = srcScene->mSceneMove;
	mSceneOrigin = srcScene->mSceneOrigin;
	mSceneWidth = srcScene->mSceneWidth;
	mSceneHeight = srcScene->mSceneHeight;
	mHeightMapPrecision = srcScene->mHeightMapPrecision;
	TRACE_DELETE_ARRAY(mSceneHeightMap);
	int heightMapWidth = mSceneWidth * mHeightMapPrecision;
	int heightMapHeight = mSceneHeight * mHeightMapPrecision;
	int heightMapSize = heightMapWidth * heightMapHeight;
	mSceneHeightMap = TRACE_NEW_ARRAY(float, heightMapSize, mSceneHeightMap);
	memcpy(mSceneHeightMap, srcScene->mSceneHeightMap, sizeof(float)* heightMapSize);
	
	int bufferCount = srcScene->getHeightMapBufferCount();
	for (int i = 0; i < bufferCount; ++i)
	{
		int vertexCount = srcScene->getHeightMapVertexCount(i);
		GLfloat* vertexBuffer = TRACE_NEW_ARRAY(GLfloat, vertexCount * 3, vertexBuffer);
		memcpy(vertexBuffer, srcScene->getHeightMapVertices(i), sizeof(GLfloat)* 3 * vertexCount);

		int indiceCount = srcScene->getHeightMapIndicesCount(i);
		GLushort* indiceBuffer = TRACE_NEW_ARRAY(GLushort, indiceCount, indiceBuffer);
		memcpy(indiceBuffer, srcScene->getHeightMapIndices(i), sizeof(GLushort)* indiceCount);

		mHeightMapMeshList.push_back(std::pair<GLushort*, GLfloat*>(indiceBuffer, vertexBuffer));
	}
	return true;
}

void txScene::moveScene(const VECTOR3& delta)
{
	auto iterSceneEntity = mSceneObjects.begin();
	auto iterSceneEntityEnd = mSceneObjects.end();
	FOR_STL(mSceneObjects, ; iterSceneEntity != iterSceneEntityEnd; ++iterSceneEntity)
	{
		txNode* node = iterSceneEntity->second->GetParent();
		node->MoveLocal(delta);
	}
	END_FOR_STL(mSceneObjects);
	mSceneMove += delta;
}

void txScene::setVisible(bool visible)
{
	mVisible = visible;
	auto iterSceneEntity = mSceneObjects.begin();
	auto iterSceneEntityEnd = mSceneObjects.end();
	FOR_STL(mSceneObjects, ; iterSceneEntity != iterSceneEntityEnd; ++iterSceneEntity)
	{
		if (iterSceneEntity->second->getType() == OT_ENTITY)
		{
			static_cast<txEntity*>(iterSceneEntity->second)->setVisible(mVisible);
		}
	}
	END_FOR_STL(mSceneObjects);
}

void txScene::addObject(txObject* object)
{
	if (object == NULL)
	{
		return;
	}
	auto iterObject = mSceneObjects.find(object->getName());
	if (iterObject != mSceneObjects.end())
	{
		return;
	}
	mSceneObjects.insert(object->getName(), object);
}

void txScene::destroy()
{
	auto iterSceneEntity = mSceneObjects.begin();
	auto iterSceneEntityEnd = mSceneObjects.end();
	FOR_STL(mSceneObjects, ; iterSceneEntity != iterSceneEntityEnd; ++iterSceneEntity)
	{
		// 这里只销毁了实体,其他类型的物体没有被销毁,会存在内存泄漏
		if (iterSceneEntity->second->getType() == OT_ENTITY)
		{
			txEntity* entity = static_cast<txEntity*>(iterSceneEntity->second);
			if (entity != NULL)
			{
				// 先销毁实体的节点
				if (entity->GetParent() != NULL)
				{
					mNodeManager->DestroyNode(entity->GetParent()->GetName());
				}
				mEntityManager->DestroyEntity(entity->getName());
			}
		}
	}
	END_FOR_STL(mSceneObjects);
	mSceneObjects.clear();

	TRACE_DELETE_ARRAY(mSceneHeightMap);

	mSceneOrigin = txMath::VEC3_ZERO;
	mSceneMove = mSceneOrigin;
}

bool txScene::rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint)
{
	bool ret = false;
	auto iter = mSceneObjects.begin();
	auto iterEnd = mSceneObjects.end();
	FOR_STL(mSceneObjects, ; iter != iterEnd; ++iter)
	{
		if (iter->second->getType() == OT_ENTITY)
		{
			VECTOR3 point;
			if (static_cast<txEntity*>(iter->second)->rayIntersect(ori, dir, point))
			{
				// 只保存距离最近的相交点
				if (!ret || txMath::getLength(point - ori) < txMath::getLength(intersectPoint - ori))
				{
					intersectPoint = point;
				}
				ret = true;
			}
		}
	}
	END_FOR_STL(mSceneObjects);
	return ret;
}

bool txScene::getHeight(const float& x, const float& z, float& y)
{
	y = 0.0f;
	if (!mVisible)
	{
		return false;
	}
	if (mSceneHeightMap == NULL)
	{
		return false;
	}

	// 传入的x,z是场景中的世界坐标,需要先将包围盒转换到世界坐标系才能判断是否在包围盒内
	MATRIX4 transMat = txMath::translate(txMath::MAT4_IDENTITY, mSceneMove - mSceneOrigin);
	txAABox worldAABB = mSceneAABB.transform(transMat);
	// 如果超出了场景的范围,则返回false
	VECTOR3 checkPoint(x, mSceneAABB.getCenter().y, z);
	if (!worldAABB.containPoint(checkPoint))
	{
		return false;
	}
	float cx = x - mSceneMove.x;
	float cz = z - mSceneMove.z;

	// 计算当前点在哪个格子内
	int gridX = (int)(cx * mHeightMapPrecision);
	int gridY = (int)(cz * mHeightMapPrecision);

	int heightMapSize = getHeightMapSize();
	int heightMapWidth = getHeightMapWidth();

	// 第一个顶点
	int heightMapPosX0 = gridX;
	int heightMapPosY0 = gridY;
	VECTOR2 point0((float)heightMapPosX0 / mHeightMapPrecision, (float)heightMapPosY0 / mHeightMapPrecision);
	int gridIndex0 = heightMapPosX0 + heightMapPosY0 * heightMapWidth;
	if (!txMath::isInRange(gridIndex0, 0, heightMapSize - 1))
	{
		return false;
	}
	VECTOR3 heightPoint0(point0.x, mSceneHeightMap[gridIndex0], point0.y);

	// 第二个顶点
	int heightMapPosX1 = gridX + 1;
	int heightMapPosY1 = gridY;
	VECTOR2 point1((float)heightMapPosX1 / mHeightMapPrecision, (float)heightMapPosY1 / mHeightMapPrecision);
	int gridIndex1 = heightMapPosX1 + heightMapPosY1 * heightMapWidth;
	if (!txMath::isInRange(gridIndex1, 0, heightMapSize - 1))
	{
		return false;
	}
	VECTOR3 heightPoint1(point1.x, mSceneHeightMap[gridIndex1], point1.y);

	// 第三个顶点
	int heightMapPosX2 = gridX + 1;
	int heightMapPosY2 = gridY + 1;
	VECTOR2 point2((float)heightMapPosX2 / mHeightMapPrecision, (float)heightMapPosY2 / mHeightMapPrecision);
	int gridIndex2 = heightMapPosX2 + heightMapPosY2 * heightMapWidth;
	if (!txMath::isInRange(gridIndex1, 0, heightMapSize - 1))
	{
		return false;
	}
	VECTOR3 heightPoint2(point2.x, mSceneHeightMap[gridIndex2], point2.y);

	// 第四个顶点
	int heightMapPosX3 = gridX;
	int heightMapPosY3 = gridY + 1;
	VECTOR2 point3((float)heightMapPosX3 / mHeightMapPrecision, (float)heightMapPosY3 / mHeightMapPrecision);
	int gridIndex3 = heightMapPosX3 + heightMapPosY3 * heightMapWidth;
	if (!txMath::isInRange(gridIndex3, 0, heightMapSize - 1))
	{
		return false;
	}
	VECTOR3 heightPoint3(point3.x, mSceneHeightMap[gridIndex3], point3.y);

	VECTOR3 ori(cx, 10000.0f, cz);
	VECTOR3 interPoint;
	if (txMath::intersectRayTriangle(ori, -txMath::AXIS_Y, heightPoint0, heightPoint2, heightPoint1, interPoint))
	{
		VECTOR3 point = (1 - interPoint.x - interPoint.y) * heightPoint0 + interPoint.x * heightPoint2 + interPoint.y * heightPoint1;
		//VECTOR3 point = ori + dir * interPoint.z; // 这种计算不准确,所以使用uv来计算交点
		y = point.y;
	}
	else if (txMath::intersectRayTriangle(ori, -txMath::AXIS_Y, heightPoint0, heightPoint3, heightPoint2, interPoint))
	{
		VECTOR3 point = (1 - interPoint.x - interPoint.y) * heightPoint0 + interPoint.x * heightPoint3 + interPoint.y * heightPoint2;
		//VECTOR3 point = ori + dir * interPoint.z;
		y = point.y;
	}
	return true;
}

TERRAIN_TYPE txScene::getTerrainType(const float& x, const float& z)
{
	return TT_GROUND;
}

bool getTerrainTypeHeight(const float& x, const float& z, float& y)
{
	return false;
}

void txScene::generateSceneRect()
{
	txAABox aabb;
	auto iter = mSceneObjects.begin();
	auto iterEnd = mSceneObjects.end();
	FOR_STL(mSceneObjects, ; iter != iterEnd; ++iter)
	{
		if (iter->second->getType() == OT_ENTITY)
		{
			auto& subEntityList = static_cast<txEntity*>(iter->second)->getSubEntityList();
			int subEntityCount = subEntityList.size();
			FOR_STL(subEntityList, int i = 0; i < subEntityCount; ++i)
			{
				txAABox transAABB = subEntityList[i]->GetOriMesh()->GetAABox()->transform(iter->second->GetParent()->GetFinalMatrix());
				aabb.merge(transAABB);
			}
			END_FOR_STL(subEntityList);
		}
	}
	END_FOR_STL(mSceneObjects);
	setSceneAABB(aabb);
}

void txScene::generateRenderHeightMap()
{
	if (mSceneHeightMap == NULL)
	{
		return;
	}

	// 创建顶点和索引数据,这是把一个个四边形分开的
	int mapWidth = mSceneWidth * mHeightMapPrecision;
	int mapHeight = mSceneHeight * mHeightMapPrecision;
	int gridCount = mapWidth * mapHeight;
	int bufferCount = gridCount / BLOCK_GRID_COUNT;
	if (gridCount % BLOCK_GRID_COUNT != 0)
	{
		++bufferCount;
	}
	for (int i = 0; i < bufferCount; ++i)
	{
		GLushort* indices = NULL;
		GLfloat* vertices = NULL;
		if (i != bufferCount - 1)
		{
			indices = TRACE_NEW_ARRAY(GLushort, BLOCK_GRID_COUNT * 8, indices);
			vertices = TRACE_NEW_ARRAY(GLfloat, BLOCK_GRID_COUNT * 4 * 3, vertices);
		}
		else
		{
			indices = TRACE_NEW_ARRAY(GLushort, (gridCount % BLOCK_GRID_COUNT) * 8, indices);
			vertices = TRACE_NEW_ARRAY(GLfloat, (gridCount % BLOCK_GRID_COUNT) * 4 * 3, vertices);
		}
		mHeightMapMeshList.push_back(std::pair<GLushort*, GLfloat*>(indices, vertices));
	}
	for (int i = 0; i < gridCount; ++i)
	{
		short indicesBufferIndex = i / BLOCK_GRID_COUNT;
		short rectIndex = i % BLOCK_GRID_COUNT;
		GLushort* indiceBuffer = mHeightMapMeshList[indicesBufferIndex].first;
		GLfloat* verticeBuffer = mHeightMapMeshList[indicesBufferIndex].second;

		int gridX = i % (mapWidth + 1);
		int gridY = i / (mapWidth + 1);

		// 第一个顶点
		verticeBuffer[4 * 3 * rectIndex + 0] = (float)gridX / (float)mHeightMapPrecision;
		verticeBuffer[4 * 3 * rectIndex + 1] = mSceneHeightMap[gridX + gridY * (mapWidth + 1)];
		verticeBuffer[4 * 3 * rectIndex + 2] = (float)gridY / (float)mHeightMapPrecision;

		// 第二个顶点
		verticeBuffer[4 * 3 * rectIndex + 3] = (float)(gridX + 1) / (float)mHeightMapPrecision;
		verticeBuffer[4 * 3 * rectIndex + 4] = mSceneHeightMap[gridX + 1 + gridY * (mapWidth + 1)];
		verticeBuffer[4 * 3 * rectIndex + 5] = (float)gridY / (float)mHeightMapPrecision;

		// 第三个顶点
		verticeBuffer[4 * 3 * rectIndex + 6] = (float)(gridX + 1) / (float)mHeightMapPrecision;
		verticeBuffer[4 * 3 * rectIndex + 7] = mSceneHeightMap[gridX + 1 + (gridY + 1) * (mapWidth + 1)];
		verticeBuffer[4 * 3 * rectIndex + 8] = (float)(gridY + 1) / (float)mHeightMapPrecision;

		// 第四个顶点
		verticeBuffer[4 * 3 * rectIndex + 9] = (float)gridX / (float)mHeightMapPrecision;
		verticeBuffer[4 * 3 * rectIndex + 10] = mSceneHeightMap[gridX + (gridY + 1) * (mapWidth + 1)];
		verticeBuffer[4 * 3 * rectIndex + 11] = (float)(gridY + 1) / (float)mHeightMapPrecision;

		short index0 = 4 * rectIndex;
		short index1 = 4 * rectIndex + 1;
		short index2 = 4 * rectIndex + 2;
		short index3 = 4 * rectIndex + 3;

		indiceBuffer[8 * rectIndex + 0] = index0;
		indiceBuffer[8 * rectIndex + 1] = index1;
		indiceBuffer[8 * rectIndex + 2] = index1;
		indiceBuffer[8 * rectIndex + 3] = index2;
		indiceBuffer[8 * rectIndex + 4] = index2;
		indiceBuffer[8 * rectIndex + 5] = index3;
		indiceBuffer[8 * rectIndex + 6] = index3;
		indiceBuffer[8 * rectIndex + 7] = index0;
	}
}

void txScene::saveHeightMapFile(const std::string& fileName)
{
	txSerializer serialzer;
	serialzer.write(mSceneWidth);
	serialzer.write(mSceneHeight);
	serialzer.write(mHeightMapPrecision);
	serialzer.writeVector3(mSceneOrigin);
	serialzer.writeBuffer((char*)mSceneHeightMap, getHeightMapSize() * sizeof(float));
	serialzer.writeToFile(fileName);
}

void txScene::loadHeightMapFile(const std::string& fileName)
{
	int fileSize = 0;
	char* buffer = txFileUtility::openBinaryFile(fileName, &fileSize);
	if (buffer == NULL)
	{
		return;
	}
	txSerializer serialzer(buffer, fileSize);
	serialzer.read(mSceneWidth);
	serialzer.read(mSceneHeight);
	serialzer.read(mHeightMapPrecision);
	serialzer.readVector3(mSceneOrigin);
	// 读取场景高度图数据
	int heightMapSize = getHeightMapSize();
	TRACE_DELETE_ARRAY(mSceneHeightMap);
	mSceneHeightMap = TRACE_NEW_ARRAY(float, heightMapSize, mSceneHeightMap);
	serialzer.readBuffer((char*)mSceneHeightMap, sizeof(float)* heightMapSize, sizeof(float)* heightMapSize);
	TRACE_DELETE_ARRAY(buffer);
	mSceneMove = mSceneOrigin;

	// 加载高度图以后清空高度图渲染数据
	int bufferCount = mHeightMapMeshList.size();
	FOR_STL(mHeightMapMeshList, int i = 0; i < bufferCount; ++i)
	{
		TRACE_DELETE_ARRAY(mHeightMapMeshList[i].first);
		TRACE_DELETE_ARRAY(mHeightMapMeshList[i].second);
	}
	END_FOR_STL(mHeightMapMeshList);
	mHeightMapMeshList.clear();
}

void txScene::setSceneAABB(const txAABox& aabb)
{
	if (aabb.isEmpty())
	{
		mSceneWidth = 0;
		mSceneHeight = 0;
		mSceneOrigin = txMath::VEC3_ZERO;
		mSceneMove = txMath::VEC3_ZERO;
		return;
	}
	float sceneWidth = (aabb.getMax() - aabb.getMin()).x;
	float sceneHeight = (aabb.getMax() - aabb.getMin()).z;
	int intSceneWidth = txMath::getForwardInt(sceneWidth);
	int intSceneHeight = txMath::getForwardInt(sceneHeight);
	if (mSceneWidth != 0 && intSceneWidth != mSceneWidth)
	{
		LOGI("error : height map width did not match scene width! height map width : %d, scene width : %d", intSceneWidth, mSceneWidth);
	}
	if (mSceneHeight != 0 && intSceneHeight != mSceneHeight)
	{
		LOGI("error : height map height did not match scene height! height map height : %d, scene height : %d", intSceneHeight, mSceneHeight);
	}
	mSceneWidth = intSceneWidth;
	mSceneHeight = intSceneHeight;
	mSceneOrigin = aabb.getMin();
	mSceneMove = mSceneOrigin;
	mSceneAABB = aabb;
}

GLfloat* txScene::getHeightMapVertices(const int& index)
{
	if (!txMath::isInRange(index, 0, (int)mHeightMapMeshList.size() - 1))
	{
		return NULL;
	}
	return mHeightMapMeshList[index].second;
}

GLushort* txScene::getHeightMapIndices(const int& index)
{
	if (!txMath::isInRange(index, 0, (int)mHeightMapMeshList.size() - 1))
	{
		return NULL;
	}
	return mHeightMapMeshList[index].first;
}

int txScene::getHeightMapVertexCount(const int& index)
{
	int mapWidth = mSceneWidth * mHeightMapPrecision;
	int mapHeight = mSceneHeight * mHeightMapPrecision;
	int gridCount = mapWidth * mapHeight;
	if (index != (int)mHeightMapMeshList.size() - 1)
	{
		return BLOCK_GRID_COUNT * 4;
	}
	else
	{
		return (gridCount % BLOCK_GRID_COUNT) * 4;
	}
}

int txScene::getHeightMapIndicesCount(const int& index)
{
	int mapWidth = mSceneWidth * mHeightMapPrecision;
	int mapHeight = mSceneHeight * mHeightMapPrecision;
	int gridCount = mapWidth * mapHeight;
	if (index != (int)mHeightMapMeshList.size() - 1)
	{
		return BLOCK_GRID_COUNT * 8;
	}
	else
	{
		return (gridCount % BLOCK_GRID_COUNT) * 8;
	}
}

void txScene::clearHeightMap()
{
	mSceneWidth = 0;
	mSceneHeight = 0;
	TRACE_DELETE_ARRAY(mSceneHeightMap);
	mHeightMapPrecision = 1;
}

void txScene::setHeightMapData(float* heightMap, const int& heightMapSize)
{
	if (heightMap == NULL || heightMapSize != getHeightMapSize())
	{
		return;
	}
	TRACE_DELETE_ARRAY(mSceneHeightMap);
	mSceneHeightMap = TRACE_NEW_ARRAY(float, heightMapSize, mSceneHeightMap);
	memcpy(mSceneHeightMap, heightMap, sizeof(float) * heightMapSize);
}

void txScene::setTerrainTypeMapData(int* heightMap, const int& heightMapSize)
{
	;
}

GLfloat* txScene::getTerrainTypeMapVertices(const int& index)
{
	return NULL;
}

GLushort* txScene::getTerrainTypeMapIndices(const int& index)
{
	return NULL;
}

int txScene::getTerrainTypeMapVertexCount(const int& index)
{
	return 0;
}

int txScene::getTerrainTypeMapIndicesCount(const int& index)
{
	return 0;
}

void txScene::clearTerrainTypeMap()
{
	;
}

void txScene::setTerrainTypeHeightMapData(int* heightMap, const int& heightMapSize)
{
	;
}

GLfloat* txScene::getTerrainTypeHeightMapVertices(const int& index)
{
	return NULL;
}

GLushort* txScene::getTerrainTypeHeightMapIndices(const int& index)
{
	return NULL;
}

int txScene::getTerrainTypeHeightMapVertexCount(const int& index)
{
	return 0;
}

int txScene::getTerrainTypeHeightMapIndicesCount(const int& index)
{
	return 0;
}

void txScene::clearTerrainTypeHeightMap()
{
	;
}

int txScene::addRouteRender(txVector<VECTOR3>& pointList)
{
	txRouteRender* routeRender = TRACE_NEW(txRouteRender, routeRender, this);
	routeRender->init(pointList);
	int index = mRouteIndexSeed++;
	mRouteRender.insert(index, routeRender);
	return index;
}

void txScene::destroyRouteRender(const int& i)
{
	auto iter = mRouteRender.find(i);
	if (iter != mRouteRender.end())
	{
		TRACE_DELETE(iter->second);
		mRouteRender.erase(iter);
	}
}

void txScene::destroyRouteRender(txRouteRender* routeRender)
{
	auto iter = mRouteRender.begin();
	auto iterEnd = mRouteRender.end();
	FOR_STL(mRouteRender, ; iter != iterEnd; ++iter)
	{
		if (iter->second == routeRender)
		{
			TRACE_DELETE(iter->second);
			mRouteRender.erase(iter, false);
			break;
		}
	}
	END_FOR_STL(mRouteRender);
}