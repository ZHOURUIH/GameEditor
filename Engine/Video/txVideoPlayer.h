#ifndef _TX_VIDEO_PLAYER_H_
#define _TX_VIDEO_PLAYER_H_

#include "txEngineDefine.h"

#if RUN_PLATFORM == PLATFORM_WINDOWS
#ifdef __cplusplus
extern "C"
{
#define inline _inline
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/mem.h"
#include "libavutil/imgutils.h"
}
#endif
#endif

#include "txEngineBase.h"
#include "ThreadLock.h"

class txTexture;
class txTextureManager;
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVFrame;
struct SwsContext;

struct DecodedFrame
{
	txTexture* mTexture;
	char* mTextureBuffer;
	int mWidth;
	int mHeight;
	int mBytesPerPixel;
	DecodedFrame()
	{
		mTexture = NULL;
		mTextureBuffer = NULL;
		mWidth = 0;
		mHeight = 0;
		mBytesPerPixel = 0;
	}
};

// 该视频播放器只能从前往后解码播放,不能倒序解码播放
class txVideoPlayer : public txEngineBase
{
public:
	txVideoPlayer(const std::string& name, const std::string& fileName);
	virtual ~txVideoPlayer(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime){}
	void destroy();
	txTexture* getFrame(const int& frame);
	void destroyFrame(const int& frame);
	void askDecodeFrame(const int& frame);
	const std::string& getName() { return mName; }
	const std::string& getFileName() { return mFileName; }
	VECTOR2 getVideoSize() { return VECTOR2((float)mVideoWidth, (float)mVideoHeight); }
	const float& getVideoLength(){ return mVideoLength; }
	const bool& getVideoEnd() { return mVideoEnd; }
	const float& getFramesPerSecond() { return mFramesPerSecond; }
protected:
	bool decode(const int& frame, DecodedFrame* frameData);
#if RUN_PLATFORM == PLATFORM_ANDROID
	static void* decodeFrame(void* arg);
#elif RUN_PLATFORM == PLATFORM_WINDOWS
	static DWORD WINAPI decodeFrame(LPVOID lpParameter);
#endif
protected:
	AVFormatContext* mFormatContext;
	AVCodecContext* mCodecContext;
	AVCodec* mCodec;
	AVFrame* mFrame;
	AVFrame* mFrameRGBA;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	PixelFormat mConvertTargetFormat;
	AVPacket mPacket;
#endif
	SwsContext* mImgConvertContext;
	int mVideoStreamIndex;	// 视频流的下标
	std::string mName;		// 名字
	std::string mFileName;	// 文件名,带media下的相对路径
	int mBytesPerPixel;		// 每个像素的字节数量
	uint8_t* mFrameBuffer;	// ffmpeg解码用的缓冲区
	int mVideoWidth;		// 视频帧宽
	int mVideoHeight;		// 视频帧高
	bool mVideoEnd;			// 视频是否已经解码完毕
	int mLastFrame;			// 上一次取的视频帧,用于解码时找到需要的帧
	float mFramesPerSecond;	// 视频每秒的帧数
	float mVideoLength;		// 视频长度,秒
	volatile bool mRunDecodeThread;	// 运行解码线程标记
	txMap<int, txTexture*> mDecodeFrames;// first是视频帧下标,second的first是视频帧纹理,是已经解码完成的
	txMap<int, DecodedFrame*> mDoneFrames;// 完成解码的帧
	txSet<int> mAddFrameSet;			// 请求解码的帧
	txSet<int> mDestroyFrameSet;		// 请求销毁的帧
	ThreadLock mFrameLock;				// mDecodeFrames的线程锁
	ThreadLock mDoneLock;				// mDoneFrameSet的线程锁
	ThreadLock mAddLock;				// mAddFrameSet的线程锁
	ThreadLock mDestroyLock;			// mRemoveFrameSet的线程锁
	int mLastDecodeIndex;				// 上一次解码的帧,用于查找当前需要解码的帧
#if RUN_PLATFORM == PLATFORM_WINDOWS
	volatile std::atomic<HANDLE> mDecodeThread;
#endif
};

#endif