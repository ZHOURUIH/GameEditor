#ifndef _TX_SCENE_H_
#define _TX_SCENE_H_

#include "txEngineDefine.h"
#include "txAABox.h"
#include "txEngineBase.h"

// 用于加载和管理单个场景文件,一个场景文件对应一个txScene
class txSceneManager;
class txObject;
class txTexture;
class txCamera;
class txRouteRender;
class txScene : public txEngineBase
{
public:
	txScene(const std::string& name);
	virtual ~txScene();
	const std::string& getName() { return mSceneName; }
	bool rename(const std::string& newName);
	// 加载场景文件
	bool loadScene(const bool& binary);
	void saveScene();
	// 加载新版本的场景文件
	bool loadNewScene();
	void saveNewScene();
	// 加载所有实体的纹理
	void generaTextures();
	// 从一个已经加载的场景拷贝一份场景
	bool copySceneFrom(txScene* srcScene);
	// 将场景中的所有物体在世界坐标系下整体平移
	void moveScene(const VECTOR3& delta);
	// 设置场景中所有物体是否可见
	void setVisible(bool visible);
	void addObject(txObject* object);
	// 得到场景中的物体列表
	txMap<std::string, txObject*>& getObjectList(){ return mSceneObjects; }
	txObject* getObject(const std::string& name)
	{
		auto iter = mSceneObjects.find(name);
		if (iter != mSceneObjects.end())
		{
			return iter->second;
		}
		return NULL;
	}
	// 销毁本场景的所有静态实体
	void destroy();
	// 场景射线检测,返回的是离射线起点最近的相交点
	bool rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint);
	// 得到场景中指定点的高度, x, z是世界坐标下的
	bool getHeight(const float& x, const float& z, float& y);
	// 获得指定点的地形类型
	TERRAIN_TYPE getTerrainType(const float& x, const float& z);
	// 得到指定点的地形类型的高度,比如水面的高度等等
	bool getTerrainTypeHeight(const float& x, const float& z, float& y);
	// 计算场景的宽高
	void generateSceneRect();
	// 生成高度图渲染数据
	void generateRenderHeightMap();
	// 将高度图保存为文件
	void saveHeightMapFile(const std::string& fileName);
	// 从文件加载高度图
	void loadHeightMapFile(const std::string& fileName);
	void setSceneAABB(const txAABox& aabb);
	float getSceneRealWidth() { return (mSceneAABB.getMax() - mSceneAABB.getMin()).x; }
	float getSceneRealHeight() { return (mSceneAABB.getMax() - mSceneAABB.getMin()).z; }
	const int& getSceneWidth() { return mSceneWidth; }
	const int& getSceneHeight() { return mSceneHeight; }
	const VECTOR3& getSceneOrigin() { return mSceneOrigin; }
	const VECTOR3& getSceneMove() { return mSceneMove; }
	// 恢复场景的平移量
	void resetScenePosition() { moveScene(mSceneOrigin - mSceneMove); }
	void alignedToWorldOrigin(const bool& alignedHeight = false) 
	{
		VECTOR3 moveDelta = -mSceneMove;
		if (!alignedHeight)
		{
			moveDelta.y = 0.0f;
		}
		moveScene(moveDelta);
	}
	// 场景高度图
	const int& getHeightMapPrecision() { return mHeightMapPrecision; }
	int getHeightMapSize() { return getHeightMapWidth() * getHeightMapHeight(); }
	int getHeightMapWidth() { return mSceneWidth * mHeightMapPrecision + 1; }
	int getHeightMapHeight() { return mSceneHeight * mHeightMapPrecision + 1; }
	float* getSceneHeightMap() { return mSceneHeightMap; }
	void setHeightMapData(float* heightMap, const int& heightMapSize);
	void setHeightMapPrecision(const int& precision) { mHeightMapPrecision = precision; }
	GLfloat* getHeightMapVertices(const int& index);
	int getHeightMapBufferCount() { return mHeightMapMeshList.size(); }
	GLushort* getHeightMapIndices(const int& index);
	int getHeightMapVertexCount(const int& index);
	int getHeightMapIndicesCount(const int& index);
	void setShowHeightMap(const bool& show){ mShowHeightMap = show; }
	const bool& getShowHeightMap() { return mShowHeightMap; }
	void clearHeightMap();

	// 场景地形类型图
	const int& getTerrainTypeMapPrecision() { return mTerrainTypeMapPrecision; }
	int getTerrainTypeMapSize() { return getTerrainTypeMapWidth() * getTerrainTypeMapHeight(); }
	int getTerrainTypeMapWidth() { return mSceneWidth * mTerrainTypeMapPrecision + 1; }
	int getTerrainTypeMapHeight() { return mSceneHeight * mTerrainTypeMapPrecision + 1; }
	int* getTerrainTypeMap() { return mTerrainTypeMap; }
	void setTerrainTypeMapData(int* heightMap, const int& heightMapSize);
	void setTerrainTypeMapPrecision(const int& precision) { mTerrainTypeHeightMapPrecision = precision; }
	GLfloat* getTerrainTypeMapVertices(const int& index);
	int getTerrainTypeMapBufferCount() { return mTerrainTypeMapMeshList.size(); }
	GLushort* getTerrainTypeMapIndices(const int& index);
	int getTerrainTypeMapVertexCount(const int& index);
	int getTerrainTypeMapIndicesCount(const int& index);
	void setShowTerrainTypeMap(const bool& show) { mShowTerrainTypeMap = show; }
	const bool& getShowTerrainTypeMap() { return mShowTerrainTypeMap; }
	void clearTerrainTypeMap();

	// 场景地形类型高度图
	const int& getTerrainTypeHeightMapPrecision() { return mTerrainTypeHeightMapPrecision; }
	int getTerrainTypeHeightMapSize() { return getTerrainTypeHeightMapWidth() * getTerrainTypeHeightMapHeight(); }
	int getTerrainTypeHeightMapWidth() { return mSceneWidth * mTerrainTypeHeightMapPrecision + 1; }
	int getTerrainTypeHeightMapHeight() { return mSceneHeight * mTerrainTypeHeightMapPrecision + 1; }
	float* getTerrainTypeHeightMap() { return mTerrainTypeHeightMap; }
	void setTerrainTypeHeightMapData(int* heightMap, const int& heightMapSize);
	void setTerrainTypeHeightMapPrecision(const int& precision) { mTerrainTypeHeightMapPrecision = precision; }
	GLfloat* getTerrainTypeHeightMapVertices(const int& index);
	int getTerrainTypeHeightMapBufferCount() { return mTerrainTypeHeightMapMeshList.size(); }
	GLushort* getTerrainTypeHeightMapIndices(const int& index);
	int getTerrainTypeHeightMapVertexCount(const int& index);
	int getTerrainTypeHeightMapIndicesCount(const int& index);
	void setShowTerrainTypeHeightMap(const bool& show) { mShowTerrainTypeHeightMap = show; }
	const bool& getShowTerrainTypeHeightMap() { return mShowTerrainTypeHeightMap; }
	void clearTerrainTypeHeightMap();
	txMap<int, txRouteRender*>& getRouteRenderList() { return mRouteRender; }
	txRouteRender* getRouteRender(int i) 
	{
		auto iter = mRouteRender.find(i);
		if (iter != mRouteRender.end())
		{
			return iter->second;
		}
		return NULL;
	}
	int addRouteRender(txVector<VECTOR3>& pointList);
	void destroyRouteRender(const int& i);
	void destroyRouteRender(txRouteRender* routeRender);
protected:
	bool loadJsonScene(const std::string& sceneName);
	bool loadBinaryScene(const std::string& sceneName);
protected:
	bool mVisible;
	std::string mSceneName;
	// 静态实体列表
	txMap<std::string, txObject*> mSceneObjects;
	// 不能在多线程中加载的纹理列表
	txVector<txTexture*> mNoneAsyncSceneTexturs;
	// 平移后场景的原点
	VECTOR3 mSceneMove;
	// 场景的初始原点
	VECTOR3 mSceneOrigin;
	// 场景的包围盒
	txAABox mSceneAABB;
	// 场景X轴的长度,往上取整
	int mSceneWidth;
	// 场景Z轴的长度,往上取整
	int mSceneHeight;
	// 场景高度图
	float* mSceneHeightMap;
	// 场景高度图的精度,值越大,则精度越高
	int mHeightMapPrecision;
	// 渲染高度图相关,每一个顶点索引对是一个高度图模型
	txVector<std::pair<GLushort*, GLfloat*> > mHeightMapMeshList;
	// 是否渲染高度图
	bool mShowHeightMap;

	// 场景中地形类型图
	int* mTerrainTypeMap;
	int mTerrainTypeMapPrecision;
	txVector<std::pair<GLushort*, GLfloat*> > mTerrainTypeMapMeshList;
	bool mShowTerrainTypeMap;

	// 场景中地形类型的高度图
	float* mTerrainTypeHeightMap;
	int mTerrainTypeHeightMapPrecision;
	txVector<std::pair<GLushort*, GLfloat*> > mTerrainTypeHeightMapMeshList;
	bool mShowTerrainTypeHeightMap;

	static int mRouteIndexSeed;
	txMap<int, txRouteRender*> mRouteRender;
};

#endif