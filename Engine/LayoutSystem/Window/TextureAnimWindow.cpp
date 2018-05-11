#include "txEngineRoot.h"
#include "txTexture.h"
#include "txTextureManager.h"

#include "WindowHeader.h"
#include "TextureAnimWindowProperty.h"
#include "LayoutUtility.h"
#include "LayoutManager.h"

txSet<std::string> TextureAnimWindow::mTextureAnimNameList;
TextureAnimWindow::TextureAnimWindow(const std::string& name, const WINDOW_TYPE& type)
	:
	TextureWindow(name, type),
	mStartIndex(0),
	mEndIndex(-1),
	mCurTextureIndex(mStartIndex),
	mPlayDirection(true),
	mLoopMode(LM_NONE),
	mInterval(0.03f),
	mCurTimeCount(0.0f),
	mPlayState(PS_STOP),
	mPlayEndCallback(NULL),
	mPlayEndUserData(NULL),
	mAutoHide(true)
{
	if (mTextureAnimNameList.size() == 0)
	{
		refreshTextureAnimNameList();
	}
}

void TextureAnimWindow::update(const float& elapsedTime)
{
	if (!isVisible())
	{
		return;
	}

	if (mPlayState == PS_PLAY)
	{
		if (mTextureCache.size() == 0)
		{
			mTexture = NULL;
		}
		else
		{
			if (mPlayDirection)
			{
				mCurTimeCount += elapsedTime;
				if (mCurTimeCount > mInterval)
				{
					// 一帧时间内可能会跳过多帧序列帧
					int elapsedFrames = (int)(mCurTimeCount / mInterval);
					mCurTimeCount -= elapsedFrames * mInterval;
					if (mCurTextureIndex + elapsedFrames <= getRealEndIndex())
					{
						mCurTextureIndex += elapsedFrames;
					}
					else
					{
						if (mLoopMode == LM_NONE)
						{
							// 非循环播放时播放完成后,自动隐藏时,隐藏窗口,并且停止播放
							if (mAutoHide)
							{
								setVisible(false);
							}
							stop(mAutoHide, true, false);
								
						}
						// 普通循环,则将下标重置到起始下标
						else if (mLoopMode == LM_NORMAL)
						{
							mCurTextureIndex = mStartIndex;
						}
						// 来回循环,则将下标重置到终止下标,并且开始反向播放
						else if (mLoopMode == LM_PINGPONG)
						{
							mCurTextureIndex = getRealEndIndex();
							mPlayDirection = !mPlayDirection;
						}
					}
					setTexture(mTextureCache[mCurTextureIndex], true);
				}
			}
			else
			{
				mCurTimeCount += elapsedTime;
				if (mCurTimeCount > mInterval)
				{
					// 一帧时间内可能会跳过多帧序列帧
					int elapsedFrames = (int)(mCurTimeCount / mInterval);
					mCurTimeCount -= elapsedFrames * mInterval;
					if (mCurTextureIndex - elapsedFrames >= mStartIndex)
					{
						mCurTextureIndex -= elapsedFrames;
					}
					else
					{
						if (mLoopMode == LM_NONE)
						{
							// 非循环播放时播放完成后,自动隐藏时,隐藏窗口,并且停止播放
							if (mAutoHide)
							{
								setVisible(false);
							}
							stop(mAutoHide, true, false);
						}
						// 普通循环,则将下标重置到终止下标
						else if (mLoopMode == LM_NORMAL)
						{
							mCurTextureIndex = getRealEndIndex();
						}
						// 来回循环,则将下标重置到起始下标,并且开始正向播放
						else if (mLoopMode == LM_PINGPONG)
						{
							mCurTextureIndex = mStartIndex;
							mPlayDirection = !mPlayDirection;
						}
					}
					setTexture(mTextureCache[mCurTextureIndex], true);
				}
			}
		}
	}
	TextureWindow::update(elapsedTime);
}

void TextureAnimWindow::setTextureSet(const std::string& textureSetName, const bool& async)
{
	mTextureCache.clear();
	mTextureSet = textureSetName;
	std::string path = P_UI_TEXTURE_ANIM_PATH + mTextureSet + "/";
	// 一直循环搜索序列帧,直到找不到为止
	for (int i = 0; ; ++i)
	{
		std::string name = mTextureSet + "_" + txStringUtility::intToString(i + 1) + ".swi";
		std::string fullPath = path + name;
		txTexture* tex = mTextureManager->createTexture(fullPath);
		if (tex != NULL)
		{
			if (txFileUtility::isFileExist(fullPath))
			{
				// 只有非异步加载时才创建纹理
				tex->initTexture(!async);
				mTextureCache.push_back(tex);
			}
			// 如果纹理文件不存在,则表示该序列帧就只有这么多纹理帧,退出循环
			else
			{
				mTextureManager->destroyTexture(tex->getName());
				break;
			}
		}
		else
		{
			ENGINE_ERROR("error : create texture failed! name : %s", name.c_str());
			break;
		}
	}

	// 重新判断起始下标和终止下标,确保下标不会越界
	txMath::clamp(mStartIndex, 0, getTextureFrameCount() - 1);
	if (mEndIndex >= 0)
	{
		txMath::clamp(mEndIndex, 0, getTextureFrameCount() - 1);
	}
}

void TextureAnimWindow::initProperty()
{
	TextureWindow::initProperty();
	addProperty<TextureAnimWindowPropertyTextureAnimSet>();
	addProperty<TextureAnimWindowPropertyFrameCount>();
	addProperty<TextureAnimWindowPropertyInterval>();
	addProperty<TextureAnimWindowPropertyLoopMode>();
	addProperty<TextureAnimWindowPropertyPlayState>();
	addProperty<TextureAnimWindowPropertyStartIndex>();
	addProperty<TextureAnimWindowPropertyEndIndex>();

	// 初始化序列帧候选列表
	TextureAnimWindowPropertyTextureAnimSet* animProperty = getProperty<TextureAnimWindowPropertyTextureAnimSet>();
	if (animProperty->getChoices() != NULL && animProperty->getChoices()->size() == 0)
	{
		txVector<std::string> animSet;
		auto iterAnim = mTextureAnimNameList.begin();
		auto iterAnimEnd = mTextureAnimNameList.end();
		FOR_STL(mTextureAnimNameList, ; iterAnim != iterAnimEnd; ++iterAnim)
		{
			animSet.push_back(*iterAnim);
		}
		END_FOR_STL(mTextureAnimNameList);
		animProperty->setChoices(animSet);
	}
}

void TextureAnimWindow::finishAsycLoad()
{
	int textureCount = mTextureCache.size();
	FOR_STL(mTextureCache, int i = 0; i < textureCount; ++i)
	{
		if (mTextureCache[i] != NULL)
		{
			mTextureCache[i]->loadGLTexture();
		}
	}
	END_FOR_STL(mTextureCache);
}

void TextureAnimWindow::getUsedResourceList(txVector<std::string>& resourceList)
{
	int textureCount = mTextureCache.size();
	FOR_STL(mTextureCache, int i = 0; i < textureCount; ++i)
	{
		if (mTextureCache[i] != NULL)
		{
			resourceList.push_back(mTextureCache[i]->getName());
		}
	}
	END_FOR_STL(mTextureCache);
}

void TextureAnimWindow::setPlayState(const PLAY_STATE& state)
{
	if (state == PS_PAUSE)
	{
		pause();
	}
	else if (state == PS_PLAY)
	{
		play();
	}
	else if (state == PS_STOP)
	{
		stop();
	}
}

void TextureAnimWindow::refreshTextureAnimNameList()
{
	mTextureAnimNameList.clear();
	txVector<std::string> fileList;
	txFileUtility::findFolders(P_UI_TEXTURE_ANIM_PATH, fileList);
	int fileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < fileCount; ++i)
	{
		std::string fileName = txStringUtility::getFileName(fileList[i]);
		mTextureAnimNameList.insert(fileName);
	}
	END_FOR_STL(fileList);
}