#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txPoint.h"

#include "WindowHeader.h"
#include "WindowComponentSelffitScale.h"

void WindowComponentSelffitScale::update(float elapsed)
{
	float scale = mRenderWindow->getScreenDensity();
	// 如果密度为1,则不再计算
	if (txMath::isFloatEqual(scale, 1.0f))
	{
		return;
	}
	Window* window = static_cast<Window*>(mComponentOwner);
	VECTOR2 localPixelPosition = window->getLocalPixelPosition();
	const VECTOR2& pixelSize = window->getPixelSize();
	const txPoint& windowPosition = window->getPosition();
	Window* parent = window->getParent();
	VECTOR2 parentSize;
	VECTOR2 parentFinalSize;
	if (parent != NULL)
	{
		parentSize = parent->getPixelSize();
		parentFinalSize = parent->getFinalSize();
	}
	else
	{
		parentSize = VECTOR2(mRenderWindow->getWidth(), mRenderWindow->getHeight());
		parentFinalSize = parentSize;
	}

	// 如果该窗口本来就是铺满父窗口的,则直接赋值为父窗口的最终大小
	VECTOR2 finalSize = pixelSize * scale;
	window->setFinalSize(finalSize);
	// 计算左下角坐标以父窗口左下角坐标为原点的相对坐标
	// 计算缩放前的父窗口中的缩放原点
	VECTOR2 parentOrigin = VECTOR2(parentSize.x * windowPosition.x.mRelative, parentSize.y * windowPosition.y.mRelative);
	// 计算缩放前的坐标相对于缩放原点的值
	VECTOR2 scalePos = localPixelPosition + VECTOR2(pixelSize.x * windowPosition.x.mRelative, pixelSize.y * windowPosition.y.mRelative);
	VECTOR2 relativePos = scalePos - parentOrigin;
	// 对该值进行缩放
	relativePos = relativePos * scale;
	// 计算缩放后的父窗口中的缩放原点
	VECTOR2 finalParentOrigin = VECTOR2(parentFinalSize.x * windowPosition.x.mRelative, parentFinalSize.y * windowPosition.y.mRelative);
	// 计算缩放后的坐标相对于缩放后的缩放原点的值
	scalePos = finalParentOrigin + relativePos;
	// 计算缩放后的窗口坐标
	localPixelPosition = scalePos - VECTOR2(finalSize.x * windowPosition.x.mRelative, finalSize.y * windowPosition.y.mRelative);

	if (parent != NULL)
	{
		const VECTOR2& parentFinalPosition = parent->getFinalPosition();
		window->setFinalPosition(parentFinalPosition + localPixelPosition);
	}
	else
	{
		window->setFinalPosition(localPixelPosition);
	}
}