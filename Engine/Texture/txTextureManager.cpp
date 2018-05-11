#include "txTextureManager.h"
#include "txTexture.h"
#include "txEngineLog.h"
#include "Utility.h"
#include "txEngineRoot.h"

txTextureManager::txTextureManager()
{
	mTextureList.clear();
}

txTextureManager::~txTextureManager()
{
	dump();
	destroyAllTextures();
}

void txTextureManager::update(float elapsedTime)
{
	destroyLaterList();
}

txTexture* txTextureManager::createTexture(const std::string& name, const bool& getIfExist, const bool& backupPixel)
{
	LOCK(mTextureListLock);
	if(mTextureList.find(name) != mTextureList.end())
	{
        if(getIfExist)
        {
			txTexture* texture = mTextureList.find(name)->second;
			UNLOCK(mTextureListLock);
			return texture;
        }
        else
        {
			UNLOCK(mTextureListLock);
			ENGINE_ERROR("error : texture named %s have already exist!", name.c_str());
            return NULL;
        }
    }
	txTexture* pTexture = TRACE_NEW(txTexture, pTexture, name, backupPixel);
	mTextureList.insert(name, pTexture);
	UNLOCK(mTextureListLock);
	return pTexture;
}

txTexture* txTextureManager::createTexture(const std::string& name, char* data, const int& width, const int& height, const int& byteCount, const bool& getIfExist, const bool& useMipmap, const bool& backupPixel)
{
	LOCK(mTextureListLock);
	if (mTextureList.find(name) != mTextureList.end())
	{
		if (getIfExist)
		{
			txTexture* texture = mTextureList.find(name)->second;
			UNLOCK(mTextureListLock);
			return texture;
		}
		else
		{
			UNLOCK(mTextureListLock);
			ENGINE_ERROR("error : texture named %s have already exist!", name.c_str());
			return NULL;
		}
	}
	txTexture* pTexture = TRACE_NEW(txTexture, pTexture, name, backupPixel);
	pTexture->createCustomTexture(data, width, height, byteCount, useMipmap);
	mTextureList.insert(name, pTexture);
	UNLOCK(mTextureListLock);
	return pTexture;
}

bool txTextureManager::destroyTexture(const std::string& name)
{
	LOCK(mTextureListLock);
	auto iter = mTextureList.find(name);
	if(iter == mTextureList.end())
	{
		UNLOCK(mTextureListLock);
		return false;
	}
	txTexture* pTexture = iter->second;
	mTextureList.erase(iter);
	UNLOCK(mTextureListLock);

	LOCK(mDeleteListLock);
	// 如果纹理大于MIN_DELETE_DELAY_SIZE MB，则放入延迟销毁列表中，在场景的第一次循环时将延迟销毁列表中的纹理销毁掉
	if (pTexture->getTextureSize().x * pTexture->getTextureSize().y * 4 >= 1024 * 1024 * MIN_DELETE_DELAY_SIZE)
    {
        mDeleteLaterList.push_back(pTexture);
    }
    else
    {
		TRACE_DELETE(pTexture);
    }
	UNLOCK(mDeleteListLock);
	return true;
}

void txTextureManager::destroyLaterList()
{
	LOCK(mDeleteListLock);
    if(mDeleteLaterList.size() == 0)
    {
		UNLOCK(mDeleteListLock);
        return;
    }
    // 本次已经释放的数量,调用一次函数最多只能释放MAX_DELETE_COUNT个纹理，不能释放多个，否则可能会造成无法释放内存的错误
    int deletedCount = 0;
	auto iter = mDeleteLaterList.begin();
	FOR_STL(mDeleteLaterList, ; iter != mDeleteLaterList.end() && deletedCount < MAX_DELETE_COUNT; ++deletedCount)
    {
		TRACE_DELETE(*iter);
        iter = mDeleteLaterList.erase(iter, false);
    }
	END_FOR_STL(mDeleteLaterList);
	UNLOCK(mDeleteListLock);
}

txTexture* txTextureManager::getTexture(const std::string& name)
{
	LOCK(mTextureListLock);
	auto iter = mTextureList.find(name);
	if(iter == mTextureList.end())
	{
		UNLOCK(mTextureListLock);
		ENGINE_ERROR("error : can not find texture : %s", name.c_str());
		return NULL;
	}
	txTexture* texture = iter->second;
	UNLOCK(mTextureListLock);
	return texture;
}

void txTextureManager::destroyAllTextures()
{
	LOCK(mTextureListLock);
	auto iter = mTextureList.begin();
	auto iterEnd = mTextureList.end();
	FOR_STL(mTextureList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mTextureList);
	mTextureList.clear();
	UNLOCK(mTextureListLock);
}

void txTextureManager::dump()
{
	LOCK(mTextureListLock);
	LOGI("---------------texture dump begin, %d textures exist----------------", (int)mTextureList.size());
	auto iter = mTextureList.begin();
	auto iterEnd = mTextureList.end();
	FOR_STL(mTextureList, ; iter != iterEnd; ++iter)
	{
		LOGI("texture dump : texture name : %s", iter->first.c_str());
	}
	END_FOR_STL(mTextureList);
	LOGI("---------------texture dump end-------------------------------------");
	UNLOCK(mTextureListLock);
}
