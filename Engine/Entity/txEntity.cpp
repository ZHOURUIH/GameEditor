#include "txEntity.h"
#include "txSubEntity.h"
#include "txSkinBone.h"
#include "txBone.h"
#include "txEngineLog.h"
#include "txSceneManager.h"
#include "txMathUtility.h"
#include "txEngineEventSystem.h"
#include "txEngineRoot.h"

txEntity::txEntity(const std::string& name)
:
txObject(OT_ENTITY),
mVisible(true),
mInView(true),
mActive(true),
mTransparent(false),
mAlpha(1.0f)
{
	setName(name.c_str());
	mVecSub.clear();
	mSkeleton = TRACE_NEW(txSkeleton, mSkeleton, this);
	mCopyType = ECT_NONE;

	stop();
	mShowAABB = false;
	mLoadPercent = 0.0f;
	mAnimationCountToLoad = 0;
	mSkeletonPercent = 0.05f;
	mModelPercent = 0.1f;
	mWorldAABBDirty = true;

	registerEngineEventHandler();
}

txEntity::~txEntity()
{
	unregisterEngineEventHandler();
	clear();
	// 如果是浅拷贝则不销毁子实体,骨骼,动作,因为这是其他实体的子实体,骨骼,动作
	if (mCopyType == ECT_SIMPLE)
	{
		mSkeleton = NULL;
	}
	// 动态模型的浅拷贝则需要销毁骨骼,因为骨骼是深度拷贝出来的,其他的直接清空
	// 深度拷贝或者是由文件加载来的则需要将所有数据释放掉
	else
	{
		TRACE_DELETE(mSkeleton);
	}
}

void txEntity::clear()
{
	mMeshFileName = "";
	mKeyFrameCallbackList.clear();
	stop();
	// 如果是浅拷贝则不销毁子实体,骨骼,动作,因为这是其他实体的子实体,骨骼,动作
	if (mCopyType == ECT_SIMPLE)
	{
		mVecSub.clear();
		mAnimationSet.clear();
	}
	// 动态模型的浅拷贝则需要销毁骨骼,因为骨骼是深度拷贝出来的,其他的直接清空
	else if (mCopyType == ECT_ANIM_SIMPLE)
	{
		mSkeleton->Clear();
		mVecSub.clear();
		mAnimationSet.clear();
	}
	// 深度拷贝或者是由文件加载来的则需要将所有数据释放掉
	else if (mCopyType == ECT_DEEP || mCopyType == ECT_NONE)
	{
		mSkeleton->Clear();
		int subEntityCount = mVecSub.size();
		if (subEntityCount > 0)
		{
			FOR_STL(mVecSub, int i = 0; i < subEntityCount; ++i)
			{
				TRACE_DELETE(mVecSub[i]);
			}
			END_FOR_STL(mVecSub);
			mVecSub.clear();
		}
		if (mAnimationSet.size() > 0)
		{
			auto iterAnim = mAnimationSet.begin();
			auto iterAnimEnd = mAnimationSet.end();
			FOR_STL(mAnimationSet, ; iterAnim != iterAnimEnd; ++iterAnim)
			{
				TRACE_DELETE(iterAnim->second);
			}
			END_FOR_STL(mAnimationSet);
			mAnimationSet.clear();
		}
	}
}

void txEntity::registerEngineEventHandler()
{
	mEngineEventSystem->registeAllEvent(this);
}

void txEntity::unregisterEngineEventHandler()
{
	mEngineEventSystem->unregisteAllEvent(this);
}

void txEntity::onEngineEvent(const ENGINE_EVENT& event, const txVector<std::string>& paramList)
{
	// 开始加载动态实体,参数为实体名
	if (event == EE_START_LOAD_DYNAMIC_ENTITY)
	{
		if (mName != paramList[0])
		{
			return;
		}
		setLoadPercent(0.0f);
	}
	// 动态实体的模型文件加载完成,参数1是实体名,参数2是模型文件名
	else if (event == EE_DYNAMIC_ENTITY_MODEL_LOADED)
	{
		if (mName != paramList[0])
		{
			return;
		}
		setLoadPercent(mLoadPercent + mModelPercent);
	}
	// 动态实体的骨骼文件加载完成,参数1是实体名,参数2是骨骼文件名
	else if (event == EE_DYNAMIC_ENTITY_SKELETON_LOADED)
	{
		if (mName != paramList[0])
		{
			return;
		}
		setLoadPercent(mLoadPercent + mSkeletonPercent);
	}
	// 开始加载动画,参数1是实体名,参数2是动画的数量
	else if (event == EE_START_LOAD_ENTITY_ANIMATION)
	{
		if (mName != paramList[0])
		{
			return;
		}
		mAnimationCountToLoad = txStringUtility::stringToInt(paramList[1].c_str());
	}
	// 一个动画加载完成,参数1是实体名,参数2是独立动画名,参数3是完整动画文件名
	else if (event == EE_ONE_ENTITY_ANIMATION_LOADED)
	{
		if (mName != paramList[0])
		{
			return;
		}
		setLoadPercent(mLoadPercent + (1.0f - mModelPercent - mSkeletonPercent) / (float)mAnimationCountToLoad);
	}
	// 动态实体的动画全部加载完成,参数1是实体名,参数2是动画数量
	else if (event == EE_ENTITY_ALL_ANIMATION_LOADED)
	{
		if (mName != paramList[0])
		{
			return;
		}
		setLoadPercent(1.0f);
	}
	// 动态实体加载完成,参数为实体名
	else if (event == EE_END_DYNAMIC_ENTITY_LOAD)
	{
		if (mName != paramList[0])
		{
			return;
		}
		setLoadPercent(1.0f);
	}
}

bool txEntity::isAnimEntity()
{
	return mSkeleton != NULL && !mSkeleton->IsSkeletonEmpty();
}

txAnimation* txEntity::CreateAnimation(const std::string& name)
{
	txAnimation* animation = TRACE_NEW(txAnimation, animation);
	animation->SetAnimationName(name);
	mAnimationSet.insert(name, animation);
	return animation;
}

void txEntity::CopyAnimation(const std::string& name, txAnimation* pSrcAnimation, const ENTITY_COPY_TYPE& type)
{
	if (type == ECT_DEEP)
	{
		txAnimation* animation = TRACE_NEW(txAnimation, animation);
		animation->CopyAnimation(pSrcAnimation);
		animation->SetAnimationName(name);
		mAnimationSet.insert(name, animation);
	}
	else if (type == ECT_ANIM_SIMPLE || type == ECT_SIMPLE)
	{
		mAnimationSet.insert(name, pSrcAnimation);
	}
}

txVector<txMaterial*> txEntity::getAllMaterials()
{
	txVector<txMaterial*> materialList;
	int subEntityCount = mVecSub.size();
	FOR_STL(mVecSub, int i = 0; i < subEntityCount; ++i)
	{
		txVector<txMaterial*> subMaterialList = mVecSub[i]->getAllMaterials();
		int subMaterialCount = subMaterialList.size();
		FOR_STL(subMaterialList, int j = 0; j < subMaterialCount; ++j)
		{
			materialList.push_back(subMaterialList[i]);
		}
		END_FOR_STL(subMaterialList);
	}
	END_FOR_STL(mVecSub);
	return materialList;
}

void txEntity::GenerateAllSubEntitiesTextures()
{
	int subEntityCount = mVecSub.size();
	FOR_STL(mVecSub, int i = 0; i < subEntityCount; ++i)
	{
		mVecSub[i]->GenerateSubEntityTextures();
	}
	END_FOR_STL(mVecSub);
}

txBoneHook* txEntity::CreateBoneHook(const std::string& boneName, const std::string& hookName)
{
	if (mSkeleton == NULL)
	{
		ENGINE_ERROR("error : skeleton is NULL, can not create hook.");
		return NULL;
	}
	return mSkeleton->CreateBoneHook(boneName, hookName);
}

txBoneHook* txEntity::GetBoneHook(const std::string& boneName, const std::string& hookName)
{
	if (mSkeleton == NULL)
	{
		ENGINE_ERROR("error : skeleton is NULL, can not get hook.");
		return NULL;
	}
	return mSkeleton->GetBoneHook(boneName, hookName);
}

bool txEntity::DestroyBoneHook(const std::string& boneName, const std::string& hookName)
{
	if (mSkeleton == NULL)
	{
		ENGINE_ERROR("error : skeleton is NULL, can not destroy hook.");
		return false;
	}
	return mSkeleton->DestroyBoneHook(boneName, hookName);
}

txBoneHook* txEntity::GetHookInAllBones(const std::string& hookName)
{
	if (mSkeleton == NULL)
	{
		ENGINE_ERROR("error : skeleton is NULL, can not get hook.txEntity::GetHookInAllBones");
		return NULL;
	}
	return mSkeleton->GetHookInAllBones(hookName);
}

bool txEntity::DestroyHookInAllBones(const std::string& hookName)
{
	if (mSkeleton == NULL)
	{
		ENGINE_ERROR("error : skeleton is NULL, can not destroy hook.txEntity::DestroyHookInAllBones");
		return false;
	}
	return mSkeleton->DestroyHookInAllBones(hookName);
}

// 会调用骨骼的同名函数返回结果
bool txEntity::HasHook(const std::string& boneName, const std::string& hookName)
{
	if (mSkeleton == NULL)
	{
		ENGINE_ERROR("error : skeleton is NULL, can not destroy hook.txEntity::HasHook");
		return false;
	}
	return mSkeleton->HasHook(boneName, hookName);
}

// 会调用骨骼的同名函数返回结果
bool txEntity::HasHookInAllBones(const std::string& hookName)
{
	if (mSkeleton == NULL)
	{
		ENGINE_ERROR("error : skeleton is NULL, can not destroy hook.txEntity::HasHookInAllBones");
		return false;
	}
	return mSkeleton->HasHookInAllBones(hookName);
}

void txEntity::animate(const float& frame)
{
	if (mCurAnim != NULL)
	{
		// 如果有上一个动作需要融合,并且不是播放的重复的动作,并且还未切换融合完毕
		if (mLastAnim != NULL && (mLastAnim != mCurAnim || mLastBeginFrame != mBeginFrame || mLastEndFrame != mEndFrame) && mCurAnimSwitchTime < ANIM_SWITCH_TIME)
		{
			if (mCurAnimSwitchTime > ANIM_SWITCH_TIME)
			{
				mCurAnimSwitchTime = ANIM_SWITCH_TIME;
				mLastAnim = NULL;
			}
			// 当前动作所占权重
			float curAnimWeight = mCurAnimSwitchTime / ANIM_SWITCH_TIME;

			txMap<std::string, AnimInfo> curAnimInfo;
			mCurAnim->perform(curAnimInfo, frame);

			txMap<std::string, AnimInfo> lastAnimInfo;
			if (mLastAnim != NULL)
			{
				mLastAnim->perform(lastAnimInfo, mLastAnimFarme);
			}

			auto iter = curAnimInfo.begin();
			auto iterEnd = curAnimInfo.end();
			FOR_STL(curAnimInfo, ; iter != iterEnd; ++iter)
			{
				txBone* bone = mSkeleton->GetBone(iter->first.c_str());
				if (bone != NULL)
				{
					AnimInfo info = iter->second;
					// 如果上一帧动画也有这个骨头的关键帧,那么和当前动作融合
					auto iterLast = lastAnimInfo.find(iter->first);
					if (iterLast != lastAnimInfo.end())
					{
						info.rot = txMath::slerp(iterLast->second.rot, info.rot, curAnimWeight);
						info.scale = iterLast->second.scale + curAnimWeight * (info.scale - iterLast->second.scale);
						info.pos = iterLast->second.pos + curAnimWeight * (info.pos - iterLast->second.pos);
					}
					else
					{
						info.rot = txMath::slerp(txMath::QUAT_IDENTITY, info.rot, curAnimWeight);
						info.scale = VECTOR3(1.0, 1.0, 1.0) + curAnimWeight * (info.scale - VECTOR3(1.0, 1.0, 1.0));
						info.pos = curAnimWeight * info.pos;
					}
					bone->SetRotationMatrix(txMath::quatToMatrix3(info.rot));
					bone->SetPosition(info.pos);
					bone->SetScale(info.scale);
				}
			}
			END_FOR_STL(curAnimInfo);
		}
		// 如果没有动作需要融合并且动作已经切换完毕,则正常播放动作
		else
		{
			txMap<std::string, AnimInfo> curAnimInfo;
			mCurAnim->perform(curAnimInfo, frame);
			auto iter = curAnimInfo.begin();
			auto iterEnd = curAnimInfo.end();
			FOR_STL(curAnimInfo, ; iter != iterEnd; ++iter)
			{
				txBone* bone = mSkeleton->GetBone(iter->first.c_str());
				if (bone != NULL)
				{
					AnimInfo info = iter->second;
					bone->SetRotationMatrix(txMath::quatToMatrix3(info.rot));
					bone->SetPosition(info.pos);
					bone->SetScale(info.scale);
				}
			}
			END_FOR_STL(curAnimInfo);
		}
	}
}

void txEntity::setLoadPercent(const float& percent)
{
	// 发送事件通知实体的加载进度有变化
	if (mLoadPercent != percent)
	{
		mLoadPercent = percent;
		txVector<std::string> paramList;
		paramList.push_back(mMeshFileName);
		paramList.push_back(mName);
		paramList.push_back(txStringUtility::floatToString(mLoadPercent, 4));
		mEngineRoot->sendEvent(EE_DYNAMIC_ENTITY_LOAD_PROGRESS_CHANGED, paramList);
	}
}

//update the txEntity
void txEntity::Update(float fElaspedTime)
{
	// 如果实体没有激活或者不可见,则不更新
	if (!mActive || !mVisible || !mInView)
	{
		return;
	}

	mCurAnimSwitchTime += fElaspedTime;

	// 更新动画
	if (mCurAnim != NULL)
	{
		updateAnim(fElaspedTime);
	}

	// 更新骨骼
	if (mSkeleton != NULL && !mSkeleton->IsSkeletonEmpty())
	{
		mSkeleton->Refresh();

		int subEntityCount = mVecSub.size();
		FOR_STL(mVecSub, int i = 0; i < subEntityCount; ++i)
		{
			txSubEntity* pSubEntity = mVecSub[i];
			pSubEntity->BuildBoneMatrixArray();
		}
		END_FOR_STL(mVecSub);
	}
}

void txEntity::refreshSkeleton()
{
	mSkeleton->Refresh();
}

txSubEntity* txEntity::CreateSubEntity(txMesh* mesh, txSkin* pSkin)
{
	txSubEntity* subEntity = TRACE_NEW(txSubEntity, subEntity, this);
	if (subEntity != NULL)
	{
		subEntity->init(mesh, !mSkeleton->IsSkeletonEmpty());
		mVecSub.push_back(subEntity);
	}

	subEntity->SetMeshSkin(pSkin);
	return subEntity;
}

void txEntity::DelSubEntity(txMesh* mesh)
{
	auto iter = mVecSub.begin();
	auto iterEnd = mVecSub.end();
	FOR_STL(mVecSub, ; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetOriMesh() == mesh)
		{
			mVecSub.erase(iter, false);
			break;
		}
	}
	END_FOR_STL(mVecSub);
}

void txEntity::CopyEntity(txEntity* pEntity, const ENTITY_COPY_TYPE& type)
{
	pEntity->mCopyType = type;
	if (pEntity->mCopyType == ECT_NONE)
	{
		return;
	}
	// 先拷贝部分成员变量和子实体
	pEntity->mTransparent = mTransparent;
	pEntity->mVisible = mVisible;
	pEntity->mShowAABB = mShowAABB;
	int subEntityCount = mVecSub.size();
	FOR_STL(mVecSub, int i = 0; i < subEntityCount; ++i)
	{
		txSubEntity* thisSubEntity = mVecSub[i];
		// 拷贝子实体
		// 浅拷贝直接将子实体的指针赋值给拷贝出的实体
		// 深度拷贝则完全将实体拷贝一份
		txSubEntity* pSubEntity = TRACE_NEW(txSubEntity, pSubEntity, pEntity);
		thisSubEntity->CopySubEntity(pSubEntity, pEntity->mCopyType);
		pEntity->mVecSub.push_back(pSubEntity);
	}
	END_FOR_STL(mVecSub);

	// 拷贝骨骼
	// 浅拷贝先将拷贝实体的骨骼释放掉,然后直接赋值为当前实体的骨骼
	if (pEntity->mCopyType == ECT_SIMPLE)
	{
		TRACE_DELETE(pEntity->mSkeleton);
		// 此处没有设置骨骼所属的实体,认为骨骼还是属于原来的实体
		pEntity->mSkeleton = mSkeleton;
	}
	// 动态模型的浅拷贝或者深度拷贝需要将骨骼深度拷贝一份
	else if (pEntity->mCopyType == ECT_DEEP || pEntity->mCopyType == ECT_ANIM_SIMPLE)
	{
		mSkeleton->CopySkeleton(pEntity->mSkeleton);
	}

	// 如果有动作,则还要拷贝动作信息,里面会判断拷贝类型
	if (mAnimationSet.size() > 0)
	{
		pEntity->mAnimationSet.clear();
		auto iterAnim = mAnimationSet.begin();
		auto iterAnimEnd = mAnimationSet.end();
		FOR_STL(mAnimationSet, ; iterAnim != iterAnimEnd; ++iterAnim)
		{
			pEntity->CopyAnimation(iterAnim->first, iterAnim->second, pEntity->mCopyType);
		}
		END_FOR_STL(mAnimationSet);
	}
}

void txEntity::playClip(const std::string& animName, int beginFrame, int endFrame, const bool& loop, float speed, const float& timeOffset,
	const std::string& hookAnimName, const int& hookBeginFrame, const int& hookEndFrame, const bool& hookAnimLoop, float hookAnimSpeed, const float& hookAnimTimeOffset)
{
	speed = std::abs(speed) * txMath::getSign(endFrame - beginFrame);
	hookAnimSpeed = std::abs(hookAnimSpeed) * txMath::getSign(hookEndFrame - hookBeginFrame);
	auto iter = mAnimationSet.find(animName);
	if (iter == mAnimationSet.end())
	{
		ENGINE_ERROR("error : there is no animation named : %s", animName.c_str());
		return;
	}

	// 切换动作播放时,刷新回调函数列表
	auto iterCallBack = mKeyFrameCallbackList.begin();
	auto iterCallBackEnd = mKeyFrameCallbackList.end();
	FOR_STL(mKeyFrameCallbackList, ; iterCallBack != iterCallBackEnd;)
	{
		bool removed = false;
		// 如果回调函数还存在,则需要先调用回调函数,通知是由于切断动作而造成的回调
		if (iterCallBack->second.mCallback != NULL)
		{
			// 先删除,再调用,避免递归回调,因为在回调函数执行中可能再次会切换动作而引起再次回调
			AnimKeyFrameCallback animCallback = iterCallBack->second.mCallback;
			void* userData = iterCallBack->second.mUserData;
			int aditionalParam = iterCallBack->second.mAditionalParam;

			mKeyFrameCallbackList.erase(iterCallBack++, false);
			removed = true;

			animCallback(userData, aditionalParam, mCurAnim->GetAnimationName(), mName, mCurPlayTime * FRAMES_PER_SECOND, mAnimSpeed, true);
		}
		// 如果当前元素没有被删除,则迭代器自增
		if (!removed)
		{
			++iterCallBack;
		}
	}
	END_FOR_STL(mKeyFrameCallbackList);

	// 无论如何都需要将回调函数清空
	mKeyFrameCallbackList.clear();

	// 在播放动画之前记录下上一个播放的动画
	mLastAnim = mCurAnim;
	mLastBeginFrame = mBeginFrame;
	mLastEndFrame = mEndFrame;
	mLastAnimFarme = mCurPlayTime * FRAMES_PER_SECOND;
	mCurAnimSwitchTime = 0.0f;

	mPlayState = PS_PLAY;
	mCurAnim = iter->second;
	if (beginFrame >= mCurAnim->getLength())
	{
		beginFrame = -1;
	}
	if (endFrame >= mCurAnim->getLength())
	{
		endFrame = -1;
	}
	mAnimSpeed = speed;
	mCurLoop = loop;
	mBeginFrame = beginFrame;
	mEndFrame = endFrame;
	// 计算当前播放时间时需要考虑动作的播放起始偏移时间和起始播放帧
	float beginFrameOffsetTime = mBeginFrame < 0 ? 0.0f : (float)mBeginFrame / FRAMES_PER_SECOND;
	mCurPlayTime = beginFrameOffsetTime + timeOffset;
	// 如果当前动画是循环播放的,则不能挂接动作,因为这样没意义
	if (!mCurLoop && hookAnimName != EMPTY_STRING)
	{
		auto iterHookAnim = mAnimationSet.find(hookAnimName);
		if (iterHookAnim == mAnimationSet.end())
		{
			ENGINE_ERROR("error : there is no animation named : %s", hookAnimName.c_str());
			return;
		}
		mHookAnim = iterHookAnim->second;
		mHookAnimSpeed = hookAnimSpeed;
		mHookAnimLoop = hookAnimLoop;
		mHookBeginFrame = hookBeginFrame;
		mHookEndFrame = hookEndFrame;
	}
	else
	{
		mHookAnim = NULL;
		mHookAnimSpeed = 1.0f;
		mHookAnimLoop = false;
		mHookAnimTimeOffset = hookAnimTimeOffset;
		mHookBeginFrame = -1;
		mHookEndFrame = -1;
	}
}

void txEntity::restart(const float& timeOffset, const float& speed)
{
	// 重新播放时,刷新回调函数列表
	auto iterCallBack = mKeyFrameCallbackList.begin();
	auto iterCallBackEnd = mKeyFrameCallbackList.end();
	FOR_STL(mKeyFrameCallbackList, ; iterCallBack != iterCallBackEnd; ++iterCallBack)
	{
		// 如果该回调函数是只能由该动作才能触发的
		if (iterCallBack->second.mOnlyForCurAnim)
		{
			iterCallBack->second.mAlreadyCallback = false;
		}
	}
	END_FOR_STL(mKeyFrameCallbackList);

	mLastAnim = mCurAnim;
	mLastBeginFrame = mBeginFrame;
	mLastEndFrame = mEndFrame;
	mLastAnimFarme = mCurPlayTime * FRAMES_PER_SECOND;
	mCurAnimSwitchTime = 0.0f;

	mPlayState = PS_PLAY;
	mAnimSpeed = speed;
	// 计算当前播放时间时需要考虑动作的播放起始偏移时间和起始播放帧
	float beginFrameOffsetTime = mBeginFrame < 0 ? 0.0f : (float)mBeginFrame / FRAMES_PER_SECOND;
	mCurPlayTime = beginFrameOffsetTime + timeOffset;

	mHookAnim = NULL;
	mHookAnimSpeed = 1.0f;
	mHookAnimLoop = false;
	mHookAnimTimeOffset = 0.0f;
	mHookBeginFrame = -1;
	mHookEndFrame = -1;
}

void txEntity::stop()
{
	mLastAnim = NULL;
	mCurAnimSwitchTime = ANIM_SWITCH_TIME;
	mPlayState = PS_STOP;
	mCurLoop = false;
	mCurAnim = NULL;
	mAnimSpeed = 1.0f;
	mCurPlayTime = 0.0f;
	mBeginFrame = -1;
	mEndFrame = -1;
	mHookAnim = NULL;
	mHookAnimSpeed = 1.0f;
	mHookAnimLoop = false;
	mHookBeginFrame = -1;
	mHookEndFrame = -1;
}

void txEntity::updateAnim(const float& elapsedTime)
{
	if (mCurAnim == NULL)
	{
		return;
	}

	if (mPlayState == PS_PLAY)
	{
		mCurPlayTime += elapsedTime * mAnimSpeed;
		float playFrame = mCurPlayTime * FRAMES_PER_SECOND;
		// 如果已经播放到了某个关键帧,则调用回调函数
		if (mKeyFrameCallbackList.size() > 0)
		{
			auto iter = mKeyFrameCallbackList.begin();
			auto iterEnd = mKeyFrameCallbackList.end();
			FOR_STL(mKeyFrameCallbackList, ; iter != iterEnd;)
			{
				bool removed = false;
				// 如果该回调函数还没有调用过,并且已经当前播放的帧已经大于指定关键帧,则调用回调函数
				if (!iter->second.mAlreadyCallback && (playFrame >= iter->second.mKeyFrame && mAnimSpeed > 0.0f || playFrame <= iter->second.mKeyFrame && mAnimSpeed < 0.0f))
				{
					// 先标记或者删除,再调用,避免递归回调
					AnimKeyFrameCallback animCallback = iter->second.mCallback;
					void* userData = iter->second.mUserData;
					int aditionalParam = iter->second.mAditionalParam;

					// 标记回调函数已经调用过了
					iter->second.mAlreadyCallback = true;
					// 如果该回调只调用一次,则回调结束后删除该回调
					if (iter->second.mCallbackOnce)
					{
						mKeyFrameCallbackList.erase(iter++, false);
						removed = true;
					}

					animCallback(userData, aditionalParam, mCurAnim->GetAnimationName(), mName, playFrame, mAnimSpeed, false);
				}
				if (!removed)
				{
					++iter;
				}
			}
			END_FOR_STL(mKeyFrameCallbackList);
		}
		// 当前动画已经播放完了
		float endFrame = mEndFrame < 0 ? mCurAnim->getLength() : (float)mEndFrame;
		if (mAnimSpeed > 0.0f && playFrame > endFrame || mAnimSpeed < 0.0f && playFrame < endFrame)
		{
			playFrame = endFrame;
			// 反过来计算当前播放的时间,为了保证其他地方获取到的当前播放时间是正确的
			mCurPlayTime = playFrame / FRAMES_PER_SECOND;
			// 如果循环播放,则再从头开始播放
			if (mCurLoop)
			{
				restart(0.0f, mAnimSpeed);
			}
			// 不循环播放
			else
			{
				// 如果有挂接动画,则播放挂接动画
				if (mHookAnim != NULL)
				{
					playClip(mHookAnim->GetAnimationName(), mHookBeginFrame, mHookEndFrame, mHookAnimLoop, mHookAnimSpeed, mHookAnimTimeOffset);
				}
				// 如果没有挂接动画,则停止播放动画
				else
				{
					stop();
				}
			}
		}
		animate(playFrame);
	}
}

float txEntity::getAnimLength(const std::string& animName)
{
	auto iter = mAnimationSet.find(animName);
	if (iter != mAnimationSet.end())
	{
		iter->second->getLength();
	}
	return 0.0f;
}

void txEntity::setAnimPlayState(const PLAY_STATE& state)
{
	if (state == PS_PLAY)
	{
		play();
	}
	else if (state == PS_PAUSE)
	{
		pause();
	}
	else if (state == PS_STOP)
	{
		stop();
	}
}

bool txEntity::rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint)
{
	VECTOR3 point;
	bool ret = false;
	int subEntityCount = mVecSub.size();
	FOR_STL(mVecSub, int i = 0; i < subEntityCount; ++i)
	{
		if (mVecSub[i]->rayIntersect(ori, dir, point))
		{
			// 只保存距离最近的相交点
			if (!ret || txMath::getLength(point - ori) < txMath::getLength(intersectPoint - ori))
			{
				intersectPoint = point;
			}
			ret = true;
		}
	}
	END_FOR_STL(mVecSub);
	return ret;
}

const txAABox& txEntity::getAABB()
{
	if (mLocalAABB.isEmpty())
	{
		// 动态模型需要将获得3DMax中的包围盒
		if (mSkeleton != NULL && !mSkeleton->IsSkeletonEmpty())
		{
			int subEntityCount = mVecSub.size();
			FOR_STL(mVecSub, int i = 0; i < subEntityCount; ++i)
			{
				txMesh* mesh = mVecSub[i]->GetOriMesh();
				if (mesh != NULL)
				{
					txAABox* subMeshAABB = mesh->get3DMaxAABB();
					mLocalAABB.merge(*subMeshAABB);
				}
			}
			END_FOR_STL(mVecSub);
		}
		else
		{
			int subEntityCount = mVecSub.size();
			FOR_STL(mVecSub, int i = 0; i < subEntityCount; ++i)
			{
				txMesh* mesh = mVecSub[i]->GetOriMesh();
				if (mesh != NULL)
				{
					txAABox* subMeshAABB = mesh->GetAABox();
					mLocalAABB.merge(*subMeshAABB);
				}
			}
			END_FOR_STL(mVecSub);
		}
	}
	return mLocalAABB;
}

const txAABox& txEntity::getWorldAABB()
{
	// 如果没有父节点,则直接返回空包围盒
	if (mParent == NULL)
	{
		mWorldAABB.setEmpty();
	}
	else
	{
		// 如果包围盒为空或者需要重新计算包围盒,则重新计算包围盒
		if (mWorldAABB.isEmpty() || isWorldAABBDirty())
		{
			mWorldAABB = getAABB();
			mWorldAABB = mWorldAABB.transform(mParent->GetFinalMatrix());
		}
	}
	return mWorldAABB;
}