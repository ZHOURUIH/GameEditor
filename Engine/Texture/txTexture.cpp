#include "txEngineLog.h"
#include "txTexture.h"
#include "txEngineRoot.h"
#include "txRendererInstance.h"
#include "txSerializer.h"
#include "Utility.h"

txTexture::txTexture(const std::string& name, const bool& backupPixelData)
:
mTex(0),
mAlphaTex(0),
mPixelData(NULL),
mFileData(NULL),
mAlphaFileData(NULL),
mNameWithPath(name),
mBackupPixelData(backupPixelData)
{
	int dotPos = mNameWithPath.find_last_of('.');
	if (dotPos != -1)
	{
		std::string preName = mNameWithPath.substr(0, dotPos);
		std::string lastName = mNameWithPath.substr(dotPos, mNameWithPath.length() - dotPos);
		mAlphaTexPath = preName + "_alpha" + lastName;
	}	
}

txTexture::~txTexture()
{
	deleteGLTexture();
	deleteFileData();
	TRACE_DELETE_ARRAY(mPixelData);
}

void txTexture::createCustomTexture(char* data, const int& width, const int& height, const bool& useMipmap)
{
	if (mTex > 0)
	{
		return;
	}
	mTextureSize.x = (float)width;
	mTextureSize.y = (float)height;
	createGLTexture(data, width, height, NULL, 4, useMipmap);
}

void txTexture::createCustomTexture(char* data, const int& width, const int& height, const int& byteCount, const bool& useMipmap)
{
	if (mTex > 0)
	{
		return;
	}
	mTextureSize.x = (float)width;
	mTextureSize.y = (float)height;
	createGLTexture(data, width, height, NULL, byteCount, useMipmap);
}

bool txTexture::initTexture(const bool& createGL)
{
	// 如果已经创建了,则直接返回true,表示可以正常使用该贴图
	if(mTex > 0)
	{
		return true;
	}
	if (mNameWithPath == EMPTY_STRING)
	{
		ENGINE_ERROR("error : texture path is empty!");
		return false;
	}
	mFileData = txFileUtility::openBinaryFile(mNameWithPath, NULL);
	if (mFileData == NULL)
	{
		// 此处不能使用错误信息宏,因为需要允许某些图片不存在的情况
		LOGI("can not load texture : %s", mNameWithPath.c_str());
		return false;
	}
	unsigned short width, height;
	memcpy((void*)&width, mFileData + 4, sizeof(unsigned short));
	memcpy((void*)&height, mFileData + 6, sizeof(unsigned short));
	mTextureSize.x = width;
	mTextureSize.y = height;
	
	// 查看是否存在透明图片
	if (txFileUtility::isFileExist(mAlphaTexPath))
	{
		mAlphaFileData = txFileUtility::openBinaryFile(mAlphaTexPath, NULL);
	}
	if (createGL)
	{
		loadGLTexture();
	}
	return true;
}

bool txTexture::loadGLTexture(const bool& useMipmap)
{
	if (mFileData == NULL)
	{
		return false;
	}
	char* alphaData = NULL;
	if (mAlphaFileData != NULL)
	{
		alphaData = mAlphaFileData + 8;
	}
	createGLTexture(mFileData + 8, (int)mTextureSize.x, (int)mTextureSize.y, alphaData, 4, useMipmap);
	deleteFileData();
	return true;
}

void txTexture::createGLTexture(char* data, const int& width, const int& height, char* alphaElem, const int& byteCount, const bool& mipmap)
{
	if(mBackupPixelData)
	{
		TRACE_DELETE_ARRAY(mPixelData);
		mPixelData = TRACE_NEW_ARRAY(char, width * height * byteCount, mPixelData);
		memcpy(mPixelData, data, width * height * byteCount);
	}
	deleteGLTexture();
	glActiveTexture(GL_TEXTURE0 + 0);
	glGenTextures(1, &mTex);
	txRendererInstance::checkGlError("glGenTextures");
	glBindTexture(GL_TEXTURE_2D, mTex);

	if (byteCount == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (byteCount == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	if (alphaElem != NULL)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glGenTextures(1, &mAlphaTex);
		glBindTexture(GL_TEXTURE_2D, mAlphaTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, alphaElem);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void txTexture::saveToFile(char* data, const int& width, const int& height, const int& byteCount)
{
	txSerializer serializer;
	serializer.writeString("SWI ");
	serializer.write((unsigned short)width);
	serializer.write((unsigned short)height);
	serializer.writeBuffer(data, width * height * byteCount);
	serializer.writeToFile(mNameWithPath);
}

void txTexture::deleteGLTexture()
{
	if (mTex > 0)
	{
		glDeleteTextures(1, &mTex);
		mTex = 0;
	}
	if (mAlphaTex > 0)
	{
		glDeleteTextures(1, &mAlphaTex);
		mAlphaTex = 0;
	}
}

void txTexture::deleteFileData()
{
	TRACE_DELETE_ARRAY(mFileData);
	TRACE_DELETE_ARRAY(mAlphaFileData);
}