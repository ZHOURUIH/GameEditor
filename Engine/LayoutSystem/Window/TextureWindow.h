#ifndef _TEXTURE_WINDOW_H_
#define _TEXTURE_WINDOW_H_

#include "Window.h"

class txTexture;
class TextureWindow : public Window
{
public:
	TextureWindow(const std::string& name, const WINDOW_TYPE& type)
		:
		Window(name, type),
		mTexture(NULL),
		mMaskTexture(NULL),
		mTextureCoord(txMath::VEC2_ZERO),
		mTextureSize(txMath::VEC2_ONE),
		mAutoSize(true),
		mArcProgress(1.0f),
		mLinearDodge(false),
		mUseMipmap(true)
	{}
	virtual ~TextureWindow(){}
	virtual void render();
		
	txTexture* getTexture()						{ return mTexture; }
	txTexture* getMaskTexture()					{ return mMaskTexture; }
	const VECTOR2& getTextureCoord()			{ return mTextureCoord; }
	const VECTOR2& getTextureSize()				{ return mTextureSize; }
	const bool& getAutoSize()					{ return mAutoSize; }
	const bool& getLinearDodge()				{ return mLinearDodge; }
	const float& getArcProgress()				{ return mArcProgress; }

	void setArcProgress(const float& progress)	{ mArcProgress = progress; }
	void setMaskTexture(txTexture* texture)		{ mMaskTexture = texture; }
	void setLinearDodge(const bool& linearDodge){ mLinearDodge = linearDodge; }
	void setTextureCoord(const VECTOR2& textureCoord, const bool& refreshNow)
	{ 
		// 只在非自动获取纹理大小时才有效
		if (!mAutoSize)
		{
			mTextureCoord = textureCoord;
			if (refreshNow)
			{
				notifyTexCoordNeedUpdated();
			}
			else
			{
				setTexCoordsDirty(true);
			}
		}
	}
	void setTextureSize(const VECTOR2& textureSize, const bool& refreshNow)
	{
		// 只在非自动获取纹理大小时才有效
		if (!mAutoSize)
		{
			mTextureSize = textureSize;
			if (refreshNow)
			{
				notifyTexCoordNeedUpdated();
			}
			else
			{
				setTexCoordsDirty(true);
			}
			selfTextureSizeChanged();
		}
	}
	void setAutoSize(const bool& autoSize);
	// refreshNow表示是否立即刷新纹理坐标,在窗口的update中调用该函数时需要设置为true
	void setTexture(txTexture* texture, const bool& refreshNow = false);
	virtual void finishAsycLoad();
	virtual void getUsedResourceList(txVector<std::string>& resourceList);
	virtual void notifyTexCoordNeedUpdated();
protected:
	virtual void initProperty();
	void selfTextureSizeChanged();
	void selfTextureChanged();
protected:
	txTexture* mTexture;			// 渲染的纹理
	txTexture* mMaskTexture;		// 透明遮挡纹理,遮挡纹理的大小需要跟渲染纹理的大小一致,如果不一致,则遮挡纹理会被拉伸
	VECTOR2 mTextureCoord;			// 需要渲染的纹理起始坐标
	VECTOR2 mTextureSize;			// 需要渲染的纹理尺寸
	bool mAutoSize;					// 是否自动获取图片的宽高
	float mArcProgress;				// 扇形剪切的角度百分比
	bool mLinearDodge;				// 是否使用线性减淡叠加方式
	bool mUseMipmap;				// 是否使用多级渐进纹理
};

#endif