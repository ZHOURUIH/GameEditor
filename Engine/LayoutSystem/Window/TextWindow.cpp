#include "txEngineRoot.h"
#include "txRendererManager.h"
#include "txTexture.h"
#include "txFreeTypeFont.h"
#include "txTextureManager.h"

#include "WindowHeader.h"
#include "TextWindowProperty.h"
#include "LayoutManager.h"

bool TextWindow::mShowTextRect = false;

TextWindow::TextWindow(const std::string& name, const WINDOW_TYPE& type)
	:
	Window(name, type),
	mFontName("STKAITI"),
	mRotateWithWindow(true),
	mTextDataDirty(true),
	mMaxLength(32),
	mDockingPosition(WDP_CENTER)
{
	;
}

TextWindow::~TextWindow()
{
	int dataCount = mTextData.size();
	FOR_STL(mTextData, int i = 0; i < dataCount; ++i)
	{
		TRACE_DELETE(mTextData[i]);
	}
	END_FOR_STL(mTextData);
	mTextData.clear();
}

void TextWindow::update(const float& elapsedTime)
{
	if (!isVisible())
	{
		return;
	}
	Window::update(elapsedTime);
	// 渲染之前确保数据已经刷新
	if (mTextDataDirty)
	{
		refreshTextData();
		mTextDataDirty = false;
	}
}

void TextWindow::render()
{
	const MATRIX4& viewMatrix = mLayoutManager->getUIViewMatrix();
	const MATRIX4& curProject = mLayoutManager->getPerspectiveMatrix();
	int length = mUnicodeText.length();
	if ((int)mTextData.size() >= length)
	{
		for (int i = 0; i < length; ++i)
		{
			VECTOR2 textureSize = VECTOR2(mTextData[i]->mTextWidth, mTextData[i]->mTextHeight);
			txRendererInstance::mUIRender->render(mTextData[i]->mTexture, NULL, curProject * viewMatrix,
				mTextData[i]->mWindowVertices, mWindowIndices, mTextData[i]->mTexCoords, mAlpha,
				mColour, mColourBlendMode, mHSLOffset, mSymmetryMode, mWaterEffect, mWaterTime, textureSize, 2.0f * txMath::MATH_PI, false);

			// 渲染线框和窗口坐标系
			if (mShowTextRect)
			{
				if (mShowWireframe)
				{
					txRendererInstance::mColourRender->render(mWireframeColour, curProject * viewMatrix, mTextData[i]->mWireframeVertices, mWireFrameIndices, WIREFRAME_INDEX_COUNT, 1.0f);
				}
				if (mShowCoordinate)
				{
					txRendererInstance::mColourRender->render(mCoordinateColour, curProject * viewMatrix, mTextData[i]->mCoordinateVertices, mCoordinateIndices, COORDINATE_INDEX_COUNT, 1.0f);
				}
			}
		}
	}

	Window::render();
}

void TextWindow::setFontName(const std::string& fontName, const bool& async)
{
	mFontName = fontName;
	setText(mText, async);
}

void TextWindow::setText(const std::string& text, const bool& async)
{
	mText = text;
	mUnicodeText = txStringUtility::ANSIToUnicode(mText);
	if ((int)mUnicodeText.length() > mMaxLength)
	{
		mUnicodeText = mUnicodeText.substr(0, mMaxLength);
		mText = txStringUtility::UnicodeToANSI(mUnicodeText);
	}
	setTextDataDirty(true);
}

int TextWindow::getContentWidth()
{
	// 文本总的渲染宽度
	int length = mTextData.size();
	int renderWidth = 0;
	for (int i = 0; i < length; ++i)
	{
		renderWidth += mTextData[i]->mTextWidth + mTextData[i]->mTextLeft;
	}

	// 当渲染个数大于1个时,渲染宽度还要考虑文字之间的间隙
	if (length > 1)
	{
		// 实际的渲染间隙
		int space = (int)(mFinalSize.y * mSpace.mRelative + mSpace.mAbsolute);
		renderWidth += space * (length - 1);
	}
	return renderWidth;
}

void TextWindow::finishAsycLoad()
{
	refreshTextData();
}

void TextWindow::initProperty()
{
	Window::initProperty();
	addProperty<TextWindowPropertyText>();
	addProperty<TextWindowPropertyTextDockingPosition>();
	addProperty<TextWindowPropertyFont>();
	addProperty<TextWindowPropertyTextRotateWithWindow>();
	addProperty<TextWindowPropertyTextSpace>();
	addProperty<TextWindowPropertyMaxTextCount>();

	// 初始化字体候选列表
	TextWindowPropertyFont* fontProperty = getProperty<TextWindowPropertyFont>();
	if (fontProperty->getChoices() != NULL && fontProperty->getChoices()->size() == 0)
	{
		fontProperty->setChoices(mFreeTypeFont->getFontFileNameList());
	}
}

//初始化字体纹理
txTexture* TextWindow::initFontTexture(const wchar_t& key, int& textLeft, int& textTop, int& textWidth, int& textHeight)
{
	VECTOR2 finalSize = mFinalSize;
	if (txMath::isFloatZero(finalSize.x))
	{
		finalSize = LayoutUtility::getWindowSize(this);
	}
	int height = (int)finalSize.y;
	// 高度不能小于10,否则在创建字体纹理时会崩溃
	if (height < 10)
	{
		height = 10;
	}
	int width = height;
	wchar_t keyStr[2] = {key, 0};
	std::string ansiKey = txStringUtility::UnicodeToANSI(keyStr);
	std::string realFontName = ansiKey + "_" + mFontName + "_" + txStringUtility::intToString(width) + "_" + txStringUtility::intToString(height);
	txTexture* texture = mTextureManager->createTexture(realFontName);
	if (texture == NULL)
	{
		return NULL;
	}
	unsigned char* buffer = TRACE_NEW_ARRAY(unsigned char, width * height * 4, buffer);
	memset(buffer, 0, width * height * 4);
	if (mFreeTypeFont->getFontBitmapData(mFontName, key, buffer, width, height, textLeft, textTop, textWidth, textHeight))
	{
		txBinaryUtility::upsideDownBuffer((char*)buffer, width, 0, 0, textWidth, textHeight, 4);
		texture->createCustomTexture((char*)buffer, width, height, 4, true);
	}

	TRACE_DELETE_ARRAY(buffer);

	return texture;
}
	
void TextWindow::refreshTextData()
{
	// 如果不足,则扩展空间
	if (mMaxLength > (int)mTextData.size())
	{
		int expandCount = mMaxLength - mTextData.size();
		for (int i = 0; i < expandCount; ++i)
		{
			TextRenderData* renderData = TRACE_NEW(TextRenderData, renderData);
			mTextData.push_back(renderData);
		}
	}
	int renderCount = mUnicodeText.length();
	if (renderCount > mMaxLength)
	{
		ENGINE_ERROR("error : text count can not be more than max count! text count : %d, max count : %d", renderCount, mMaxLength);
		return;
	}

	// 将没有使用的数据清空,其实不清空也行
	int emptyCount = mTextData.size() - renderCount;
	for (int i = 0; i < emptyCount; ++i)
	{
		memset(mTextData[renderCount + i]->mWindowVertices, 0, sizeof(GLfloat)* UI_VERTEX_COUNT * 3);
		memset(mTextData[renderCount + i]->mTexCoords, 0, sizeof(GLfloat)* UI_TEXTURE_COORD_COUNT * 2);
		memset(mTextData[renderCount + i]->mWireframeVertices, 0, sizeof(GLfloat)* UI_VERTEX_COUNT * 3);
		memset(mTextData[renderCount + i]->mCoordinateVertices, 0, sizeof(GLfloat)* UI_VERTEX_COUNT * 3);
		mTextData[renderCount + i]->mTextureCoord = txMath::VEC2_ZERO;
		mTextData[renderCount + i]->mTextureSize = txMath::VEC2_ZERO;
		mTextData[renderCount + i]->mTexture = NULL;
		mTextData[renderCount + i]->mTextLeft = 0;
		mTextData[renderCount + i]->mTextTop = 0;
		mTextData[renderCount + i]->mTextWidth = 0;
		mTextData[renderCount + i]->mTextHeight = 0;
	}

	// 生成渲染纹理
	// 当显示字符改变,字体改变,窗口高度改变时,都要刷新文字纹理数据
	int unicodeCount = mUnicodeText.length();
	for (int i = 0; i < unicodeCount; ++i)
	{
		mTextData[i]->mTexture = initFontTexture(mUnicodeText[i], mTextData[i]->mTextLeft, mTextData[i]->mTextTop, mTextData[i]->mTextWidth, mTextData[i]->mTextHeight);
	}

	// 文本总的渲染宽度
	float renderWidth = (float)getContentWidth();
	// 实际的渲染间隙
	int space = (int)(mFinalSize.y * mSpace.mRelative + mSpace.mAbsolute);
	VECTOR2 renderPos;
	if (mDockingPosition == WDP_LEFT)
	{
		renderPos = mFinalPosition;
	}
	else if (mDockingPosition == WDP_RIGHT)
	{
		renderPos = mFinalPosition + VECTOR2(mFinalSize.x - renderWidth, 0.0f);
	}
	else if (mDockingPosition == WDP_CENTER)
	{
		renderPos = mFinalPosition + VECTOR2((mFinalSize.x - renderWidth) / 2.0f, 0.0f);
	}
	// 计算顶点数据之前,先生成纹理
	if (mRotateWithWindow)
	{
		// 当前文字相对于窗口的平移矩阵
		MATRIX4 textTransform(1.0f);
		VECTOR2 renderSize((float)renderWidth, mFinalSize.y);
		VECTOR3 transformCenter = VECTOR3(mTransformCenter.x * mFinalSize.x, mTransformCenter.y * mFinalSize.y, mTransformCenter.z);
		for (int i = 0; i < renderCount; ++i)
		{
			VECTOR2 textureSize = VECTOR2(mTextData[i]->mTextWidth, mTextData[i]->mTextHeight);
			// 顶点坐标
			float offsetY = (float)(mTextData[i]->mTextTop - mTextData[i]->mTextHeight);
			txMath::setMatrixPosition(textTransform, VECTOR3(renderPos.x - mFinalPosition.x + mTextData[i]->mTextLeft, offsetY, 0.0f));
			LayoutUtility::generateVertices(mTextData[i]->mWindowVertices, mFinalPosition, textureSize, mFinalMatrix * textTransform, transformCenter, true, 
				mLayoutManager->getFovY(), mLayoutManager->getDirectVertices());
			// 线框和坐标系顶点坐标
			LayoutUtility::generateWireframeCoordinate(mTextData[i]->mWireframeVertices, mTextData[i]->mCoordinateVertices, mTextData[i]->mWindowVertices);
			// 纹理坐标
			LayoutUtility::generateTexCoord(mTextData[i]->mTexture, mTextData[i]->mTexCoords, mSymmetryMode, txMath::VEC2_ZERO, textureSize);
			renderPos.x += textureSize.x + (float)mTextData[i]->mTextLeft + (float)space;
		}
	}
	else
	{
		for (int i = 0; i < renderCount; ++i)
		{
			VECTOR2 textureSize = VECTOR2(mTextData[i]->mTextWidth, mTextData[i]->mTextHeight);
			float offsetY = (float)(mTextData[i]->mTextTop - mTextData[i]->mTextHeight);
			// 顶点坐标
			LayoutUtility::generateVertices(mTextData[i]->mWindowVertices, renderPos + VECTOR2(mTextData[i]->mTextLeft, offsetY), textureSize, mFinalMatrix,
				mTransformCenter, false, mLayoutManager->getFovY(), mLayoutManager->getDirectVertices());
			// 线框和坐标系顶点坐标
			LayoutUtility::generateWireframeCoordinate(mTextData[i]->mWireframeVertices, mTextData[i]->mCoordinateVertices, mTextData[i]->mWindowVertices);
			// 纹理坐标
			LayoutUtility::generateTexCoord(mTextData[i]->mTexture, mTextData[i]->mTexCoords, mSymmetryMode, txMath::VEC2_ZERO, textureSize);
			renderPos.x += textureSize.x + (float)mTextData[i]->mTextLeft + (float)space;
		}
	}
}