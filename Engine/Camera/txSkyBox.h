#ifndef _SKY_BOX_H_
#define _SKY_BOX_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txNode;
class txEntity;
class txCamera;
class txSkyBox : public txEngineBase
{
public:
	txSkyBox();
	virtual ~txSkyBox();
	// texturePath是以/结尾的media下的相对路径
	void init(const float& fDistance, const float& fUpDistance, const float& fBottomDistance, const std::string& texturePath);
	void update(float elapsedTime){}
	void render(txCamera* camera);
	void destroy();
	void setSkyboxPerspective(const float& fovY, const float& nearClip, const float& farClip);
	const float& getFovY() { return mFovY; }
	const float& getRatio() { return mRatio; }
	const float& getNearClip() { return mNearClip; }
	const float& getFarClip() { return mFarClip; }
protected:
	txEntity* mBoxEntity;
	std::string mBoxEntityName;
	MATRIX4 mSkyboxPerspective;
	float mFovY;
	float mRatio;
	float mNearClip;
	float mFarClip;
	// 不将天空盒挂到任何场景节点上,不然摄像机在渲染场景时会把天空盒当成普通的实体渲染出来
};

#endif