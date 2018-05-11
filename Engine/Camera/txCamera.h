#ifndef  _TX_CAMERA_H_
#define  _TX_CAMERA_H_

#include "txNode.h"
#include "txObject.h"
#include "txDirLight.h"
#include "txEntity.h"
#include "txSubEntity.h"
#include "txTexture.h"
#include "txFrustumR.h"
#include "txEngineBase.h"

//场景半透明物体特殊处理
struct RenderStruct
{
	RenderStruct(const float& entityDepth, txSubEntity* pSubEntity)
	{
		mEntityDepth = entityDepth;
		mpSubEntity = pSubEntity;
	}
	RenderStruct& operator=(const RenderStruct& aMess)
	{
		if (this != &aMess)
		{
			mEntityDepth = aMess.mEntityDepth;
			mpSubEntity = aMess.mpSubEntity;
		}
		return *this;
	}
	float mEntityDepth;
	txSubEntity* mpSubEntity;
};

class txViewport;
class txSceneManager;
class txCamera : public txEngineBase
{
public:
	txCamera();
	virtual ~txCamera();

	void setName(const std::string& name) { mName = name; }
	const std::string& getName() { return mName; }

	void render(txSceneManager* pSceneManager);
	void setViewport(const float& topX, const float& topY, const float& width, const float& height);
	void setActive(const bool& v){ mActive = v; }
	void setViewMatrix(const MATRIX4& view){ mViewMatrix = view; }
	void setPerspectiveMatrix(const MATRIX4& mat) { mPerspectiveMatrix = mat; }
	const MATRIX4& getViewMatrix(){ return mViewMatrix; }
	const MATRIX4& getPerspectiveMatrix() { return mPerspectiveMatrix; }
	MATRIX4 getVPMatrix() { return mPerspectiveMatrix * mViewMatrix; }
	void setPerspectiveParameters(const float& angle, const float& ratio, const float& nearp, const float& farp);
	void setAspect(const float& aspect);
	void setDirLight(txDirLight* dirLight);
	void renderHeightMap(txSceneManager* pSceneManager);
	void renderSceneRoute(txSceneManager* pSceneManager);
	void renderStaticMeshAABB(txMesh* pMesh, const MATRIX4& transMat, const VECTOR4& colour = VECTOR4(1.0f, 1.0f, 0.0f, 1.0f));
	void renderDynamicMeshAABB(txMesh* pMesh, const MATRIX4& transMat, const VECTOR4& colour = VECTOR4(1.0f, 1.0f, 0.0f, 1.0f));

	//阴影图绘制
	void Entity_RenderNode_ShadowMap(txNode* node);
	void DrawSceneToTexture(txSceneManager* pSceneManager);
	// 获取阴影图
	GLuint GetShadowMap() {return shadowMapId;}
	//得到渲染出的深度值
	void DrawDepthMap(txSceneManager* pSceneManager);
	//直接将深度值设置到颜色缓冲区上得到输出纹理
	void GenerateDepthMap();
	void SetShadowMapSize(GLsizei TEXTURE_SIZE);
	//将深度图输出到文件中
	void OutputShadowMapToFile();
protected:

	void Entity_RenderEntity(txEntity* ent, txDirLight* light, const VECTOR3& lightvpos);
	void Entity_RenderSubEntity(txSubEntity* pSubEntity, txDirLight* light, const VECTOR3& lightvpos);
	void Entity_RenderNode(txNode* node, txDirLight* light, const VECTOR3& lightvpos);

	void Light_RenderDirLight(txNode* sceneNode, txDirLight* light, const VECTOR3& lightvpos);
	void Light_RenderNode(txNode* node, const VECTOR3& lightvpos);

	void PushOneTransparentEntity(txEntity* pEntity);
	void RenderTransparentEntity();
protected:
	std::string mName;
	//场景的深度图
	GLuint shadowMapId;
	//深度图的尺寸
	GLsizei mTEXTURE_SIZE;
	//渲染缓冲区内容
	GLuint mFbo;		// FBO对象的句柄
	GLuint mDepthbuffer;
	GLuint mRendertarget;		// 纹理对象的句柄
	//投影参数
	float mAngle;
	float mRatio;
	float mNearp;
	float mFarp;
	bool bOpenShadowMap;
	txViewport* mViewport;
	txDirLight* pDirLight;
	txFrustumR frustum;
	bool mActive;
	MATRIX4 mPerspectiveMatrix;
	MATRIX4 mViewMatrix;
	MATRIX4 mLightProjectionMatrix;
	MATRIX4 mLightViewMatrix;
	//半透明实体特殊处理
	txVector<RenderStruct> mTransparentEntity;
};

#endif