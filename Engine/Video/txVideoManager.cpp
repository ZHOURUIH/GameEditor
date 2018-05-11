#include "txVideoManager.h"
#include "Utility.h"
#include "txEngineRoot.h"
#include "txVideoPlayer.h"

void txVideoManager::init()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	av_register_all();
#endif
}

void txVideoManager::update(float elapsedTime)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	auto iter = mVideoPlayerList.begin();
	auto iterEnd = mVideoPlayerList.end();
	FOR_STL(mVideoPlayerList, ; iter != iterEnd; ++iter)
	{
		iter->second->update(elapsedTime);
	}
	END_FOR_STL(mVideoPlayerList);
#endif
}

void txVideoManager::destroy()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	auto iter = mVideoPlayerList.begin();
	auto iterEnd = mVideoPlayerList.end();
	FOR_STL(mVideoPlayerList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mVideoPlayerList);
	mVideoPlayerList.clear();
#endif
}

txVideoPlayer* txVideoManager::createPlayer(const std::string& name, const std::string& fileName)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	auto iterPlayer = mVideoPlayerList.find(name);
	if (iterPlayer != mVideoPlayerList.end())
	{
		return iterPlayer->second;
	}
	txVideoPlayer* player = TRACE_NEW(txVideoPlayer, player, name, fileName);
	player->init();
	mVideoPlayerList.insert(player->getName(), player);
	return player;
#else
	return NULL;
#endif
}

txVideoPlayer* txVideoManager::getPlayer(const std::string& name)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	auto iterPlayer = mVideoPlayerList.find(name);
	if (iterPlayer != mVideoPlayerList.end())
	{
		return iterPlayer->second;
	}
#endif
	return NULL;
}

void txVideoManager::destroyPlayer(const std::string& name)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	auto iterPlayer = mVideoPlayerList.find(name);
	if (iterPlayer != mVideoPlayerList.end())
	{
		TRACE_DELETE(iterPlayer->second);
		mVideoPlayerList.erase(iterPlayer);
	}
#endif
}
