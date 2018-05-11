#include "txTexture.h"
#include "txRendererManager.h"

#include "WindowHeader.h"
#include "TextureWindowProperty.h"
#include "LayoutUtility.h"

void TextureWindow::render()
{
	if (NULL != mTexture)
	{
		renderTexture(mTexture, mMaskTexture, mTextureCoord, mTextureSize, mArcProgress * 2 * txMath::MATH_PI, mLinearDodge, mUseMipmap);
	}
	Window::render();
}

void TextureWindow::initProperty()
{
	Window::initProperty();
	addProperty<TextureWindowPropertyTexture>();
	addProperty<TextureWindowPropertyMaskTexture>();
	addProperty<TextureWindowPropertyAutoSize>();
	addProperty<TextureWindowPropertyTextureCoord>();
	addProperty<TextureWindowPropertyTextureSize>();
	addProperty<TextureWindowPropertyArcProgress>();
	addProperty<TextureWindowPropertyLinearDodge>();
}

void TextureWindow::setAutoSize(const bool& autoSize)
{ 
	mAutoSize = autoSize;
	if (mAutoSize && mTexture != NULL)
	{
		mTextureSize = mTexture->getTextureSize();
	}
	setTexCoordsDirty(true);
}

void TextureWindow::setTexture(txTexture* texture, const bool& refreshNow)
{
	if (mTexture == texture)
	{
		return;
	}
	mTexture = texture;
	// 自动获得纹理大小
	if (mAutoSize && mTexture != NULL)
	{
		mTextureSize = mTexture->getTextureSize();
	}
	// 如果立即刷新,则重新计算纹理坐标
	if (refreshNow)
	{
		notifyTexCoordNeedUpdated();
	}
	// 否则只标记需要刷新纹理坐标
	else
	{
		setTexCoordsDirty(true);
	}
	selfTextureChanged();
}

void TextureWindow::finishAsycLoad()
{
	if (mTexture != NULL)
	{
		mTexture->loadGLTexture();
	}
}

void TextureWindow::getUsedResourceList(txVector<std::string>& resourceList)
{
	if (mTexture != NULL)
	{
		resourceList.push_back(mTexture->getName());
	}
}

void TextureWindow::selfTextureSizeChanged()
{
	if (mParentComplexWindow != NULL)
	{
		mParentComplexWindow->notifyChildTextureSizeChanged(this);
	}
}

void TextureWindow::selfTextureChanged()
{
	if (mParentComplexWindow != NULL)
	{
		mParentComplexWindow->notifyChildTextureChanged(this);
	}
}

void TextureWindow::notifyTexCoordNeedUpdated()
{
	LayoutUtility::generateTexCoord(mTexture, mTexCoords, mSymmetryMode, mTextureCoord, mTextureSize);
}