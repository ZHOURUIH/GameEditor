#ifndef _TX_BILLBOARD_H_
#define _TX_BILLBOARD_H_

#include "txObject.h"
#include "txEngineDefine.h"

class txNode;
class txTexture;
class txCamera;
class txRendererTexture;

class txBillboard : public txObject
{
public:
	txBillboard(const std::string& name);
	virtual ~txBillboard();
	void init(txCamera* camera);
	virtual void NotifyParentUpdated(){}
	void destroyTexture();
	void destroyVertex();
	// 创建公告板的顶点数据,需要先设置好宽高
	void createVertex();
	// 设置序列帧图片名称,会创建图片
	// frameCount是序列帧数,如果不是序列帧,则为1,如果不为1,则根据序列帧名称来加载图片,比如序列帧图片命名为a_0.swi, a_1.swi, a_2.swi,则textureName应为a_.swi,frameCount为3
	void setTextureName(const std::string& textureName, const int& frameCount);
	// 设置序列帧播放的持续时间
	void setDuration(const float& time){ mDuration = time; }
	// 设置公告板中心的位置,相对于父节点的
	void setPosition(const VECTOR3& position){ mParent->SetPosition(position); }
	const VECTOR3& getPosition(){ return mParent->GetPosition(); }
	// 设置公告板中心的位置,绝对的世界坐标
	void setWorldPosition(const VECTOR3& position){ mParent->SetWorldPosition(position); }
	const VECTOR3& getWorldPosition(){ return mParent->GetWorldPosition(); }
	// 设置公告板的宽高
	void setSize(const VECTOR2& size) { mWidth = size.x; mHeight = size.y; }
	VECTOR2 getSize() { return VECTOR2(mWidth, mHeight); }
	// 设置父节点
	void setParentNode(txNode* parentNode);
	txNode* getParentNode(){ return GetParent(); }
	// 设置是否自动销毁
	void setAutoDestroy(const bool& value){ mAutoDestroy = value; }
	const bool& getAutoDestroy(){ return mAutoDestroy; }
	// 刷新公告板的旋转
	virtual void Update(float fElaspedTime);
	// 渲染公告板
	void render();
	void stop(const bool& visible = false);
	void pause(const bool& visible = false)
	{
		mBillboardState = PS_PAUSE;
		setVisible(visible);
	}
	void play(const bool& visible = true)
	{
		mBillboardState = PS_PLAY;
		setVisible(visible);
	}
	void setState(const PLAY_STATE& state){ mBillboardState = state; }
	const PLAY_STATE& getState(){ return mBillboardState; }
	void setVisible(const bool& visible){ mVisible = visible; }
	const bool& getVisible(){ return mVisible; }
	void setHideAndStopPlayEnd(const bool& hide){ mHideAndStopPlayEnd = hide; }
	const bool& getHideAndStopPlayEnd(){ return mHideAndStopPlayEnd; }
	void setAlpha(const float& alpha) { mAlpha = alpha; }
	const float& getAlpha() { return mAlpha; }
	void setColour(const VECTOR3& colour) { mColour = colour; }
	const VECTOR3& getColour() { return mColour; }
	void setColourBlendMode(const BLEND_MODE& mode) { mColourBlendMode = mode; }
	const BLEND_MODE& getColourBlendMode() { return mColourBlendMode; }

protected:
	txVector<std::string> mTextureNameList; 	// 公告板纹理列表,公告板的纹理由公告板来管理,不放到场景管理器中
	float mDuration;		// 纹理播放的持续时间,也就是一个完整序列帧的播放总时间
	float mCurPlayTime;		// 当前已经播放的时间
	int mCurTextureIndex;	// 当前播放的序列帧下标
	txTexture* mCurTexture;	// 当前显示的纹理
	txCamera* mCamera;		// 摄像机
	float mWidth;			// 公告板的宽, x方向上的
	float mHeight;			// 公告板的高, z方向上的
	bool mAutoDestroy;		// 是否在播放完一次序列帧后自动销毁
	GLfloat* mVertices;
	GLfloat* mTexCoord;
	GLushort* mIndices;
	float mAlpha;
	VECTOR3 mColour;
	BLEND_MODE mColourBlendMode;
	bool mVisible;			// 是否处于隐藏状态
	bool mHideAndStopPlayEnd; // 是否在播放一次循环结束后隐藏并且停止,如果设置了自动销毁则该变量无效
	PLAY_STATE mBillboardState;
	static txRendererTexture* mTextureRenderer;
};

#endif