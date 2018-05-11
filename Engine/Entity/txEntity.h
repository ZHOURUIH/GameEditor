#ifndef _TX_ENTITY_H_
#define _TX_ENTITY_H_

#include "txEngineDefine.h"
#include "txObject.h"
#include "txMesh.h"
#include "txMaterial.h"
#include "txSkeleton.h"
#include "txAnimation.h"
#include "txSkin.h"
#include "txEngineEventHandler.h"
#include "txEngineBase.h"

class txSubEntity;
class txEntity : public txObject, public txEngineEventHandler
{
public:
	txEntity(const std::string& name);
	virtual ~txEntity();
	void clear();

	virtual void registerEngineEventHandler();
	virtual void unregisterEngineEventHandler();
	virtual void onEngineEvent(const ENGINE_EVENT& event, const txVector<std::string>& paramList);

	bool isAnimEntity();

	// 新建一个空的动作,并返回创建出的动作
	txAnimation* CreateAnimation(const std::string& name);
	// 从一个动作拷贝一份动作并且添加到自己的动作列表中,此处的CopyType应该与实体的拷贝类型一致,否则会有内存泄漏或者程序崩溃
	void CopyAnimation(const std::string& name, txAnimation* pSrcAnimation, const ENTITY_COPY_TYPE& type);

	// 得到所有子实体的材质
	txVector<txMaterial*> getAllMaterials();

	// 初始化所有子实体的纹理
	void GenerateAllSubEntitiesTextures();

	// 会调用骨骼的同名函数返回结果
	txBoneHook* CreateBoneHook(const std::string& boneName, const std::string& hookName);
	// 会调用骨骼的同名函数返回结果
	txBoneHook* GetBoneHook(const std::string& boneName, const std::string& hookName);
	// 会调用骨骼的同名函数返回结果
	bool DestroyBoneHook(const std::string& boneName, const std::string& hookName);
	// 会调用骨骼的同名函数返回结果
	txBoneHook* GetHookInAllBones(const std::string& hookName);
	// 会调用骨骼的同名函数返回结果
	bool DestroyHookInAllBones(const std::string& hookName);
	// 会调用骨骼的同名函数返回结果
	bool HasHook(const std::string& boneName, const std::string& hookName);
	// 会调用骨骼的同名函数返回结果
	bool HasHookInAllBones(const std::string& hookName);

	//update the txEntity
	virtual void Update(float fElaspedTime);

	void refreshSkeleton();

	// 当父节点有改变时,下次获取世界包围盒时需要重新计算包围盒
	virtual void NotifyParentUpdated(){ setWorldAABBDirty(true); }

	txSubEntity* CreateSubEntity(txMesh* mesh, txSkin *pSkin = NULL);
	void DelSubEntity(txMesh*  mesh);

	txVector<txSubEntity*>& getSubEntityList() { return mVecSub; }
	void setMeshFileName(const std::string& fileName) { mMeshFileName = fileName; }
	const std::string& getMeshFileName() { return mMeshFileName; }

	txSkeleton* GetSkeleton() { return mSkeleton; }

	void setVisible(const bool& visible) { mVisible = visible; }
	const bool& isVisible() { return mVisible; }
	void setActive(const bool& active) { mActive = active; }
	const bool& isActive() { return mActive; }
	const ENTITY_COPY_TYPE& getCopyType() { return mCopyType; }

	// 拷贝实体的所有数据
	void CopyEntity(txEntity* pEntity, const ENTITY_COPY_TYPE& type);
	// 播放动画相关,其中的time全是以秒为单位,每秒对应30帧
	void playClip(const std::string& animName, int beginFrame, int endFrame, const bool& loop = false, float speed = 1.0f, const float& timeOffset = 0.0f,
		const std::string& hookAnim = EMPTY_STRING, const int& hookBeginFrame = -1, const int& hookEndFrame = -1, const bool& hookAnimLoop = false, float hookAnimSpeed = 1.0f,
		const float& hookAnimTimeOffset = 0.0f);
	
	// keyFrame是动作关键帧
	bool addAnimKeyFrameCallback(AnimKeyFrameCallback keyFrameCallback, const float& keyFrame, void* userData = NULL, const int& aditionalParam = -1, const bool& callBackOnce = false, const bool& onlyForCurAnim = true)
	{
		if (mKeyFrameCallbackList.find(keyFrame) != mKeyFrameCallbackList.end())
		{
			return false;
		}
		AnimKeyFrameCallbackInfo info;
		info.mAlreadyCallback = false;
		info.mCallback = keyFrameCallback;
		info.mCallbackOnce = callBackOnce;
		info.mKeyFrame = keyFrame;
		info.mOnlyForCurAnim = onlyForCurAnim;
		info.mUserData = userData;
		info.mAditionalParam = aditionalParam;
		mKeyFrameCallbackList.insert(keyFrame, info);
		return true;
	}
	bool setAnimEndCallback(AnimKeyFrameCallback keyFrameCallback, void* userData = NULL, const int& aditionalParam = -1, const bool& callBackOnce = false, const bool& onlyForCurAnim = true)
	{
		float animLength = mCurAnim->getLength();
		return addAnimKeyFrameCallback(keyFrameCallback, animLength, userData, aditionalParam, callBackOnce, onlyForCurAnim);
	}
	// 用于从暂停状态继续播放动作
	void play(){mPlayState = PS_PLAY;}
	void pause(){mPlayState = PS_PAUSE;}
	void stop();
	void setAnimSpeed(const float& speed){ mAnimSpeed = speed; }
	void setCurPlayTime(const float& time){ mCurPlayTime = time; }
	void updateAnim(const float& elapsedTime);
	txAnimation* getCurAnim() { return mCurAnim; }
	const std::string& getCurAnimName() { return mCurAnim != NULL ? mCurAnim->GetAnimationName() : EMPTY_STRING; }
	std::pair<int, int> getCurAnimFrameClip() { return std::pair<int, int>(mBeginFrame, mEndFrame); }
	const float& getCurAnimSpeed() { return mAnimSpeed; }
	const bool& getCurAnimLoop() { return mCurLoop; }
	txAnimation* getHookAnim() { return mHookAnim; }
	const std::string& getHookAnimName() { return mHookAnim != NULL ? mHookAnim->GetAnimationName() : EMPTY_STRING; }
	float getAnimLength(const std::string& animName);
	void setAnimPlayState(const PLAY_STATE& state);
	const PLAY_STATE& getAnimPlayState(){ return mPlayState; }

	// 在模型的本地空间作射线检测,返回的是距离射线原点最近的交点
	bool rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint);

	// 得到本地的包围盒，动态模型的包围盒会经过一些处理
	const txAABox& getAABB();
	const txAABox& getWorldAABB();

	// 取消动作的平滑切换
	void cutAnimLerp()
	{
		// 将上一个动作清零,转换时间设置为最大值,表示已经转换完成
		mLastAnim = NULL;
		mCurAnimSwitchTime = ANIM_SWITCH_TIME;
	}
	const bool& getShowAABB() { return mShowAABB; }
	const bool& isWorldAABBDirty() { return mWorldAABBDirty; }
	bool isTransparent() { return mTransparent || mAlpha < 1.0f; }
	const float& getAlpha() { return mAlpha; }
	const bool& getInView() { return mInView; }
	const float& getLoadPercent() { return mLoadPercent; }

	void setShowAABB(const bool& show) { mShowAABB = show; }
	void setWorldAABBDirty(const bool& dirty) { mWorldAABBDirty = dirty; }
	void setTransparent(const bool& transparent) { mTransparent = transparent; }
	void setAlpha(const float& alpha) { mAlpha = alpha; }
	void setInView(const bool& inView) { mInView = inView; }
	
	txMap<std::string, txAnimation*>& getAnimationSet() { return mAnimationSet; }
protected:
	//播放动画,仅内部使用,播放动画到指定的帧
	void animate(const float& frame);
	// 设置加载进度
	void setLoadPercent(const float& percent);
	// 用于循环播放时的从头播放
	void restart(const float& timeOffset = 0.0f, const float& speed = 1.0f);

protected:
	ENTITY_COPY_TYPE mCopyType;
	bool mVisible;
	bool mInView;	// 摄像机是否可看见实体,如果摄像机看不见实体,则不更新实体
	bool mActive; // 是否激活,如果不激活则不更新动画
	bool mTransparent;	//指定实体Entity是否是半透明的(是指材质本身是否是半透的)
	float mAlpha; // 实体的透明度,1为不透明,0为完全透明,该值是逻辑控制修改的,与mTransparent没有关系
	txVector<txSubEntity*> mVecSub;
	std::string mMeshFileName; // 模型文件名,不带路径

	txSkeleton* mSkeleton;
	txMap<std::string, txAnimation*> mAnimationSet;

	// 动作关键帧回调函数,仅支持回调一次或者对当前循环播放的动作的循环回调,切换动作后所有回调将会失效
	txMap<float, AnimKeyFrameCallbackInfo> mKeyFrameCallbackList;
	
	float mCurAnimSwitchTime;		// 当前动作切换已经经过的时间
	txAnimation* mLastAnim;			// 上一个播放的动画
	int mLastBeginFrame;			// 上个动画播放的起始帧
	int mLastEndFrame;				// 上个动画播放的终止帧
	float mLastAnimFarme;			// 上一个动画在播放到多少帧时切换到了当前动画
	float mAnimSpeed;				// 动画播放的速度
	int mBeginFrame;				// 当前动画的开始帧,为-1则表示从0开始
	int mEndFrame;					// 当前动画的结束帧,为-1则表示播放到动作结尾
	txAnimation* mCurAnim;			// 当前播放的动画
	bool mCurLoop;					// 当前动画是否循环播放
	float mCurPlayTime;				// 当前动画已经播放的时间
	// 挂接动画,挂接动画只生效一次,生效后或者停止动画后都会失效
	txAnimation* mHookAnim;			// 挂接的下一个动画
	float mHookAnimSpeed;			// 挂接动画的播放速度
	bool mHookAnimLoop;				// 挂接动画是否循环播放
	float mHookAnimTimeOffset;		// 挂接动画的播放时间偏移
	int mHookBeginFrame;			// 挂接动画的开始帧,为-1则表示从0开始
	int mHookEndFrame;				// 挂接动画的结束帧,为-1则表示播放到动作结尾
	float mLoadPercent;				// 实体的加载进度
	float mSkeletonPercent;			// 骨骼所占的加载百分比
	float mModelPercent;			// 模型文件所占的加载百分比
	int mAnimationCountToLoad;		// 需要加载的动画数量
	bool mShowAABB;					// 是否显示包围盒
	txAABox mLocalAABB;				// 本地的包围盒
	txAABox mWorldAABB;				// 世界包围盒
	bool mWorldAABBDirty;			// 是否需要计算世界包围盒
	PLAY_STATE mPlayState;			// 当前播放状态
};

#endif