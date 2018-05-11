#ifndef _TX_VIDEO_MANAGER_H_
#define _TX_VIDEO_MANAGER_H_

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

class txVideoPlayer;

class txVideoManager
{
public:
	txVideoManager(){}
	virtual ~txVideoManager(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime);
	void destroy();
	txVideoPlayer* createPlayer(const std::string& name, const std::string& fileName);
	txVideoPlayer* getPlayer(const std::string& name);
	void destroyPlayer(const std::string& name);
protected:
	txMap<std::string, txVideoPlayer*> mVideoPlayerList;
};

#endif