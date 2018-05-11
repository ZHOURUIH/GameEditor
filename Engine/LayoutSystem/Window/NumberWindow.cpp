#include "txEngineRoot.h"
#include "txRendererManager.h"
#include "txTextureManager.h"
#include "txTexture.h"

#include "WindowHeader.h"
#include "NumberWindowProperty.h"
#include "LayoutManager.h"

bool NumberWindow::mShowNumberRect = false;
txSet<std::string> NumberWindow::mNumberStyleList;

NumberWindow::NumberWindow(const std::string& name, const WINDOW_TYPE& type)
	:
	Window(name, type),
	mMaxCount(10),
	mDockingPosition(WDP_LEFT),
	mDotTexture(NULL),
	mRotateWithWindow(true),
	mNumberDataDirty(true)
{
	for (int i = 0; i < 10; ++i)
	{
		mNumberTexture[i] = NULL;
	}
	if (mNumberStyleList.size() == 0)
	{
		refreshNumberStyleList();
	}
	refreshNumberData();
}

NumberWindow::~NumberWindow()
{
	int dataCount = mNumberData.size();
	FOR_STL(mNumberData, int i = 0; i < dataCount; ++i)
	{
		TRACE_DELETE(mNumberData[i]);
	}
	END_FOR_STL(mNumberData);
	mNumberData.clear();
}

void NumberWindow::update(const float& elapsedTime)
{
	if (!isVisible())
	{
		return;
	}
	Window::update(elapsedTime);
	if (mNumberDataDirty)
	{
		refreshNumberData();
		mNumberDataDirty = false;
	}
}

void NumberWindow::render()
{
	const MATRIX4& viewMatrix = mLayoutManager->getUIViewMatrix();
	const MATRIX4& curProject = mLayoutManager->getPerspectiveMatrix();
	int renderCount = (int)mNumber.size();
	if ((int)mNumberData.size() >= renderCount)
	{
		for(int i = 0; i < renderCount; ++i)
		{
			if (mNumber[i] >= '0' && mNumber[i] <= '9')
			{
				int num = mNumber[i] - '0';
				txTexture* texture = mNumberTexture[num];
				if (texture != NULL)
				{
					txRendererInstance::mUIRender->render(texture, NULL, curProject * viewMatrix, mNumberData[i]->mWindowVertices, mWindowIndices,
						mNumberData[i]->mTexCoords, mAlpha, mColour, mColourBlendMode, mHSLOffset, mSymmetryMode, mWaterEffect, mWaterTime, mNumberData[i]->mTextureSize, 2.0f * txMath::MATH_PI, false);
				}
			}
			else if (mNumber[i] == '.')
			{
				if (mDotTexture != NULL)
				{
					txRendererInstance::mUIRender->render(mDotTexture, NULL, curProject * viewMatrix, mNumberData[i]->mWindowVertices, mWindowIndices,
						mNumberData[i]->mTexCoords, mAlpha, mColour, mColourBlendMode, mHSLOffset, mSymmetryMode, mWaterEffect, mWaterTime, mNumberData[i]->mTextureSize, 2.0f * txMath::MATH_PI, false);
				}
			}
			// 渲染线框和窗口坐标系
			if (mShowNumberRect)
			{
				if (mShowWireframe)
				{
					txRendererInstance::mColourRender->render(mWireframeColour, curProject * viewMatrix, mNumberData[i]->mWireframeVertices, mWireFrameIndices, WIREFRAME_INDEX_COUNT, 1.0f);
				}
				if (mShowCoordinate)
				{
					txRendererInstance::mColourRender->render(mCoordinateColour, curProject * viewMatrix, mNumberData[i]->mCoordinateVertices, mCoordinateIndices, COORDINATE_INDEX_COUNT, 1.0f);
				}
			}
		}
	}
	Window::render();
}

void NumberWindow::setNumber(const std::string& number, const bool& refreshNow)
{ 
	mNumber = number; 
	// 设置的数字字符串不能超过最大数量
	if ((int)mNumber.length() > mMaxCount)
	{
		mNumber = mNumber.substr(0, mMaxCount);
	}
	if (!refreshNow)
	{
		setNumberDataDirty(true);
	}
	else
	{
		refreshNumberData();
	}
}

void NumberWindow::setNumberStyle(const std::string& style, const bool& async)
{
	mNumberStyle = style;
	std::string path = P_UI_NUMBER_PATH + style + "/";
	VECTOR2 texSize;
	for (int i = 0; i < 10; ++i)
	{
		std::string texName = style + "_" + txStringUtility::intToString(i) + ".swi";
		txTexture* tex = mTextureManager->createTexture(path + texName);
		if (tex != NULL)
		{
			// 非异步加载时才初始化纹理
			if (tex->initTexture(!async))
			{
				texSize = tex->getTextureSize();
			}
		}
		mNumberTexture[i] = tex;
	}
	mNumberTextureSize = texSize;

	std::string finalTextureName = path + style + "_dot.swi";
	txTexture* tex = mTextureManager->createTexture(finalTextureName);
	if (tex != NULL)
	{
		tex->initTexture(!async);
		mDotTextureSize = tex->getTextureSize();
	}
	mDotTexture = tex;
	setNumberDataDirty(true);
}

void NumberWindow::initProperty()
{
	Window::initProperty();
	addProperty<NumberWindowPropertyNumber>();
	addProperty<NumberWindowPropertyNumberStyle>();
	addProperty<NumberWindowPropertyNumberInterval>();
	addProperty<NumberWindowPropertyMaxNumberCount>();
	addProperty<NumberWindowPropertyNumberDockingPosition>();
	addProperty<NumberWindowPropertyRotateWithWindow>();

	// 为风格属性添加候选列表
	NumberWindowPropertyNumberStyle* styleProperty = getProperty<NumberWindowPropertyNumberStyle>();
	if (styleProperty->getChoices() != NULL && styleProperty->getChoices()->size() == 0)
	{
		txVector<std::string> styleList;
		auto iterStyle = mNumberStyleList.begin();
		auto iterStyleEnd = mNumberStyleList.end();
		FOR_STL(mNumberStyleList, ; iterStyle != iterStyleEnd; ++iterStyle)
		{
			styleList.push_back(*iterStyle);
		}
		END_FOR_STL(mNumberStyleList);
		styleProperty->setChoices(styleList);
	}
}

void NumberWindow::finishAsycLoad()
{
	for (int i = 0; i < 10; ++i)
	{
		if (mNumberTexture[i] != NULL)
		{
			mNumberTexture[i]->loadGLTexture();
		}
	}
	if (mDotTexture != NULL)
	{
		mDotTexture->loadGLTexture();
	}
}

void NumberWindow::getUsedResourceList(txVector<std::string>& resourceList)
{
	for (int i = 0; i < 10; ++i)
	{
		if (mNumberTexture[i] != NULL)
		{
			resourceList.push_back(mNumberTexture[i]->getName());
		}
	}
	if (mDotTexture != NULL)
	{
		resourceList.push_back(mDotTexture->getName());
	}
}

void NumberWindow::refreshNumberStyleList()
{
	mNumberStyleList.clear();
	txVector<std::string> fileList;
	txFileUtility::findFiles(P_UI_NUMBER_PATH, fileList, ".swi");
	int fileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < fileCount; ++i)
	{
		// 获得每张数字图片的风格
		std::string fileName = txStringUtility::getFileName(fileList[i]);
		int pos = fileName.find_last_of('_');
		if (pos != -1)
		{
			std::string style = fileName.substr(0, pos);
			// 如果该风格不存在,则放入列表
			if (mNumberStyleList.find(style) == mNumberStyleList.end())
			{
				mNumberStyleList.insert(style);
			}
		}
	}
	END_FOR_STL(fileList);
}

void NumberWindow::refreshNumberData()
{
	// 如果不足,则扩展空间
	if (mMaxCount > (int)mNumberData.size())
	{
		int expandCount = mMaxCount - mNumberData.size();
		for (int i = 0; i < expandCount; ++i)
		{
			NumberRenderData* renderData = TRACE_NEW(NumberRenderData, renderData);
			mNumberData.push_back(renderData);
		}
	}
	int renderCount = mNumber.length();
	if (renderCount > mMaxCount)
	{
		ENGINE_ERROR("error : number count can not be more than max count! number count : %d, max count : %d", renderCount, mMaxCount);
		return;
	}

	// 将没有使用的数据清空,其实不清空也行
	int emptyCount = mNumberData.size() - renderCount;
	for (int i = 0; i < emptyCount; ++i)
	{
		memset(mNumberData[renderCount + i]->mWindowVertices, 0, sizeof(GLfloat) * UI_VERTEX_COUNT * 3);
		memset(mNumberData[renderCount + i]->mTexCoords, 0, sizeof(GLfloat) * UI_TEXTURE_COORD_COUNT * 2);
		memset(mNumberData[renderCount + i]->mWireframeVertices, 0, sizeof(GLfloat) * UI_VERTEX_COUNT * 3);
		memset(mNumberData[renderCount + i]->mCoordinateVertices, 0, sizeof(GLfloat) * UI_VERTEX_COUNT * 3);
		mNumberData[renderCount + i]->mTextureCoord = txMath::VEC2_ZERO;
		mNumberData[renderCount + i]->mTextureSize = txMath::VEC2_ZERO;
	}

	// 没有需要渲染的数字不再计算
	if (renderCount == 0)
	{
		return;
	}

	// 数字图片的宽高比
	float numberTextureAspect = 0.0f;
	if (!txMath::isFloatZero(mNumberTextureSize.y))
	{
		numberTextureAspect = mNumberTextureSize.x / mNumberTextureSize.y;
	}

	// 数字的最终渲染高度与窗口的高度相同
	VECTOR2 numberRenderSize;
	numberRenderSize.y = mFinalSize.y;
	numberRenderSize.x = numberTextureAspect * numberRenderSize.y;
	float textureScale = 0.0f;
	if (!txMath::isFloatZero(mNumberTextureSize.y))
	{
		textureScale = numberRenderSize.y / mNumberTextureSize.y;
	}
	VECTOR2 dotRenderSize = textureScale * mDotTextureSize;

	int contentWidth = 0;
	int dotPos = mNumber.find(".");
	//有小数点
	if (dotPos != -1)
	{
		// 小数点在最大数量之后
		if (dotPos >= mMaxCount)
		{
			contentWidth = int(txMath::getMin((renderCount - 1), mMaxCount) * numberRenderSize.x);
		}
		// 小数点在最大数量之前
		else
		{
			contentWidth = int(dotRenderSize.x + txMath::getMin((renderCount - 1), mMaxCount) * numberRenderSize.x);
		}
	}
	// 没有小数点
	else
	{
		// 取当前要渲染的个数个最大数量中最小的数
		contentWidth = int(txMath::getMin(renderCount, mMaxCount) * numberRenderSize.x);
	}

	int interval = (int)(mFinalSize.y * mInterval.mRelative + mInterval.mAbsolute);
	// 当渲染个数大于1个时,渲染宽度还要考虑数字之间的间隙
	if (renderCount > 1)
	{
		contentWidth += interval * (renderCount - 1);
	}

	VECTOR2 renderPos;
	if (mDockingPosition == WDP_LEFT)
	{
		renderPos = mFinalPosition;
	}
	else if (mDockingPosition == WDP_RIGHT)
	{
		renderPos = mFinalPosition + VECTOR2(mFinalSize.x - contentWidth, 0.0f);
	}
	else if (mDockingPosition == WDP_CENTER)
	{
		renderPos = mFinalPosition + VECTOR2((mFinalSize.x - contentWidth) / 2.0f, 0.0f);
	}

	if (mRotateWithWindow)
	{
		VECTOR2 renderSize((float)contentWidth, numberRenderSize.y);
		VECTOR3 transformCenter = VECTOR3(mTransformCenter.x * mFinalSize.x, mTransformCenter.y * mFinalSize.y, mTransformCenter.z);
		MATRIX4 numberTransform(1.0f);
		for (int i = 0; i < renderCount; ++i)
		{
			// 当前数字相对于数字窗口的平移矩阵
			txMath::setMatrixPosition(numberTransform, VECTOR3(renderPos.x - mFinalPosition.x, 0.0f, 0.0f));
			if (mNumber[i] >= '0' && mNumber[i] <= '9')
			{
				txTexture* texture = mNumberTexture[mNumber[i] - '0'];
				if (texture != NULL)
				{
					// 顶点数据
					LayoutUtility::generateVertices(mNumberData[i]->mWindowVertices, mFinalPosition, numberRenderSize, mFinalMatrix * numberTransform, transformCenter, true, mLayoutManager->getFovY(), mLayoutManager->getDirectVertices());
					// 线框和坐标系的顶点数据
					LayoutUtility::generateWireframeCoordinate(mNumberData[i]->mWireframeVertices, mNumberData[i]->mCoordinateVertices, mNumberData[i]->mWindowVertices);
					// 纹理坐标
					LayoutUtility::generateTexCoord(texture, mNumberData[i]->mTexCoords, mSymmetryMode, txMath::VEC2_ZERO, mNumberTextureSize);
					mNumberData[i]->mTextureCoord = mNumberTextureSize;
					renderPos += VECTOR2(numberRenderSize.x + interval, 0.0f);
				}
			}
			else if (mNumber[i] == '.')
			{
				if (mDotTexture != NULL)
				{
					// 顶点数据
					LayoutUtility::generateVertices(mNumberData[i]->mWindowVertices, mFinalPosition, dotRenderSize, mFinalMatrix * numberTransform, transformCenter, true, mLayoutManager->getFovY(), mLayoutManager->getDirectVertices());
					// 线框和坐标系的顶点数据
					LayoutUtility::generateWireframeCoordinate(mNumberData[i]->mWireframeVertices, mNumberData[i]->mCoordinateVertices, mNumberData[i]->mWindowVertices);
					// 纹理坐标
					LayoutUtility::generateTexCoord(mDotTexture, mNumberData[i]->mTexCoords, mSymmetryMode, txMath::VEC2_ZERO, mDotTextureSize);
				}
				renderPos += VECTOR2(dotRenderSize.x + interval, 0.0f);
			}
		}
	}
	else
	{
		for (int i = 0; i < renderCount; ++i)
		{
			if (mNumber[i] >= '0' && mNumber[i] <= '9')
			{
				txTexture* texture = mNumberTexture[mNumber[i] - '0'];
				if (NULL != texture)
				{
					// 顶点数据
					LayoutUtility::generateVertices(mNumberData[i]->mWindowVertices, renderPos, numberRenderSize, mFinalMatrix, mTransformCenter, false, mLayoutManager->getFovY(), mLayoutManager->getDirectVertices());
					// 线框和坐标系的顶点数据
					LayoutUtility::generateWireframeCoordinate(mNumberData[i]->mWireframeVertices, mNumberData[i]->mCoordinateVertices, mNumberData[i]->mWindowVertices);
					// 纹理坐标
					LayoutUtility::generateTexCoord(texture, mNumberData[i]->mTexCoords, mSymmetryMode, txMath::VEC2_ZERO, mNumberTextureSize);
					renderPos += VECTOR2(numberRenderSize.x + interval, 0.0f);
				}
			}
			else if (mNumber[i] == '.')
			{
				if (mDotTexture != NULL)
				{
					// 顶点数据
					LayoutUtility::generateVertices(mNumberData[i]->mWindowVertices, renderPos, dotRenderSize, mFinalMatrix, mTransformCenter, false, mLayoutManager->getFovY(), mLayoutManager->getDirectVertices());
					// 线框和坐标系的顶点数据
					LayoutUtility::generateWireframeCoordinate(mNumberData[i]->mWireframeVertices, mNumberData[i]->mCoordinateVertices, mNumberData[i]->mWindowVertices);
					// 纹理坐标
					LayoutUtility::generateTexCoord(mDotTexture, mNumberData[i]->mTexCoords, mSymmetryMode, txMath::VEC2_ZERO, mDotTextureSize);
				}
				renderPos += VECTOR2(dotRenderSize.x + interval, 0.0f);
			}
		}
	}
}