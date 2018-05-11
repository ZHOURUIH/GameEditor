#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txTextureManager.h"
#include "txVideoPlayer.h"
#include "txVideoManager.h"
#include "txTexture.h"

#include "WindowHeader.h"
#include "VideoWindowProperty.h"

VideoWindow::VideoWindow(const std::string& name, const WINDOW_TYPE& type)
	:
	TextureWindow(name, type),
	mVideoPlayer(NULL),
	mVideoTime(0.0f),
	mRate(1.0f),
	mVideoState(PS_STOP),
	mLoop(false),
	mCurFrame(-1),
	mLastFrame(-1),
	mEndCallback(NULL),
	mEndUserData(NULL)
{
	// 视频窗口不使用多级渐进纹理,提高解码和渲染速度
	mUseMipmap = false;
}
	
VideoWindow::~VideoWindow()
{
	destroy();
}

void VideoWindow::update(const float& elapsedTime)
{
	if (!isVisible())
	{
		return;
	}

	Window::update(elapsedTime);
	if (mVideoPlayer != NULL)
	{
		if (mVideoState == PS_PLAY)
		{
			// 累计视频实际播放的时间
			mVideoTime += elapsedTime * mRate;
			// 当前帧已经渲染完,则开始下一帧的解码和渲染
			if (mCurFrame == -1)
			{
				// 计算视频当前帧
				int frame = (int)(mVideoTime * mVideoPlayer->getFramesPerSecond());
				// 计算出来的帧与上一帧不同
				if (mLastFrame != frame)
				{
					mCurFrame = frame;
					mVideoPlayer->askDecodeFrame(mCurFrame);
				}
			}
				
			// 查询当前帧是否已经解码完成
			if (mCurFrame >= 0)
			{
				txTexture* frameTexture = mVideoPlayer->getFrame(mCurFrame);
				// 如果已经解码完成,则立即设置当前渲染纹理
				if (frameTexture != NULL)
				{
					// 先销毁上一帧
					if (mLastFrame >= 0)
					{
						mVideoPlayer->destroyFrame(mLastFrame);
					}
					// 由于是在窗口的update中设置纹理,需要立即刷新
					setTexture(frameTexture, true);
					mLastFrame = mCurFrame;
					mCurFrame = -1;
				}
			}

			if (mVideoPlayer->getVideoEnd())
			{
				// 销毁上一帧
				if (mLastFrame >= 0)
				{
					mVideoPlayer->destroyFrame(mLastFrame);
				}
				setPlayState(PS_STOP, false);
				if (mLoop)
				{
					setPlayState(PS_PLAY);
				}
				// 非循环播放时,调用播放结束的回调函数
				else
				{
					callAndClearEndCallback(false);
				}
				// 程序暂停一段时间,避免视频在重新播放时有一帧黑的,但是只能在正常播放速度下可以有效避免
				txUtility::sleep(30);
			}
		}
	}
	else
	{
		// 设置为空纹理时,可以不立即刷新
		setTexture(NULL);
	}
}

void VideoWindow::destroy()
{
	destroyPlayer();
}

void VideoWindow::setPlayState(const PLAY_STATE& state, const bool& callback)
{
	if (mVideoState == state)
	{
		return;
	}
	mVideoState = state;
	if (state == PS_PLAY)
	{
		// 如果还未创建播放器,则先创建播放器,因为可能是从暂停状态恢复到播放状态
		if (mVideoPlayer == NULL)
		{
			createPlayer();
			// 从停止状态开始播放视频时,预先解码一帧
			mCurFrame = 0;
			mVideoPlayer->askDecodeFrame(mCurFrame);
		}
	}
	else if (state == PS_PAUSE)
	{
		;
	}
	else if (state == PS_STOP)
	{
		// 停止播放时销毁播放器
		mVideoTime = 0.0f;
		mLastFrame = -1;
		mCurFrame = -1;
		destroyPlayer();
		if (callback)
		{
			callAndClearEndCallback(true);
		}
	}
}

void VideoWindow::createPlayer()
{
	if (mVideoPlayer == NULL && mFileName != EMPTY_STRING)
	{
		mVideoPlayer = mVideoManager->createPlayer(mName, P_VIDEO_PATH + mFileName);
	}
}

void VideoWindow::destroyPlayer()
{
	if (mVideoPlayer != NULL)
	{
		mVideoManager->destroyPlayer(mVideoPlayer->getName());
		mVideoPlayer = NULL;
	}
}

float VideoWindow::getVideoLength()
{
	if (mVideoPlayer == NULL)
	{
		return 0.0f;
	}
	return mVideoPlayer->getVideoLength();
}

float VideoWindow::getVideoFPS()
{
	if (mVideoPlayer == NULL)
	{
		return 0.0f;
	}
	return mVideoPlayer->getFramesPerSecond();
}

VECTOR2 VideoWindow::getVideoSize()
{
	if (mVideoPlayer == NULL)
	{
		return txMath::VEC2_ZERO;
	}
	return mVideoPlayer->getVideoSize();
}

void VideoWindow::initProperty()
{
	TextureWindow::initProperty(); 
	addProperty<VideoWindowPropertyVideo>();
	addProperty<VideoWindowPropertyPlayState>();
	addProperty<VideoWindowPropertyVideoLoop>();
	addProperty<VideoWindowPropertyRate>();
	addProperty<VideoWindowPropertyVideoLength>();
	addProperty<VideoWindowPropertyPlayPercent>();
	addProperty<VideoWindowPropertyPlayTime>();
	addProperty<VideoWindowPropertyVideoSize>();
	addProperty<VideoWindowPropertyVideoFPS>();

	// 初始化序列帧候选列表
	VideoWindowPropertyVideo* videoProperty = getProperty<VideoWindowPropertyVideo>();
	if (videoProperty->getChoices() != NULL && videoProperty->getChoices()->size() == 0)
	{
		txVector<std::string> fileList;
		txVector<std::string> patterns;
		patterns.push_back(".mp4");
		patterns.push_back(".avi");
		patterns.push_back(".rmvb");
		txFileUtility::findFiles(P_VIDEO_PATH, fileList, patterns, false);
		txVector<std::string> videoList;
		int fileCount = fileList.size();
		FOR_STL(fileList, int i = 0; i < fileCount; ++i)
		{
			videoList.push_back(txStringUtility::getFileName(fileList[i]));
		}
		END_FOR_STL(fileList);
		videoProperty->setChoices(videoList);
	}
}