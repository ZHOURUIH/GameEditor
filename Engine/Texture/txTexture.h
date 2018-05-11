#ifndef _TX_TEXTURE_H_
#define _TX_TEXTURE_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"
#include "txIPoint.h"

class txDataElem;

class txTexture : public txEngineBase
{
public:
	txTexture(const std::string& name, const bool& backupPixelData = false);
	virtual ~txTexture();
	const std::string& getName() { return mNameWithPath; }
	//初始化纹理
	bool initTexture(const bool& createGL = true);
	bool loadGLTexture(const bool& useMipmap = true);		// 该函数不能再子线程中调用
	void saveToFile(char* data, const int& width, const int& height, const int& byteCount);
	//自定义纹理
	void createCustomTexture(char* data, const int& width, const int& height, const bool& useMipmap);
	void createCustomTexture(char* data, const int& width, const int& height, const int& byteCount, const bool& useMipmap);
	void deleteGLTexture();
	void deleteFileData();
	// 得到纹理的颜色数据
	char* GetTextureData(){ return mPixelData; }
	const VECTOR2& getTextureSize() { return mTextureSize; }
	//返回0表示没有使用
	const GLuint& getTextureID(){ return mTex; }
	const GLuint& getAlphaTextureID() { return mAlphaTex; }
protected:
	void createGLTexture(char* data, const int& width, const int& height, char* alphaElem, const int& byteCount, const bool& mipmap);
protected:
	std::string mNameWithPath;	// 带相对路径的文件名,也就是纹理的名字
	std::string mAlphaTexPath;	// 带相对路径的透明纹理的文件名,是media下的路径
	VECTOR2 mTextureSize;		// 图片大小
	GLuint mTex;				// 纹理句柄
	GLuint mAlphaTex;			// 用于采集透明通道的纹理,因为透明通道在经过mipmap计算后会出现问题,所以另外使用一张图片来对透明度进行采样
	char* mPixelData;			// 备份的像素数据,只有颜色数据,只有在构造时通知图片需要保存颜色数据后,才能获取颜色数据,否则获取不到颜色数据
	bool mBackupPixelData;		// 是否在销毁文件之前备份像素数据
	char* mFileData;
	char* mAlphaFileData;
};

#endif