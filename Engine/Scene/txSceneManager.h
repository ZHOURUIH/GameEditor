#ifndef _TX_SCENE_MANAGER_H_
#define _TX_SCENE_MANAGER_H_

#include "txEngineDefine.h"
#include "txMathUtility.h"
#include "txEngineBase.h"

class txObject;
class txNode;
class txScene;
class txSkyBox;
class txSceneManager : public txEngineBase
{
public:
	txSceneManager();
	virtual ~txSceneManager();
	virtual void init();
	void destroy();
	bool notifySceneRenamed(const std::string& oldName, txScene* scene);
	txNode* getRootNode(){ return mRootNode; }
	void destroySkyBox();
	txSkyBox* createSkyBox(const float& fDistance, const float& fUpDistance, const float& fBottomDistance, const std::string& texturePath);
	txSkyBox* getSkyBox(){ return mSkyBox; }
	txScene* newScene(const std::string& newSceneName);
	// 从场景文件加载场景
	bool loadSceneFromFile(const char* sceneName, const bool& binary);
	// 从已加载的场景拷贝一个新的完全相同的场景
	bool copyScene(const char* srcSceneName, const char* newSceneName);
	void update(float fElaspedTime);
	// 异步加载方法后创建纹理,如果sceneName为空,则表示为所有场景加载纹理,不为空则为指定场景加载纹理
	void createSceneTexture(const std::string& sceneName);
	void generateSceneAABB(const std::string& sceneName);
	void setSceneFog(const VECTOR3& fogColor, const float& nearDis, const float& farDis)
	{
		mFogColor = fogColor;
		mNearDis = nearDis;
		mFarDis = farDis;
	}
	const VECTOR3& getSceneFogColor(){ return mFogColor; }
	const float& getSceneFogNearDis(){ return mNearDis; }
	const float& getSceneFogFarDis(){ return mFarDis; }
	void setSkyFogCoef(const float& k){ fSky_FogCoef = k; }
	const float& getSkyFogCoef(){ return fSky_FogCoef; }
	txMap<std::string, txScene*>& getSceneList() { return mSceneList; }
	txScene* getScene(const std::string& sceneName);
	bool setSceneVisible(const std::string& sceneName, const bool& visible);
	bool getSceneEntityList(const std::string& sceneName, txMap<std::string, txObject*>& entityList);
	bool moveSceneEntities(const std::string& sceneName, const VECTOR3& delta);
	bool alignedToWorldOrigin(const std::string& sceneName, const bool& alignedHeight = false);
	bool setSceneMove(const std::string& sceneName, const VECTOR3& pos);
	void destroyScene(const std::string& sceneName);
	void destroyAllScene();
	bool getHeight(const float& x, const float& z, float& y, const std::string& sceneName = EMPTY_STRING);
	TERRAIN_TYPE getTerrainType(const float& x, const float& z, const std::string& sceneName = EMPTY_STRING);
public:
	// 设置场景阴影的步骤：(这些设置会反映到相机中被利用)
	// 1、设置光源的位置（平行光截断位置）和光源的方向
	// 2、设置观察的矩形尺寸（左右、下上、近远）
	// 3、设置阴影图的尺寸（正方形的纹理）
	// 4、选择是否开启阴影
	//设置光源的位置
	void setLightPosition(const VECTOR3& position){ mLightPosition = position; }
	//设置光源的方向
	void setLightDirection(const VECTOR3& direction){ mLightDirection = direction; }
	//设置光位置观察的立方体范围
	void setLightOrtho(const float& left, const float& right, const float& bottom, const float& up, const float& fNear, const float& fFar)
	{
		mLightOrthoMatrix = txMath::ortho(left, right, bottom, up, fNear, fFar);
	}
	//设置阴影图尺寸
	void setShadowSize(const int& nTextureSize){ mShadowTextureSize = nTextureSize; }
	//控制是否开启阴影效果
	void switchShadowMap(const bool& bOpen){ bSwitchShadowMap = bOpen; }
	const MATRIX4& getLightOrthoMatrix(){ return mLightOrthoMatrix; }
	const VECTOR3& getLightPosition(){ return mLightPosition; }
	const VECTOR3& getLightDirection(){ return mLightDirection; }
	const int& getShadowSize(){ return mShadowTextureSize; }
	const bool& getOpenShadowMap(){ return bSwitchShadowMap; }
	void lock();
	void unlock();
protected:
	// 场景根节点
	txNode* mRootNode;
	txSkyBox* mSkyBox;
	VECTOR3 mFogColor;
	float mNearDis;
	float mFarDis;
	float fSky_FogCoef;
	int mShadowTextureSize;
	MATRIX4 mLightOrthoMatrix;
	VECTOR3 mLightPosition;
	VECTOR3 mLightDirection;

	//场景的阴影图
	GLuint shadowMapId;
	bool bSwitchShadowMap;

	// 各个场景列表,与mEntitiesNameSets中的实体没有重叠部分
	txMap<std::string, txScene*> mSceneList;

	// 是否锁定场景更新
	bool mLockUpdate;
	// 线程锁暂不使用
#if RUN_PLATFORM == PLATFORM_WINDOWS
	HANDLE mMutex;
#elif RUN_PLATFORM == PLATFORM_ANDROID
	pthread_mutex_t mMutex;
#endif
};

#endif