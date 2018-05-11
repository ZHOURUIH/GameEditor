#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txURect.h"
#include "txRendererInstance.h"
#include "txTexture.h"

#include "LayoutUtility.h"
#include "WindowHeader.h"
#include "LayoutManager.h"

txMap<WINDOW_TYPE, std::string> LayoutUtility::mWindowTypeToTypeNameMap;
txMap<std::string, WINDOW_TYPE> LayoutUtility::mTypeNameToWindowTypeMap;

LayoutUtility::LayoutUtility()
{
	mWindowTypeToTypeNameMap.insert(WT_PREFAB, PREFAB_TYPE_NAME);
	mWindowTypeToTypeNameMap.insert(WT_WINDOW, "Window");
	mWindowTypeToTypeNameMap.insert(WT_BUTTON, "Button");
	mWindowTypeToTypeNameMap.insert(WT_EDITBOX, "EditBox");
	mWindowTypeToTypeNameMap.insert(WT_CHECKBOX, "CheckBox");
	mWindowTypeToTypeNameMap.insert(WT_NUMBER_WINDOW, "NumberWindow");
	mWindowTypeToTypeNameMap.insert(WT_PROGRESS_WINDOW, "ProgressWindow");
	mWindowTypeToTypeNameMap.insert(WT_TEXTURE_WINDOW, "TextureWindow");
	mWindowTypeToTypeNameMap.insert(WT_TEXTURE_ANIM_WINDOW, "TextureAnimWindow");
	mWindowTypeToTypeNameMap.insert(WT_TEXT_WINDOW, "TextWindow");
	mWindowTypeToTypeNameMap.insert(WT_VIDEO_WINDOW, "VideoWindow");

	auto iterType = mWindowTypeToTypeNameMap.begin();
	auto iterTypeEnd = mWindowTypeToTypeNameMap.end();
	FOR_STL(mWindowTypeToTypeNameMap, ; iterType != iterTypeEnd; ++iterType)
	{
		mTypeNameToWindowTypeMap.insert(iterType->second, iterType->first);
	}
	END_FOR_STL(mWindowTypeToTypeNameMap);
	if (mTypeNameToWindowTypeMap.size() < WT_MAX)
	{
		ENGINE_ERROR("error : not all window type registered!");
	}
}

VECTOR2 LayoutUtility::getWindowPosition(Window* pWindow)
{
	if (pWindow == NULL)
	{
		return txMath::VEC2_ZERO;
	}
	else
	{
		VECTOR2 parentPosition = getWindowPosition(pWindow->getParent());
		VECTOR2 parentSize = getWindowSize(pWindow->getParent());
		const txPoint& windowPointPos = pWindow->getPosition();
		VECTOR2 position = txMath::pointToVector2(windowPointPos, parentSize) + parentPosition;
		return position;
	}
}

VECTOR2 LayoutUtility::getRelativeWindowPosition(Window* pWindow)
{
	VECTOR2 parentSize = getWindowSize(pWindow->getParent());
	const txPoint& windowPoint = pWindow->getPosition();
	VECTOR2 position = txMath::pointToVector2(windowPoint, parentSize);
	return position;
}

VECTOR2 LayoutUtility::getWindowSize(Window* pWindow)
{
	if (pWindow == NULL)
	{
		return getScreenSize();
	}
	else
	{
		VECTOR2 parentSize = getWindowSize(pWindow->getParent());
		txPoint windowSize = pWindow->getSize();
		// 不自动计算宽高,则直接返回实际的宽高
		if (pWindow->getAutoSizeType() == AST_NONE)
		{
			VECTOR2 size;
			size.x = parentSize.x * windowSize.x.mRelative + windowSize.x.mAbsolute;
			size.y = parentSize.y * windowSize.y.mRelative + windowSize.y.mAbsolute;
			return size;
		}
		// 自动计算高度
		else if (pWindow->getAutoSizeType() == AST_AUTO_HEIGHT)
		{
			VECTOR2 size;
			size.x = parentSize.x * windowSize.x.mRelative + windowSize.x.mAbsolute;
			const txDim& autoHeight = pWindow->getAutoHeight();
			size.y = size.x * autoHeight.mRelative + autoHeight.mAbsolute;
			return size;
		}
		// 自动计算宽度
		else if (pWindow->getAutoSizeType() == AST_AUTO_WIDTH)
		{
			VECTOR2 size;
			size.y = parentSize.y * windowSize.y.mRelative + windowSize.y.mAbsolute;
			const txDim& autoWidth = pWindow->getAutoWidth();
			size.x = size.y * autoWidth.mRelative + autoWidth.mAbsolute;
			return size;
		}
		return txMath::VEC2_ZERO;
	}
}

txRect LayoutUtility::getWindowRect(Window* pWindow)
{
	if (pWindow == NULL)
	{
		return txRect(txMath::VEC2_ZERO, getScreenSize());
	}
	else
	{
		txRect parentRect = getWindowRect(pWindow->getParent());
		const txURect& windowRect = pWindow->getRect();
		txRect rect;
		rect.mMin.x = parentRect.mMin.x * windowRect.mMin.x.mRelative + windowRect.mMin.x.mAbsolute;
		rect.mMin.y = parentRect.mMin.y * windowRect.mMin.y.mRelative + windowRect.mMin.y.mAbsolute;

		// 不自动计算宽高,则直接返回实际的宽高
		if (pWindow->getAutoSizeType() == AST_NONE)
		{
			rect.mMax.x = parentRect.mMax.x * windowRect.mMax.x.mRelative + windowRect.mMax.x.mAbsolute;
			rect.mMax.y = parentRect.mMax.y * windowRect.mMax.y.mRelative + windowRect.mMax.y.mAbsolute;
		}
		// 自动计算高度
		else if (pWindow->getAutoSizeType() == AST_AUTO_HEIGHT)
		{
			rect.mMax.x = parentRect.mMax.x * windowRect.mMax.x.mRelative + windowRect.mMax.x.mAbsolute;
			const txDim& autoHeight = pWindow->getAutoHeight();
			rect.mMax.y = rect.mMax.x * autoHeight.mRelative + autoHeight.mAbsolute;
		}
		// 自动计算宽度
		else if (pWindow->getAutoSizeType() == AST_AUTO_WIDTH)
		{
			rect.mMax.y = parentRect.mMax.y * windowRect.mMax.y.mRelative + windowRect.mMax.y.mAbsolute;
			const txDim& autoWidth = pWindow->getAutoWidth();
			rect.mMax.x = rect.mMax.y * autoWidth.mRelative + autoWidth.mAbsolute;
		}
		return rect;
	}
}

VECTOR3 LayoutUtility::getPickupRayDirection(const VECTOR2& screenPoint)
{
	// 将屏幕坐标点转换到-1~1之间
	const int& renderWindowWdith = mRenderWindow->getWidth();
	const int& renderWindowHeight = mRenderWindow->getHeight();
	VECTOR2 relativeScreenPos = screenPoint;
	relativeScreenPos.x -= renderWindowWdith / 2.0f;
	relativeScreenPos.x /= renderWindowWdith / 2.0f;
	relativeScreenPos.y -= renderWindowHeight / 2.0f;
	relativeScreenPos.y /= renderWindowHeight / 2.0f;
	// 然后经过投影矩阵和视图矩阵的逆计算
	VECTOR3 worldPoint = txMath::transformVector3(VECTOR3(relativeScreenPos.x, relativeScreenPos.y, 1.0f), mLayoutManager->getInversePerspectiveMatrix(), true);
	worldPoint = txMath::transformVector3(worldPoint, mLayoutManager->getInverseUIViewMatrix(), true);
	return txMath::normalize(worldPoint);
}

const int& LayoutUtility::getScreenWidth()
{
	return mRenderWindow->getWidth();
}

const int& LayoutUtility::getScreenHeight()
{
	return mRenderWindow->getHeight();
}

VECTOR2 LayoutUtility::getScreenSize()
{
	return VECTOR2((float)getScreenWidth(), (float)getScreenHeight());
}

VECTOR2 LayoutUtility::getWindowPositionInScreen(Window* window)
{
	// 当自己没有父窗口时,返回自己在屏幕中的像素坐标
	if (window->getParent() == NULL)
	{
		return getWindowPosition(window);
	}
	// 当自己有父窗口时,就先计算父窗口在屏幕中的像素坐标,然后再加上自己在父窗口中的像素坐标
	else
	{
		return getWindowPositionInScreen(window->getParent()) + getWindowPosition(window);
	}
}

void LayoutUtility::generateVertices(GLfloat* vertices, const VECTOR2& windowPosition, const VECTOR2& windowSize, const MATRIX4& transform,
	const VECTOR3& transformCenter, const bool& isAbsoluteCenter, const float& fovy, const bool& directVertices)
{
	const int& screenWidth = mRenderWindow->getWidth();
	const int& screenHeight = mRenderWindow->getHeight();
	VECTOR3 point[UI_VERTEX_COUNT];
	VECTOR2 windowPos = windowPosition;
	if (directVertices)
	{
		// 将窗口坐标转换到以屏幕中心为原点的坐标
		windowPos -= VECTOR2(screenWidth / 2.0f, screenHeight / 2.0f);
		point[0] = VECTOR3(windowPos.x, windowPos.y, 0.0f);									// 屏幕左下角
		point[1] = VECTOR3(windowPos.x + windowSize.x, windowPos.y, 0.0f);					// 屏幕右下角
		point[2] = VECTOR3(windowPos.x + windowSize.x, windowPos.y + windowSize.y, 0.0f);	// 屏幕右上角
		point[3] = VECTOR3(windowPos.x, windowPos.y + windowSize.y, 0.0f);					// 屏幕左上角
		VECTOR3 wSize = point[2] - point[0];
		VECTOR3 windowCenter;
		// 计算最终的变换中心
		if (!isAbsoluteCenter)
		{
			// z由于是绝对的值,所以只能相加
			windowCenter = point[0] + VECTOR3(wSize.x * transformCenter.x, wSize.y * transformCenter.y, wSize.z + transformCenter.z);
		}
		else
		{
			windowCenter = transformCenter;
		}
		for (int i = 0; i < UI_VERTEX_COUNT; ++i)
		{
			// 首先默认转到以窗口中心为原点的坐标,然后进行坐标变换,然后再转回屏幕左下角为原点的坐标,所以所有的变换都是以窗口中心为准
			point[i] = point[i] - windowCenter;
			point[i] = txMath::transformVector3(point[i], transform);
			point[i] = point[i] + windowCenter;
			// 将坐标缩放到-1.0f到1.0f之间
			point[i].x /= (screenWidth / 2.0f);
			point[i].y /= (screenHeight / 2.0f);
		}
	}
	else
	{
		// 计算UI平面到摄像机的距离
		float uiClipDis = -generateUIClipZ(screenHeight, fovy);
		point[0] = VECTOR3(windowPos.x, windowPos.y, uiClipDis); // 屏幕左下角
		point[1] = VECTOR3(windowPos.x + windowSize.x, windowPos.y, uiClipDis);// 屏幕右下角
		point[2] = VECTOR3(windowPos.x + windowSize.x, windowPos.y + windowSize.y, uiClipDis);// 屏幕右上角
		point[3] = VECTOR3(windowPos.x, windowPos.y + windowSize.y, uiClipDis);// 屏幕左上角
		// 先将所有坐标平移(-width / 2.0f, -height / 2.0f),保证处于对齐世界坐标系原点
		for (int i = 0; i < 4; ++i)
		{
			point[i] -= VECTOR3(screenWidth / 2.0f, screenHeight / 2.0f, 0.0f);
		}
		VECTOR3 wSize = point[2] - point[0];
		VECTOR3 curTransformCenter = transformCenter;
		// 计算最终的变换中心
		if (!isAbsoluteCenter)
		{
			// z由于是绝对的值,所以只能相加
			curTransformCenter = VECTOR3(wSize.x * transformCenter.x, wSize.y * transformCenter.y, wSize.z + transformCenter.z);
		}
		VECTOR3 windowCenter = point[0] + curTransformCenter;
		for (int i = 0; i < UI_VERTEX_COUNT; ++i)
		{
			// 首先默认转到以窗口中心为原点的坐标,然后进行坐标变换,然后再转回屏幕左下角为原点的坐标,所以所有的变换都是以窗口中心为准
			point[i] = point[i] - windowCenter;
			point[i] = txMath::transformVector3(point[i], transform);
			point[i] = point[i] + windowCenter;
		}
	}
	// 顶点坐标
	for (int i = 0; i < UI_VERTEX_COUNT; ++i)
	{
		vertices[i * 3 + 0] = point[i].x;
		vertices[i * 3 + 1] = point[i].y;
		vertices[i * 3 + 2] = point[i].z;
	}
}

void LayoutUtility::generateTexCoord(txTexture* tex, GLfloat* texcoords, SYMMETRY_MODE& symmetryMode, const VECTOR2& texCoord, const VECTOR2& texSize)
{
	if (texcoords != NULL)
	{
		// 所属图片的宽高
		float texWidth = 1.0f, texHeight = 1.0f;
		// 纹理区域的起点和大小
		VECTOR2 curTexCoord = texCoord;
		VECTOR2 curTexSize = texSize;
		if (tex == NULL)
		{
			curTexCoord = txMath::VEC2_ZERO;
			curTexSize = txMath::VEC2_ZERO;
			for (int i = 0; i < UI_TEXTURE_COORD_COUNT * 2; ++i)
			{
				texcoords[i] = 0.0f;
			}
		}
		else
		{
			texWidth = tex->getTextureSize().x;
			texHeight = tex->getTextureSize().y;

			// 左右镜像或者左右拷贝时,纹理起点坐标不在0,0或者宽高不等于图片宽高,都不能做对称处理
			if (symmetryMode == SM_LEFT_COPY_TO_RIGHT || symmetryMode == SM_LEFT_MIRROR_TO_RIGHT)
			{
				if (!txMath::isFloatZero(curTexCoord.x) || !txMath::isFloatZero(curTexCoord.y) ||
					!txMath::isFloatZero(curTexSize.x - texWidth) || !txMath::isFloatZero(curTexSize.y - texHeight))
				{
					symmetryMode = SM_NONE;
				}
			}
			// 不做对称处理
			if (symmetryMode == SM_NONE)
			{
				int vertexIndex = 0;
				// 屏幕左下角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕右下角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕右上角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// 屏幕左上角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
			}
			// 将左边的直接复制到右边或将左边的镜像复制到右边
			else if (symmetryMode == SM_LEFT_COPY_TO_RIGHT || symmetryMode == SM_LEFT_MIRROR_TO_RIGHT)
			{
				// 纹理只铺满窗口的左半边
				int vertexIndex = 0;
				// 屏幕左下角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕右下角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth * 2.0f;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕右上角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth * 2.0f;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// 屏幕左上角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
			}
			// 水平翻转
			else if (symmetryMode == SM_HORI_INVERSE)
			{
				int vertexIndex = 0;
				// 屏幕左下角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕右下角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕右上角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// 屏幕左上角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
			}
			// 竖直翻转
			else if (symmetryMode == SM_VERT_INVERSE)
			{
				int vertexIndex = 0;
				// 屏幕左下角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// 屏幕右下角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// 屏幕右上角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕左上角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
			}
			else if (symmetryMode == SM_HORI_VERT_MIRROR)
			{
				int vertexIndex = 0;
				// 屏幕左下角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕右下角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth * 2.0f;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y) / texHeight;
				++vertexIndex;

				// 屏幕右上角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth * 2.0f;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight * 2.0f;
				++vertexIndex;

				// 屏幕左上角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight * 2.0f;
			}
			else if (symmetryMode == SM_BOTTOM_COPY_TO_TOP || symmetryMode == SM_BOTTOM_MIRROR_TO_TOP)
			{
				// 纹理只铺满窗口的下半边
				int vertexIndex = 0;
				// 屏幕左下角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕右下角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// 屏幕右上角
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight * 2.0f;
				++vertexIndex;

				// 屏幕左上角
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight * 2.0f;
			}
		}
	}
}

void LayoutUtility::generateWireframeCoordinate(GLfloat* wireframeVertices, GLfloat* coordinateVertices, GLfloat* vertices)
{
	// 线框顶点坐标
	if (wireframeVertices != NULL)
	{
		memcpy(wireframeVertices, vertices, sizeof(GLfloat)* 3 * UI_VERTEX_COUNT);
	}
	// 窗口坐标系顶点坐标
	if (coordinateVertices != NULL)
	{
		// 计算窗口坐标系
		VECTOR3 point[UI_VERTEX_COUNT];
		for (int i = 0; i < UI_VERTEX_COUNT; ++i)
		{
			point[i] = VECTOR3(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]);
		}
		VECTOR3 coordinateCenter((point[0] + point[2]) / 2.0f);
		for (int i = 0; i < UI_VERTEX_COUNT; ++i)
		{
			VECTOR3 middlePoint = (point[i] + point[(i + 1) % UI_VERTEX_COUNT]) / 2.0f;
			VECTOR3 relativePoint = middlePoint - coordinateCenter;
			txMath::setLength(relativePoint, 2000.0f);
			middlePoint = coordinateCenter + relativePoint;
			coordinateVertices[i * 3 + 0] = middlePoint.x;
			coordinateVertices[i * 3 + 1] = middlePoint.y;
			coordinateVertices[i * 3 + 2] = middlePoint.z;
		}
	}
}