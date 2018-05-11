#ifndef _TX_TEXTURE_MANAGER_H_
#define _TX_TEXTURE_MANAGER_H_

#include "txEngineDefine.h"
#include "ThreadLock.h"

// 纹理管理器,所有的纹理都必须由该管理器来创建和销毁
class txTexture;
class txTextureManager
{
public:
	txTextureManager();
	virtual ~txTextureManager();
	virtual void init(){}
	virtual void update(float elapsedTime);
	txTexture* createTexture(const std::string& name, const bool& getIfExist = true, const bool& backupPixel = false);
	txTexture* createTexture(const std::string& name, char* data, const int& width, const int& height, const int& byteCount, const bool& getIfExist = true, const bool& useMipmap = true, const bool& backupPixel = false);
	bool destroyTexture(const std::string& name);
	txTexture* getTexture(const std::string& name);
	void destroyAllTextures();
	void dump();
protected:
	void destroyLaterList();
protected:
	txMap<std::string, txTexture*> mTextureList;
    // 需要延迟销毁的纹理列表，因为某些纹理太大，可能无法在销毁时真正地释放掉内存，所以需要延迟销毁
    txVector<txTexture*> mDeleteLaterList;
	ThreadLock mDeleteListLock;
	ThreadLock mTextureListLock;
};

#endif